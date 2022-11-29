//
// Created by Jabin Ma on 2022/11/28.
//

#include "cpu.h"
#include "strings_utils.h"
#include <iostream>
#include <vector>
using namespace sysmonitor;
using namespace std;
Cpu::Cpu() {
  struct dirent **dirlist;
  const char cpu_base_path[] = "/sys/devices/system/cpu/cpufreq";
  int ret = scandir(
          cpu_base_path, &dirlist,
          [](auto dire) { return (int) StartsWith(dire->d_name, "policy"); },
          nullptr);
  if (ret == -1) { /*error */
    cout << "ERROR!" << endl;
  }
  cout << "CLUSTER >>" << ret << endl;
  uint32_t cluster_count = ret;
  vector<string> policyFileNames;
  for (uint32_t i = 0; i < cluster_count; ++i) {
    policyFileNames.emplace_back(dirlist[i]->d_name);
    free(dirlist[i]);
  }
  free(dirlist);

  for (const auto &policy: policyFileNames) {
    //    std::cout << policy << std::endl;
    for (const auto &name: {"available", "boost"}) {
      auto freq_level_path = StringPrintf("%s/%s/scaling_%s_frequencies", cpu_base_path, policy.c_str(), name);
      //        cout << path <<endl;
    }
  }
}
