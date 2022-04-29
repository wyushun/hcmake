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
  string desp;
};

// clang-format off
const static RegInfo regs[(int)Reg::NUM] = {
  {.reg=Reg::C_DATE, .name="C_DATE", .addr=0x000, .desp=""},
  {.reg=Reg::C_VER, .name="C_VER", .addr=0x004, .desp=""},
  {.reg=Reg::START, .name="start", .addr=0x008, .desp=""},
  {.reg=Reg::SOFT_RESET, .name="soft_reset", .addr=0x00C, .desp=""},
  {.reg=Reg::INT, .name="int", .addr=0x010, .desp=""},
  {.reg=Reg::INT_MASK, .name="int_mask", .addr=0x014, .desp=""},
  {.reg=Reg::INSTR_BASE_ADDR_L, .name="instr_base_addr_l", .addr=0x018, .desp=""},
  {.reg=Reg::INSTR_BASE_ADDR_H, .name="instr_base_addr_h", .addr=0x01C, .desp=""},
  {.reg=Reg::IMG_BASE_ADDR_L, .name="img_base_addr_l", .addr=0x020, .desp=""},
  {.reg=Reg::IMG_BASE_ADDR_H, .name="img_base_addr_h", .addr=0x024, .desp=""},
  {.reg=Reg::WGT_BASE_ADDR_L, .name="wgt_base_addr_l", .addr=0x028, .desp=""},
  {.reg=Reg::WGT_BASE_ADDR_H, .name="wgt_base_addr_h", .addr=0x02C, .desp=""},
  {.reg=Reg::DM_AXI_CONFIG, .name="dm_axi_config", .addr=0x030, .desp=""},
  {.reg=Reg::INSTR_AXI_CONFIG, .name="instr_axi_config", .addr=0x034, .desp=""},
  {.reg=Reg::AXI_TRANS_DONE, .name="axi_trans_done", .addr=0x038, .desp=""},
  {.reg=Reg::ERR_CODE, .name="err_code", .addr=0x400, .desp=""},
  {.reg=Reg::INSTR_STAT_CODE, .name="instr_stat_code", .addr=0x404, .desp=""},
  {.reg=Reg::SYSTEM_STAT, .name="system_stat", .addr=0x408, .desp=""},
  {.reg=Reg::LOAD_STAT, .name="load_stat", .addr=0x40C, .desp=""},
  {.reg=Reg::SAVE_STAT, .name="save_stat", .addr=0x410, .desp=""},
  {.reg=Reg::CONV_STAT, .name="conv_stat", .addr=0x414, .desp=""},
  {.reg=Reg::MISC_STAT, .name="misc_stat", .addr=0x418, .desp=""},
  {.reg=Reg::PROFILE_SYST_ADDR_BASE_L, .name="profile_syst_addr_base_l", .addr=0xC00, .desp=""},
  {.reg=Reg::PROFILE_SYST_ADDR_BASE_H, .name="profile_syst_addr_base_h", .addr=0xC04, .desp=""},
  {.reg=Reg::PROFILE_LOAD_ADDR_BASE_L, .name="profile_load_addr_base_l", .addr=0xC08, .desp=""},
  {.reg=Reg::PROFILE_LOAD_ADDR_BASE_H, .name="profile_load_addr_base_h", .addr=0xC0C, .desp=""},
  {.reg=Reg::PROFILE_SAVE_ADDR_BASE_L, .name="profile_save_addr_base_l", .addr=0xC10, .desp=""},
  {.reg=Reg::PROFILE_SAVE_ADDR_BASE_H, .name="profile_save_addr_base_h", .addr=0xC14, .desp=""},
  {.reg=Reg::PROFILE_CONV_ADDR_BASE_L, .name="profile_conv_addr_base_l", .addr=0xC18, .desp=""},
  {.reg=Reg::PROFILE_CONV_ADDR_BASE_H, .name="profile_conv_addr_base_h", .addr=0xC1C, .desp=""},
  {.reg=Reg::PROFILE_MISC_ADDR_BASE_L, .name="profile_misc_addr_base_l", .addr=0xC20, .desp=""},
  {.reg=Reg::PROFILE_MISC_ADDR_BASE_H, .name="profile_misc_addr_base_h", .addr=0xC24, .desp=""},
  {.reg=Reg::PROFILE_SYST_INSTR_CNT, .name="profile_syst_instr_cnt", .addr=0xC28, .desp=""},
  {.reg=Reg::PROFILE_LOAD_INSTR_CNT, .name="profile_load_instr_cnt", .addr=0xC2C, .desp=""},
  {.reg=Reg::PROFILE_SAVE_INSTR_CNT, .name="profile_save_instr_cnt", .addr=0xC30, .desp=""},
  {.reg=Reg::PROFILE_CONV_INSTR_CNT, .name="profile_conv_instr_cnt", .addr=0xC34, .desp=""},
  {.reg=Reg::PROFILE_MISC_INSTR_CNT, .name="profile_misc_instr_cnt", .addr=0xC38, .desp=""},
  {.reg=Reg::LOAD_CNT, .name="load_cnt", .addr=0xC40, .desp=""},
  {.reg=Reg::SAVE_CNT, .name="save_cnt", .addr=0xC44, .desp=""},
  {.reg=Reg::PROFILE_ERR_CODE, .name="profile_err_code", .addr=0xC80, .desp=""},
};
// clang-format on

int main(int argc, char** argv) {
  cout << "Executing npu_aol_attach......\n";
  auto* handle = npu_aol_attach(0, NPU_SCHEDULE_MODE_SINGLE);
  if (!handle) {
    cerr << "npu_aol_attach error!\n";
    return 1;
  }
  cout << "npu_aol_attach success: "
       << "\n\taol version: 0x" << std::hex << handle->aol_version
       << "\n\tcore count: " << (int)handle->core_count
       << "\n\tcore phy addr: 0x" << std::hex << handle->core_phy_addr << "\n";

  cout << "regs info:\n";
  cout << "-----------------------------------\n";
  for (auto i = 0; i < (int)Reg::NUM; i++) {
    uint32_t value;
    auto ret =
        npu_aol_read_regs(handle, regs[i].addr, &value, sizeof(uint32_t));
    if (ret != NPU_AOL_OK) {
      cerr << "npu_aol_read_regs error when reading " << regs[i].name << "\n";
      return 1;
    }
    cout << "reg: " << regs[i].name << ", phy addr: " << std::hex
         << regs[i].addr << ", read value: " << std::hex << value << "\n";
  }

  cout << "Executing npu_aol_detach......\n";
  auto ret = npu_aol_detach(handle);
  if (ret != NPU_AOL_OK) {
    cerr << "npu_aol_detach error!\n";
    return 1;
  }
  cout << "npu_aol_detach success!\n";
  return 0;
}
