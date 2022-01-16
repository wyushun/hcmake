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

template <typename T>
class intrusive_ptr {
 private:
  T* ptr_;

 public:
  intrusive_ptr() noexcept : ptr_(nullptr) {}
  intrusive_ptr(T* ptr) noexcept : ptr_(ptr) { ptr_->increase(); }
  intrusive_ptr(const intrusive_ptr<T>& other) noexcept : ptr_(other.ptr_) {
    ptr_->increase();
  }
  intrusive_ptr(intrusive_ptr<T>&& other) noexcept : ptr_(other.ptr_) {
    other.ptr_ = nullptr;
  }
  intrusive_ptr<T>& operator=(const intrusive_ptr<T>& other) noexcept {
    if (ptr_ == other.ptr_) return *this;
    ptr_->increase();
    return *this;
  }
  intrusive_ptr<T>& operator=(intrusive_ptr<T>&& other) noexcept {
    std::swap(ptr_, other.ptr_);
    return *this;
  }
  ~intrusive_ptr() {
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

