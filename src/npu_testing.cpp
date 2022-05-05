#include "npu_testing.h"

// clang-format off
array<RegInfo, (int)Reg::NUM> NPUTesting::regs = {{
  {.reg=Reg::C_DATE, .name="C_DATE", .addr=0x000, .desp="", .value=0},
  {.reg=Reg::C_VER, .name="C_VER", .addr=0x004, .desp="", .value=0},
  {.reg=Reg::START, .name="start", .addr=0x008, .desp="", .value=0},
  {.reg=Reg::SOFT_RESET, .name="soft_reset", .addr=0x00C, .desp="", .value=0},
  {.reg=Reg::INT, .name="int", .addr=0x010, .desp="", .value=0},
  {.reg=Reg::INT_MASK, .name="int_mask", .addr=0x014, .desp="", .value=0},
  {.reg=Reg::INSTR_BASE_ADDR_L, .name="instr_base_addr_l", .addr=0x018, .desp="", .value=0},
  {.reg=Reg::INSTR_BASE_ADDR_H, .name="instr_base_addr_h", .addr=0x01C, .desp="", .value=0},
  {.reg=Reg::IMG_BASE_ADDR_L, .name="img_base_addr_l", .addr=0x020, .desp="", .value=0},
  {.reg=Reg::IMG_BASE_ADDR_H, .name="img_base_addr_h", .addr=0x024, .desp="", .value=0},
  {.reg=Reg::WGT_BASE_ADDR_L, .name="wgt_base_addr_l", .addr=0x028, .desp="", .value=0},
  {.reg=Reg::WGT_BASE_ADDR_H, .name="wgt_base_addr_h", .addr=0x02C, .desp="", .value=0},
  {.reg=Reg::DM_AXI_CONFIG, .name="dm_axi_config", .addr=0x030, .desp="", .value=0},
  {.reg=Reg::INSTR_AXI_CONFIG, .name="instr_axi_config", .addr=0x034, .desp="", .value=0},
  {.reg=Reg::AXI_TRANS_DONE, .name="axi_trans_done", .addr=0x038, .desp="", .value=0},
  {.reg=Reg::ERR_CODE, .name="err_code", .addr=0x400, .desp="", .value=0},
  {.reg=Reg::INSTR_STAT_CODE, .name="instr_stat_code", .addr=0x404, .desp="", .value=0},
  {.reg=Reg::SYSTEM_STAT, .name="system_stat", .addr=0x408, .desp="", .value=0},
  {.reg=Reg::LOAD_STAT, .name="load_stat", .addr=0x40C, .desp="", .value=0},
  {.reg=Reg::SAVE_STAT, .name="save_stat", .addr=0x410, .desp="", .value=0},
  {.reg=Reg::CONV_STAT, .name="conv_stat", .addr=0x414, .desp="", .value=0},
  {.reg=Reg::MISC_STAT, .name="misc_stat", .addr=0x418, .desp="", .value=0},
  {.reg=Reg::PROFILE_SYST_ADDR_BASE_L, .name="profile_syst_addr_base_l", .addr=0xC00, .desp="", .value=0},
  {.reg=Reg::PROFILE_SYST_ADDR_BASE_H, .name="profile_syst_addr_base_h", .addr=0xC04, .desp="", .value=0},
  {.reg=Reg::PROFILE_LOAD_ADDR_BASE_L, .name="profile_load_addr_base_l", .addr=0xC08, .desp="", .value=0},
  {.reg=Reg::PROFILE_LOAD_ADDR_BASE_H, .name="profile_load_addr_base_h", .addr=0xC0C, .desp="", .value=0},
  {.reg=Reg::PROFILE_SAVE_ADDR_BASE_L, .name="profile_save_addr_base_l", .addr=0xC10, .desp="", .value=0},
  {.reg=Reg::PROFILE_SAVE_ADDR_BASE_H, .name="profile_save_addr_base_h", .addr=0xC14, .desp="", .value=0},
  {.reg=Reg::PROFILE_CONV_ADDR_BASE_L, .name="profile_conv_addr_base_l", .addr=0xC18, .desp="", .value=0},
  {.reg=Reg::PROFILE_CONV_ADDR_BASE_H, .name="profile_conv_addr_base_h", .addr=0xC1C, .desp="", .value=0},
  {.reg=Reg::PROFILE_MISC_ADDR_BASE_L, .name="profile_misc_addr_base_l", .addr=0xC20, .desp="", .value=0},
  {.reg=Reg::PROFILE_MISC_ADDR_BASE_H, .name="profile_misc_addr_base_h", .addr=0xC24, .desp="", .value=0},
  {.reg=Reg::PROFILE_SYST_INSTR_CNT, .name="profile_syst_instr_cnt", .addr=0xC28, .desp="", .value=0},
  {.reg=Reg::PROFILE_LOAD_INSTR_CNT, .name="profile_load_instr_cnt", .addr=0xC2C, .desp="", .value=0},
  {.reg=Reg::PROFILE_SAVE_INSTR_CNT, .name="profile_save_instr_cnt", .addr=0xC30, .desp="", .value=0},
  {.reg=Reg::PROFILE_CONV_INSTR_CNT, .name="profile_conv_instr_cnt", .addr=0xC34, .desp="", .value=0},
  {.reg=Reg::PROFILE_MISC_INSTR_CNT, .name="profile_misc_instr_cnt", .addr=0xC38, .desp="", .value=0},
  {.reg=Reg::LOAD_CNT, .name="load_cnt", .addr=0xC40, .desp="", .value=0},
  {.reg=Reg::SAVE_CNT, .name="save_cnt", .addr=0xC44, .desp="", .value=0},
  {.reg=Reg::PROFILE_ERR_CODE, .name="profile_err_code", .addr=0xC80, .desp="", .value=0},
}};
// clang-format on

NPUTesting::NPUTesting() {
  cout << "Executing npu_aol_attach......\n";
  dev_ = npu_aol_attach(0, NPU_SCHEDULE_MODE_SINGLE);
  if (!dev_) {
    cerr << "npu_aol_attach error!\n";
    return;
  }

  cout << "npu_aol_attach success: "
       << "\n\taol version: 0x" << std::hex << dev_->aol_version
       << "\n\tcore count: " << (int)dev_->core_count << "\n\tcore phy addr: 0x"
       << std::hex << dev_->core_phy_addr << "\n";
}

NPUTesting::~NPUTesting() {
  cout << "Executing npu_aol_detach......\n";
  auto ret = npu_aol_detach(dev_);
  if (ret != NPU_AOL_OK) {
    cerr << "npu_aol_detach error!\n";
    return;
  }
  cout << "npu_aol_detach success!\n";
}

void NPUTesting::read_regs() {
  auto old_flags = cout.flags();
  cout << std::left << setw(32) << "reg name" << setw(16) << "reg addr"
       << setw(16) << "reg value" << endl;
  cout << "--------------------------------------------------------------\n";
  for (auto i = 0; i < (int)Reg::NUM; i++) {
    auto ret = npu_aol_read_regs(dev_, regs[i].addr + dev_->core_phy_addr,
                                 &regs[i].value, 4);
    if (ret != NPU_AOL_OK) {
      cerr << "npu_aol_read_regs error when reading " << regs[i].name << "\n";
      return;
    }
    cout << setw(32) << regs[i].name << setw(16) << std::hex
         << dev_->core_phy_addr + regs[i].addr << setw(16) << std::hex
         << regs[i].value << "\n";
  }
  cout.flags(old_flags);
}

void NPUTesting::alloc_mem() {
  auto old_flags = cout.flags();
  static uint64_t bytes = 1 * 1024 * 1024u;
  for (uint32_t i = 4;; i += 4) {
    cout << "Allocating NPU memory " << std::dec << i << "MB...\n";
    auto *mem = npu_aol_alloc_dev_mem(
        dev_, i * bytes, NPU_AOL_MEM_PROT_READ | NPU_AOL_MEM_PROT_WRITE);
    if (mem) {
      cout << "Mem size: " << show_size(mem->size) << ", phy addr: 0x"
           << std::hex << mem->addr_phy << ", vir addr: 0x" << std::hex
           << mem->addr_virt << "\n";
      auto ret = npu_aol_free_dev_mem(dev_, mem);
      if (ret != NPU_AOL_OK) {
        cerr << "npu_aol_free_dev_mem error!\n";
        break;
      }
    } else {
      cerr << "npu_aol_alloc_dev_mem failure!\n";
      break;
    }
  }
  cout.flags(old_flags);
}

string NPUTesting::show_size(uint64_t size) const {
  if (size == 0) return "0B";

  std::array<std::pair<uint64_t, string>, 4> mem_magnitude = {{
      {1 * 1024 * 1024 * 1024, "G"},
      {1 * 1024 * 1024, "M"},
      {1 * 1024, "K"},
      {1, "B"},
  }};
  std::ostringstream oss;

  for (auto i = 0u; i < mem_magnitude.size(); i++) {
    auto n = size / mem_magnitude[i].first;
    size %= mem_magnitude[i].first;
    if (n) {
      oss << n << mem_magnitude[i].second;
    }
  }

  return oss.str();
}
