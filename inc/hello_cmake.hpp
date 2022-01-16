#pragma once

#include "std_inc.hpp"

class HelloCMake {
 public:
  HelloCMake() = default;
  ~HelloCMake() = default;

  std::string operator()();
};
