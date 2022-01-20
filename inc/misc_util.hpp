#ifndef __MISC_UTIL_HPP__
#define __MISC_UTIL_HPP__

#include "std_inc.hpp"

namespace util {

// used to calculate time
enum TimeTypeEnum : int {
  TIME_S,
  TIME_MS,
  TIME_US,
};
void TimeBegin(struct timeval &begin);
unsigned long TimeEnd(const struct timeval &begin, int type = TIME_MS);

// misc
template <typename T, typename std::enable_if<std::is_integral<T>::value,
                                              bool>::type = true>
T Random(T low, T high, uint64_t seed = 1024);
template <typename T, typename std::enable_if<std::is_floating_point<T>::value,
                                              bool>::type = true>
T Random(T low, T high, uint64_t seed = 1024);
template <typename T, typename std::enable_if<std::is_integral<T>::value,
                                              bool>::type = true>
void Random(T *data, uint64_t size, T low, T high, uint64_t seed);
template <typename T, typename std::enable_if<std::is_floating_point<T>::value,
                                              bool>::type = true>
void Random(T *data, uint64_t size, T low, T high, uint64_t seed);

uint32_t FillMaskU32(int start, int num);
uint64_t FillMaskU64(int start, int num);

uint64_t AlignByN(uint64_t data, uint64_t align);

} /* namespace util */

#include "misc_util_def.hpp"

#endif /* __MISC_UTIL_HPP__*/
