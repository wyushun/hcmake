/*
 * Copyright 2022 Novauto Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "basis.hpp"

namespace {
// clang-format off
const static array<uint64_t, E2I(MemUnit::NUM)> memUnitSize = {
  1 * 1024 * 1024 * 1024,
  1 * 1024 * 1024,
  1 * 1024,
  1,
};

const static array<string, E2I(MemUnit::NUM)> memUnitName {
  "G",
  "M",
  "K",
  "B",
};
// clang-format on
}  // namespace

string showMemSize(uint64_t size) {
  if (size == 0) return "0B";

  std::ostringstream oss;

  for (auto i = 0; i < E2I(MemUnit::NUM); i++) {
    auto n = size / memUnitSize[i];
    size %= memUnitSize[i];
    if (n) {
      oss << n << memUnitName[i];
    }
  }

  return oss.str();
}

void timeBegin(struct timeval &begin) { gettimeofday(&begin, 0); }

unsigned long timeEnd(const struct timeval &begin, TimeUnit type) {
  struct timeval end;
  gettimeofday(&end, 0);
  if (type == TimeUnit::S) {
    return (1000000 * (end.tv_sec - begin.tv_sec) +
            (end.tv_usec - begin.tv_usec)) /
           1000000;
  } else if (type == TimeUnit::MS) {
    return (1000000 * (end.tv_sec - begin.tv_sec) +
            (end.tv_usec - begin.tv_usec)) /
           1000;
  } else {
    return (1000000 * (end.tv_sec - begin.tv_sec) +
            (end.tv_usec - begin.tv_usec)) /
           1;
  }
}

uint64_t getFileSize(const string &fname) {
  struct stat buf;
  if (stat(fname.c_str(), &buf) == -1) {
    cerr << fname << " stat error: " << strerror(errno) << endl;
    abort();
  }

  return buf.st_size;
}

uint64_t loadBin(const string &fname, char *data, uint64_t size) {
  // check memory size's validation
  auto fsize = getFileSize(fname);
  if (size < fsize) {
    cerr << "Buffer size (" << size << ") is less than file size(" << fsize
         << ")!" << endl;
    abort();
  }

  // get file length, allocate data buf
  std::fstream f(fname, std::ios_base::in | std::ios_base::binary);
  chkOpen(f, fname);

  // read file contents
  f.read(data, size);
  f.close();

  return fsize;
}

void saveBin(const string &fname, const char *data, uint64_t size,
             SaveMode mode) {
  // write into file
  std::ofstream f;
  if (mode == SaveMode::TRUNC) {
    f.open(fname, std::ios::trunc | std::ios::binary | std::ios::out);
  } else {
    f.open(fname, std::ios::app | std::ios::binary | std::ios::out);
  }

  chkOpen(f, fname);

  f.write(data, size);
  f.close();
}
void delFileOrDir(const string &name) {
  struct stat buf;
  const char *fname = name.c_str();

  if (lstat(fname, &buf) == -1) {
    cout << "stat result: " << strerror(errno) << "(" << fname << ")" << endl;
    return;
  }

  // handle symbol link
  if (S_ISLNK(buf.st_mode)) {
    unlink(fname);
    return;
  }

  // handle common file
  if (S_ISREG(buf.st_mode)) {
    remove(fname);
    return;
  }

  // handle not directory
  if (!S_ISDIR(buf.st_mode)) {
    remove(fname);
    return;
  }

  // handle directory
  DIR *dir = NULL;
  char next_fname[1024];
  struct dirent *ptr;

  // open directory
  if ((dir = opendir(fname)) == NULL) {
    cerr << fname << " opendir error: " << strerror(errno) << endl;
    abort();
  }

  while ((ptr = readdir(dir))) {
    if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
      continue;

    snprintf(next_fname, 1024, "%s/%s", fname, ptr->d_name);
    delFileOrDir(string(next_fname));
  }

  if (closedir(dir) == -1) {
    cerr << fname << " closedir error: " << strerror(errno) << endl;
    abort();
  }

  if (rmdir(fname) == -1) {
    cerr << fname << " rmdir error: " << strerror(errno) << endl;
    abort();
  }
}

bool chkFile(const string &name, bool del_exists) {
  bool found = (access(name.c_str(), R_OK) == 0);

  if (!del_exists || !found) return found;

  // file exists, return
  if (access(name.c_str(), R_OK | W_OK) == 0) {
    delFileOrDir(name);
  } else {
    cerr << "No priviledge to delete " << name << endl;
    abort();
  }

  return found;
}

bool chkFolder(const string &name, bool del_exists) {
  bool found = (access(name.c_str(), R_OK) == 0);

  if (found) {
    if (del_exists) {
      if (access(name.c_str(), R_OK | W_OK | X_OK) == 0) {
        delFileOrDir(name);
      } else {
        cerr << "No priviledge to delete " << name << endl;
        abort();
      }
    } else {
      return found;
    }
  }

  // folder doesn't exist, create it
  if (mkdir(name.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
    cerr << "mkdir error: " << name << endl;
    abort();
  }
  // cout << "Create folder: " << name << endl;

  return found;
}

string trim(const string &str) {
  auto tmp = str;

  // erase whitespace before the string
  string::iterator it1;
  for (it1 = tmp.begin(); it1 < tmp.end(); it1++) {
    if (!isspace(*it1)) break;
  }
  tmp.erase(0, it1 - tmp.begin());

  // erase whitespace after the string
  string::reverse_iterator it2;
  for (it2 = tmp.rbegin(); it2 < tmp.rend(); it2++) {
    if (!isspace(*it2)) break;
  }
  tmp.erase(tmp.rend() - it2, it2 - tmp.rbegin());

  return tmp;
}

// split all words in the string
vector<string> split(const string &s, const string &delim, bool if_trim) {
  string::size_type begin = 0;
  string::size_type addr = 0;
  vector<string> r;
  while (1) {
    addr = s.find_first_of(delim, begin);

    auto len = (addr == string::npos) ? (s.size() - begin) : (addr - begin);
    if (len) {
      auto tmp = if_trim ? trim(s.substr(begin, len)) : s.substr(begin, len);
      if (!tmp.empty()) r.emplace_back(tmp);
    }
    begin = addr + 1;
    if (addr == string::npos || begin >= s.size()) break;
  }

  return r;
}

float sigmoid(float p) { return 1.0 / (1 + exp(-p * 1.0)); }