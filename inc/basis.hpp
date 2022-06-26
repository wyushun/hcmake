#pragma once

#include <dirent.h>
#include <fcntl.h>
#include <getopt.h>
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
using std::istream;
using std::ofstream;
using std::ostream;
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

#include <glog/logging.h>

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
using std::nullopt;
using std::optional;
#else
#include "optional.hpp"
using tl::nullopt;
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

// calculate spent time
enum class TimeUnit { S, MS, US };

void timeBegin(struct timeval &begin);
unsigned long timeEnd(const struct timeval &begin,
                      TimeUnit type = TimeUnit::MS);

// show memory size in human readable form
enum class MemUnit { GB, MB, KB, B, NUM };
string showMemSize(uint64_t size);

// file operation related funcs
template <typename T>
void chkOpen(T &f, const string &fname) {
  struct stat buf;

  if (lstat(fname.c_str(), &buf) == -1) {
    cerr << fname << " stat error: " << strerror(errno) << endl;
    abort();
  }

  // handle common file
  if (!S_ISREG(buf.st_mode)) {
    cerr << "Not a normal file: " << fname << endl;
    abort();
  }

  if (f.fail()) {
    cerr << "open file fail(): " << fname << endl;
    abort();
  }

  if (f.bad()) {
    cerr << "open file bad(): " << fname << endl;
    abort();
  }

  if (f.eof()) {
    cerr << "open file eof(): " << fname << endl;
    abort();
  }
}
uint64_t getFileSize(const string &fname);
enum class SaveMode { TRUNC, APPEND };
uint64_t loadBin(const string &fname, char *data, uint64_t size);
void saveBin(const string &fname, const char *data, uint64_t size,
             SaveMode mode = SaveMode::TRUNC);
void delFileOrDir(const string &name);
bool chkFile(const string &name, bool del_exists = false);
bool chkFolder(const string &name, bool del_exists = false);

// string truncation and split funcs
string trim(const string &str);
vector<string> split(const string &str, const string &delim = " ,:;\t",
                     bool trim_flag = true);

// math related
float sigmoid(float p);
