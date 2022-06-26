#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/message.h>
#include <google/protobuf/text_format.h>

#include "basis.hpp"
#include "cxxopts.hpp"
#include "onnx.pb.h"

// clang-format off
namespace {
string model;

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
}
// clang-format on

int main(int argc, char** argv) {
  parse(argc, argv);

  return 0;
}
