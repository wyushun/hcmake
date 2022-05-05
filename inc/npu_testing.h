#pragma once

#include "npu_aol.h"
#include "std_inc.h"

enum class Reg {
  C_DATE,
  C_VER,
  START,
  SOFT_RESET,
  INT,
  INT_MASK,
  INSTR_BASE_ADDR_L,
  INSTR_BASE_ADDR_H,
  IMG_BASE_ADDR_L,
  IMG_BASE_ADDR_H,
  WGT_BASE_ADDR_L,
  WGT_BASE_ADDR_H,
  DM_AXI_CONFIG,
  INSTR_AXI_CONFIG,
  AXI_TRANS_DONE,
  ERR_CODE,
  INSTR_STAT_CODE,
  SYSTEM_STAT,
  LOAD_STAT,
  SAVE_STAT,
  CONV_STAT,
  MISC_STAT,
  PROFILE_SYST_ADDR_BASE_L,
  PROFILE_SYST_ADDR_BASE_H,
  PROFILE_LOAD_ADDR_BASE_L,
  PROFILE_LOAD_ADDR_BASE_H,
  PROFILE_SAVE_ADDR_BASE_L,
  PROFILE_SAVE_ADDR_BASE_H,
  PROFILE_CONV_ADDR_BASE_L,
  PROFILE_CONV_ADDR_BASE_H,
  PROFILE_MISC_ADDR_BASE_L,
  PROFILE_MISC_ADDR_BASE_H,
  PROFILE_SYST_INSTR_CNT,
  PROFILE_LOAD_INSTR_CNT,
  PROFILE_SAVE_INSTR_CNT,
  PROFILE_CONV_INSTR_CNT,
  PROFILE_MISC_INSTR_CNT,
  LOAD_CNT,
  SAVE_CNT,
  PROFILE_ERR_CODE,
  NUM
};

struct RegInfo {
  Reg reg;
  string name;
  uint64_t addr;
  uint32_t value;
  string desp;
};

class NPUTesting {
 private:
  static array<RegInfo, (int)Reg::NUM> regs;
  const static std::array<std::pair<uint64_t, string>, 4> mem_magnitude;
  enum class TimeUnit {
    TIME_S,
    TIME_MS,
    TIME_US,
  };

 public:
  NPUTesting();
  ~NPUTesting();

 public:
  void read_regs() const;
  void alloc_mem() const;
  void calc_api_time() const;

 private:
  string show_size(uint64_t size) const;
  void time_begin(struct timeval &begin);
  unsigned long time_end(const struct timeval &begin,
                         TimeUnit type = TimeUnit::TIME_MS);

 private:
  npu_aol_dev_handle_t *dev_;
};
