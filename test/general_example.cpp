#include "cxxopts.hpp"
#include "std_inc.hpp"

// clang-format off
void parse(int argc, char** argv, uint64_t &size) {
  try {
    cxxopts::Options options(argv[0], " - npu command line options");
    options.positional_help("[optional args]").show_positional_help();

    options.set_width(70)
      .set_tab_expansion()
      .add_options()
      ("s,size", "Memory size(MB) to be operated", cxxopts::value<uint64_t>())
      ("h,help", "Print all command usages");

    auto result = options.parse(argc, argv);
    if (result.count("help")) {
      std::cout << options.help() << std::endl;
      exit(0);
    }

    if (result.count("size")) {
      size = result["size"].as<uint64_t>();
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

int main(int argc, char** argv) {
  cout << "test cxxopts..." << endl;
  uint64_t size;
  parse(argc, argv, size);

  cout << "test any..." << endl;
  any t = size;
  cout << "any value is: " << open_source::get<uint64_t>(t) << endl;

  cout << "test optional..." << endl;
  optional<any> t2 = t;
  if (t2.has_value()) {
    // cout << "optional value is: " << open_source::get<uint64_t>(t2.value)
    //      << endl;
  }
  return 0;
}
