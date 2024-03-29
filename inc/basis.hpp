#pragma once

#include <dirent.h>
#include <fcntl.h>
#include <getopt.h>
#include <glog/logging.h>
#include <pwd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <bitset>
#include <cassert>
#include <cfenv>
#include <climits>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <future>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <random>
#include <regex>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;

using std::array;
using std::bitset;
using std::deque;
using std::list;
using std::map;
using std::pair;
using std::regex;
using std::set;
using std::string;
using std::tuple;
using std::unordered_map;
using std::vector;

using std::make_shared;
using std::move;
using std::shared_ptr;
using std::unique_ptr;

using std::back_inserter;
using std::front_inserter;

using std::fstream;
using std::ifstream;
using std::ofstream;
using std::stringstream;

using std::async;
using std::function;
using std::mutex;
using std::thread;

using std::stod;
using std::stof;
using std::stoi;
using std::stol;
using std::stoul;
using std::to_string;

using std::dec;
using std::hex;
using std::left;
using std::oct;
using std::setfill;
using std::setw;

#if __cplusplus >= 201103L
template <typename... Args>
void unused(Args &&...args) {
  (void)(sizeof...(args));
}
#endif

#if __cplusplus >= 201402L
using std::make_unique;
#else
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args &&...args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif

#if __cplusplus >= 201703L
#include <any>
using std::any;
#else
#include "any.hpp"
using open_source::any;
#endif

#if __cplusplus >= 201703L
#include <optional>
using std::optional;
#else
#include "optional.hpp"
using tl::optional;
#endif

template <typename E>
constexpr typename std::underlying_type<E>::type E2I(E e) noexcept {
  return static_cast<typename std::underlying_type<E>::type>(e);
}

#ifdef __GNUC__
#define UNUSED(x) UNUSED_##x __attribute__((__unused__))
#else
#define UNUSED(x) UNUSED_##x
#endif

#ifdef __GNUC__
#define UNUSED_FUNCTION(x) __attribute__((__unused__)) UNUSED_##x
#else
#define UNUSED_FUNCTION(x) UNUSED_##x
#endif

enum class TimeUnit {
  TIME_S,
  TIME_MS,
  TIME_US,
};

void timeBegin(struct timeval &begin);
unsigned long timeEnd(const struct timeval &begin,
                      TimeUnit type = TimeUnit::TIME_MS);

enum class MemUnit { GB, MB, KB, B, NUM };
string showMemSize(uint64_t size);
