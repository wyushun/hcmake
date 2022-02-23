#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <numeric>
#include <string>
#include <functional>
#include <algorithm>

using namespace std;

// make a naive Tensor class:
// 1. use vector<int> to indicate shape, suppose it's valid and hasn't negative integers
// 2. data_ contents will be [0, elem_num)
// 3. stride_ means each dimension's memory size
class Tensor {
public:
  Tensor(const vector<int>& shape) : shape_(shape) {
    num_ = std::accumulate(shape_.begin(), shape_.end(),
            int(1), [](int a, int b) {return a*b;});
    stride_.resize(shape_.size());

    // initialize stride value according to shape
    auto factor = 1;
    for(int i=shape_.size()-1; i>=0; i--) {
      factor *= shape_[i];
      stride_[i] = factor / shape_[i];
    }

    // initialize data value
    auto cnt = 0;
    data_.resize(num_);
    std::transform(data_.begin(), data_.end(),
            data_.begin(), [&cnt](int value) { return (value = cnt++); });
  }

  void print();
  void print_data();

private:
  int num_;
  vector<int> shape_;
  vector<int> stride_;
  vector<int> data_;
};
