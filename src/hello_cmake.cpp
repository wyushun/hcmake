#include "hello_cmake.hpp"

std::string HelloCMake::operator()() {
  std::string s("hello cmake!");
  std::cout << s << std::endl;
  return s;
}
