#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/message.h>
#include <google/protobuf/text_format.h>

#include "basis.hpp"
#include "cxxopts.hpp"
#include "onnx.pb.h"

namespace {
string model;

// clang-format off
void parse(int argc, char** argv) {
  try {
    cxxopts::Options options(argv[0], " - npu command line options");
    options.positional_help("[optional args]").show_positional_help();

    options.set_width(70)
      .set_tab_expansion()
      .add_options()
      ("m,model", "onnx model", cxxopts::value<string>())
      ("h,help", "Print all command usages");

    auto result = options.parse(argc, argv);
    if (result.count("help")) {
      std::cout << options.help() << std::endl;
      exit(0);
    }

    if (result.count("model")) {
      model = result["model"].as<string>();
    } else {
      std::cout << options.help() << std::endl;
      exit(0);
    }

  } catch (const cxxopts::OptionException& e) {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }
}
// clang-format on

void read_proto_from_binary(const char* filepath, onnx::ModelProto* message) {
  std::ifstream ifs(filepath, std::ifstream::in | std::ifstream::binary);
  CHECK(ifs.is_open());

  google::protobuf::io::IstreamInputStream input(&ifs);
  google::protobuf::io::CodedInputStream codedstr(&input);

#if GOOGLE_PROTOBUF_VERSION >= 3011000
  codedstr.SetTotalBytesLimit(INT_MAX);
#else
  codedstr.SetTotalBytesLimit(INT_MAX, INT_MAX / 2);
#endif

  bool success = message->ParseFromCodedStream(&codedstr);
  CHECK(success);
}

int get_tensor_proto_data_size(const onnx::TensorProto& tp) {
  if (tp.has_raw_data()) {
    const std::string& raw_data = tp.raw_data();
    int size = (int)raw_data.size() / 4;
    return size;
  } else if (tp.data_type() == 1) {
    return tp.float_data_size();
  }

  return 0;
}
}  // namespace

int main(int argc, char** argv) {
  parse(argc, argv);

  onnx::ModelProto onnx_model;
  read_proto_from_binary(model.c_str(), &onnx_model);

  const auto& graph = onnx_model.graph();
  // auto* mutable_graph = onnx_model.mutable_graph();
  auto node_count = graph.node_size();

  // weight node and weight reshape node
  std::map<std::string, onnx::TensorProto> weights;
  for (int j = 0; j < graph.initializer_size(); j++) {
    const auto& initializer = graph.initializer(j);
    weights[initializer.name()] = initializer;
  }

  for (int i = 0; i < node_count; i++) {
    const auto& node = graph.node(i);
    const auto& op_type = node.op_type();
    const auto& op_name = node.name();

    if (op_type == "Conv") {
      cout << "Handling " << op_name << " ...\n";

      const auto& w = weights[node.input(1)];
      if (w.has_raw_data()) {
        const std::string& raw_data = w.raw_data();
        CHECK(raw_data.size() == get_tensor_proto_data_size(w));
        saveBin(op_name + ".bin", raw_data.c_str(), raw_data.size());
      } else if (w.data_type() == 1) {
        saveBin(op_name + ".bin",
                reinterpret_cast<const char*>(w.float_data().data()),
                get_tensor_proto_data_size(w) * sizeof(float));
      } else {
        // cout << op_name << " does not have raw data!\n";
      }
    }
  }

  return 0;
}
