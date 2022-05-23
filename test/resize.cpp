#include <opencv2/imgcodecs.hpp>
#include "basis.hpp"
#include "cxxopts.hpp"

namespace {
static string image;
static int height = 224;
static int width = 224;
static int channel = 3;

// clang-format off
void parse(int argc, char** argv) {
  try {
    cxxopts::Options options(argv[0], " - npu command line options");
    options.positional_help("[optional args]").show_positional_help();

    options.set_width(70)
      .set_tab_expansion()
      .add_options()
      ("i,image", "input image", cxxopts::value<string>())
      ("h,height", "resize height", cxxopts::value<int>())
      ("w,width", "resize width", cxxopts::value<int>())
      ("c,channel", "resize channel", cxxopts::value<int>())
      ("h,help", "Print all command usages");

    auto result = options.parse(argc, argv);
    if (result.count("help")) {
      std::cout << options.help() << std::endl;
      exit(0);
    }

    if (result.count("image")) {
      image = result["image"].as<string>();
    } else {
      std::cout << options.help() << std::endl;
      exit(0);
    }

    if (result.count("height")) {
      height = result["height"].as<int>();
    } else {
      std::cout << options.help() << std::endl;
      exit(0);
    }

    if (result.count("width")) {
      width = result["width"].as<int>();
    } else {
      std::cout << options.help() << std::endl;
      exit(0);
    }

    if (result.count("channel")) {
      channel = result["channel"].as<int>();
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
}  // namespace

int main(int argc, char** argv) {
  parse(argc, argv);

  auto mat = cv::imread(image, cv::IMREAD_COLOR);
  // cv::resize(frame, frame, cv::Size(HEIGHT, WIDTH));

  return 0;
}
