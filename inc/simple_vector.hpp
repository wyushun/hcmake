#include "vector_iterator.hpp"

template <typename T, typename Alloc = std::allocator<T>>
class SimpleVector {
 public:
  SimpleVector(const Alloc& alloc = Alloc())
      : start_(nullptr),
        finish_(nullptr),
        end_of_storage_(nullptr),
        allocator_(alloc) {}
  ~SimpleVector() = default;

  void push_back(T&& value) {
    if (end_of_storage_ == nullptr || finish_ == end_of_storage_) {
      resize();
    }
    allocator_.construct(finish_, std::forward<T>(value));
    finish_++;
  }
  void pop_back() {
    if (empty()) return;
    finish_--;
    allocator_.destroy(finish_);
  }

  bool empty() const { return start_ == finish_; }
  int size() const { return finish_ - start_; }
  T& operator[](int idx) { return start_[idx]; }
  const T& operator[](int idx) const { return start_[idx]; }

  VectorIterator<T> begin() { return VectorIterator<T>(start_); }
  VectorIterator<T> end() { return VectorIterator<T>(finish_); }

 private:
  void resize() {
    if (start_ == nullptr) {
      start_ = allocator_.allocate(8);
      finish_ = start_;
      end_of_storage_ = start_ + 8;
    } else {
      auto n = size();

      T* ptmp = allocator_.allocate(2 * n);
      for (auto i = 0; i < n; ++i) {
        allocator_.construct(ptmp + i, start_[i]);
        allocator_.destroy(start_ + i);
      }
      allocator_.deallocate(start_, n);

      start_ = ptmp;
      finish_ = ptmp + n;
      end_of_storage_ = ptmp + 2 * n;
    }
  }

 private:
  T* start_;
  T* finish_;
  T* end_of_storage_;

  Alloc allocator_;
};
