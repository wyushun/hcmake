#include "npu_aol.h"
#include "std_inc.h"

int main(int argc, char** argv) {
  cout << "Executing npu_aol_attach......" << endl;
  auto* handle = npu_aol_attach(0, NPU_SCHEDULE_MODE_SINGLE);
  if (!handle) {
    cerr << "npu_aol_attach error!" << endl;
    return 1;
  }
  cout << "npu_aol_attach success: \n"
       << "\taol version: 0x" << std::hex << handle->aol_version
       << "\tcore count: 0x" << std::hex << handle->core_count
       << "\tcore phy addr: 0x" << std::hex << handle->core_phy_addr;

  cout << "Executing npu_aol_detach......" << endl;
  auto ret = npu_aol_detach(handle);
  if (ret != NPU_AOL_OK) {
    cerr << "npu_aol_detach error!" << endl;
    return 1;
  }
  cout << "npu_aol_detach success!\n";
  return 0;
}
