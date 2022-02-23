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
class NaiveTensor {
 public:
  NaiveTensor(const vector<int>& shape) : shape_(shape) {
    ndim_ = shape_.size();
    elem_num_ = std::accumulate(shape_.begin(), shape_.end(), int(1),
                                [](int a, int b) { return a * b; });
    stride_.resize(ndim_);

    // initialize stride value according to shape
    auto factor = 1;
    for(int i=shape_.size()-1; i>=0; i--) {
      factor *= shape_[i];
      stride_[i] = factor / shape_[i];
    }

    // initialize data value
    auto cnt = 0;
    data_.resize(elem_num_);
    std::transform(data_.begin(), data_.end(), data_.begin(),
            [&cnt](int value) { return (value = cnt++); });
  }

  void Print();
  void PrintData();

  void NaiveTranspose(const vector<int>& dim_order) {
    // get new shape and new stirde
    vector<int> new_shape;
    for (auto i = 0; i < ndim_; i++) {
      new_shape.push_back(shape_[dim_order[i]]);
    }
    if(shape_ == new_shape) { return; }
    vector<int> new_stride;
    new_stride.resize(ndim_);
    auto factor = 1;
    for (int i = ndim_ - 1; i >= 0; i--) {
      factor *= new_shape[i];
      new_stride[i] = factor / new_shape[i];
    }

    // do transpose
    vector<int> rlt;
    rlt.resize(elem_num_);
    for (auto i = 0; i < elem_num_; i++) {
      auto new_coord = addr_to_coord(new_stride, i);
      vector<int> old_coord(ndim_, 0);
      for (auto j = 0; j < ndim_; j++) {
        old_coord[dim_order[j]] = new_coord[j];
      }

      auto old_pos = coord_to_addr(stride_, old_coord);
      rlt[i] = data_[old_pos];
    }

    // update data
    shape_ = new_shape;
    stride_ = new_stride;
    data_ = rlt;
  }

private:
  int coord_to_addr(const vector<int>& stride, const vector<int>& coord) const {
    int pos = 0;
    for(auto i=0U; i<coord.size(); i++) {
      pos += coord[i] * stride[i];
    }
    return pos;
  }

  vector<int> addr_to_coord(const vector<int>& stride, int pos) const {
    auto remainder = pos;
    vector<int> coord(stride.size(), 0);
    for(int i=stride.size()-1; i>=0; i--) {
      if(remainder == 0) break;
      coord[i] = remainder / stride[i];
      remainder = remainder % stride[i];
    }
    return coord;
  }

private:
 int ndim_;
 int elem_num_;
 vector<int> shape_;
 vector<int> stride_;
 vector<int> data_;
};
