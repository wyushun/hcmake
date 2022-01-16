#pragma once

#include <iostream>
#include <atomic>

class RefCount {
protected:
  std::atomic<int> cnt_;

protected:
  RefCount() noexcept : cnt_(0) {}
  ~RefCount() = default;

  int increase() { return ++cnt_; }
  int decrease() { return --cnt_; }
  bool is_zero() const { return cnt_ == 0; }
};

template<typename T>
class IntrusivePtr {
private:
  T* ptr_;

public:
  intrusive_ptr() = default;
  intrusive_ptr(T* ptr) : ptr_(ptr) {}
  intrusive_ptr(const intrusive_ptr<T>& other) : ptr_(other.ptr) {
    ptr_->increase();
  }
  intrusive_ptr(intrusive_ptr<T>&& other) : ptr_(other.ptr) {
    other.ptr = nullptr;
  }
  intrusive_ptr<T>& operator=(const intrusive_ptr<T>& other) : ptr_(other.ptr) {
    ptr_->increase();
    return *this;
  }
  intrusive_ptr(intrusive_ptr<T>&& other) : ptr_(other.ptr) {
    other.ptr = nullptr;
  }

};
