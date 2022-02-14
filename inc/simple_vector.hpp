#include "std_inc.hpp"

template <typename T, typename Alloc = std::allocator<T>>
class SimpleVector {
 public:
  SimpleVector(const Alloc& alloc = Alloc()) : allocator_(alloc) {
    first_.ptr_ = nullptr;
    last_.ptr_ = nullptr;
    end_.ptr_ = nullptr;
  }

  void push_back(T&& val) {
    if (full()) resize();
    allocator_.construct(last_.ptr_, std::forward<T>(val));
    last_.ptr_++;
  }

  void pop_back() {
    if (empty()) return;
    last_.ptr_--;
    allocator_.destroy(last_.ptr_);
  }

  bool full() const { return last_.ptr_ == end_.ptr_; }
  bool empty() const { return first_.ptr_ == last_.ptr_; }
  uint32_t size() const { return last_.ptr_ - first_.ptr_; }
  T& operator[](int idx) {
    CHECK(idx < size());
    return first_.ptr_[idx];
  }
  const T& operator[](int idx) const {
    CHECK(idx < size());
    return first_.ptr_[idx];
  }

  class iterator {
   public:
    friend class SimpleVector;
    iterator(T* ptr = nullptr) : ptr_(ptr) {}
    iterator& operator++() {
      ptr_++;
      return *this;
    }
    iterator operator++(int) {
      iterator tmp(ptr_);
      ptr_++;
      return tmp;
    }
    bool operator!=(const iterator& it) { return ptr_ != it.ptr_; }
    T& operator*() { return *ptr_; }
    T* operator->() { return ptr_; }

   private:
    T* ptr_;
  };
  iterator begin() { return iterator(first_.ptr_); }
  iterator end() { return iterator(last_.ptr_); }

 private:
  void resize() {
    if (first_.ptr_ == nullptr) {
      first_.ptr_ = allocator_.allocate(1);
      last_.ptr_ = first_.ptr_;
      end_.ptr_ = first_.ptr_ + 1;
    } else {
      int size = last_.ptr_ - first_.ptr_;
      T* ptmp = allocator_.allocate(2 * size);
      for (int i = 0; i < size; ++i) {
        allocator_.construct(ptmp + i, first_.ptr_[i]);
        allocator_.destroy(first_.ptr_ + i);
      }
      allocator_.deallocate(first_.ptr_, size);
      first_.ptr_ = ptmp;
      last_.ptr_ = first_.ptr_ + size;
      end_.ptr_ = first_.ptr_ + 2 * size;
    }
  }

 private:
  iterator first_;
  iterator last_;
  iterator end_;
  Alloc allocator_;
};
