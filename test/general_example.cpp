#include "npu_aol.h"
#include "std_inc.h"

int main(int argc, char** argv) {
  cout << "Executing npu_aol_attach......\n";
  auto* handle = npu_aol_attach(0, NPU_SCHEDULE_MODE_SINGLE);
  if (!handle) {
    cerr << "npu_aol_attach error!\n";
    return 1;
  }
  cout << "npu_aol_attach success: "
       << "\n\taol version: 0x" << std::hex << handle->aol_version
       << "\n\tcore count: 0x" << std::hex << handle->core_count
       << "\n\tcore phy addr: 0x" << std::hex << handle->core_phy_addr << "\n";

  cout << "Executing npu_aol_detach......\n";
  auto ret = npu_aol_detach(handle);
  if (ret != NPU_AOL_OK) {
    cerr << "npu_aol_detach error!\n";
    return 1;
  }
  cout << "npu_aol_detach success!\n";
  return 0;
}
