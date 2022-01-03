#pragma once

#include <iostream>
#include <string>

class HelloCMake {
 public:
  HelloCMake() = default;
  ~HelloCMake() = default;

  std::string operator()();
};
