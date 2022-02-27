#pragma once

#include "std_inc.hpp"

template <typename T>
class VectorIterator {
 public:
  VectorIterator(T* ptr) : ptr_(ptr) {}

  VectorIterator& operator++() {
    ptr_++;
    return *this;
  }
  VectorIterator operator++(int) {
    VectorIterator tmp(ptr_);
    ptr_++;
    return tmp;
  }
  VectorIterator& operator--() {
    ptr_--;
    return *this;
  }
  VectorIterator operator--(int) {
    VectorIterator tmp(ptr_);
    ptr_--;
    return tmp;
  }
  bool operator==(const VectorIterator& it) { return ptr_ == it.ptr_; }
  bool operator!=(const VectorIterator& it) { return !operator==(it); }
  T& operator*() { return *ptr_; }
  T* operator->() { return *ptr_; }

 private:
  T* ptr_;
};

template <typename T>
using ConstVectorIterator = VectorIterator<const T>;
