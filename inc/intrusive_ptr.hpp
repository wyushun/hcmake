#pragma once

#include <iostream>
#include <atomic>

class RefCount {
 protected:
  std::atomic<int> cnt_;

 public:
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
  IntrusivePtr() noexcept : ptr_(nullptr) {}
  IntrusivePtr(T* ptr) noexcept : ptr_(ptr) { ptr_->increase(); }
  IntrusivePtr(const IntrusivePtr<T>& other) noexcept : ptr_(other.ptr_) {
    ptr_->increase();
  }
  IntrusivePtr(IntrusivePtr<T>&& other) noexcept : ptr_(other.ptr_) {
    other.ptr_ = nullptr;
  }
  IntrusivePtr<T>& operator=(const IntrusivePtr<T>& other) noexcept {
    if (ptr_ == other.ptr_) return *this;
    ptr_->increase();
    return *this;
  }
  IntrusivePtr<T>& operator=(IntrusivePtr<T>&& other) noexcept {
    std::swap(ptr_, other.ptr_);
    return *this;
  }
  ~IntrusivePtr() {
    if (ptr_) {
      ptr_->decrease();
      if (ptr_->is_zero()) {
        delete ptr_;
        ptr_ = nullptr;
      }
    }
  }

  T* get() const { return ptr_; }
  T* operator->() const { return ptr_; }
  T& operator*() const { return *ptr_; }
};

