#ifndef __MISC_UTIL_DEF_HPP__
#define __MISC_UTIL_DEF_HPP__

#include "std_inc.hpp"

namespace util {

template <typename T,
          typename std::enable_if<std::is_integral<T>::value, bool>::type>
T Random(T low, T high, uint64_t seed) {
  std::random_device rd;
  std::mt19937 gen(rd());
  gen.seed(seed);
  std::uniform_int_distribution<T> dis(low, high);

  return dis(gen);
}

template <typename T,
          typename std::enable_if<std::is_floating_point<T>::value, bool>::type>
T Random(T low, T high, uint64_t seed) {
  std::random_device rd;
  std::mt19937 gen(rd());
  gen.seed(seed);
  std::uniform_real_distribution<T> dis(low, high);

  return dis(gen);
}

template <typename T,
          typename std::enable_if<std::is_integral<T>::value, bool>::type>
void Random(T* data, uint64_t size, T low, T high, uint64_t seed) {
  std::random_device rd;
  std::mt19937 gen(rd());
  gen.seed(seed);
  std::uniform_int_distribution<T> dis(low, high);

  for (auto i = 0U; i < size; i++) {
    if (seed == 0)
      data[i] = 0;
    else
      data[i] = dis(gen);
  }
}

template <typename T,
          typename std::enable_if<std::is_floating_point<T>::value, bool>::type>
void Random(T* data, uint64_t size, T low, T high, uint64_t seed) {
  std::random_device rd;
  std::mt19937 gen(rd());
  gen.seed(seed);
  std::uniform_real_distribution<T> dis(low, high);

  for (auto i = 0U; i < size; i++) {
    if (seed == 0)
      data[i] = 0;
    else
      data[i] = dis(gen);
  }
}

} /* namespace util */

#endif /* __MISC_UTIL_DEF_HPP__*/
