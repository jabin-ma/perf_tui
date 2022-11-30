#include "cpu.h"
#include "strings_utils.h"
#include <cinttypes>
#include <dirent.h>
#include <iostream>
#include <vector>
using namespace sysmonitor;
using namespace std;


Cpu::Cpu() {
  struct dirent **pDirent;
  const char cpu_base_path[] = "/sys/devices/system/cpu/cpufreq";
  int scan_count = scandir(
          cpu_base_path, &pDirent,
          [](auto dire) { return (int) StartsWith(dire->d_name, "policy"); },
          [](auto dire_a, auto dire_b) {
            uint32_t policyN1, policyN2;
            if (sscanf((*dire_a)->d_name, "policy%" SCNu32 "", &policyN1) != 1 ||
                sscanf((*dire_b)->d_name, "policy%" SCNu32 "", &policyN2) != 1) {
              return 0;
            }
            return (int) (policyN1 - policyN2);
          });

  if (scan_count == -1 || scan_count == 0) { /*error */
    cout << "ERROR!" << endl;
    return;
  }


  auto ptrStub = pDirent;
  vector<string> policyFileNames(scan_count);
  vector<vector<uint32_t>> clusterFreq;
  for (auto &item: policyFileNames) {
    item = (*(pDirent++))->d_name;
    //
    //    for (const auto &name: {"available"/*, "boost"*/}) {
    auto freqLevelPath = StringPrintf("%s/%s/scaling_available_frequencies", cpu_base_path, item.c_str());
    auto nums = readNumbersFromFile(freqLevelPath);
    if (!nums) {
      cout << "Not found: " << freqLevelPath << endl;
    } else {
      cout << "PUSH FREQS " << freqLevelPath <<endl;
      clusterFreq.emplace_back(*nums);
//      clusterFreq
    }
    free(*pDirent);//free *
  }
  free(ptrStub);//free **


  for (const auto &cluster: clusterFreq){
    cout << "CORE ------ BEGIN -------" <<endl;
    for (const auto &freq: cluster){
        cout << "FREQ:" <<freq <<endl;
    }
    cout << "CORE ------ END ------" <<endl;
  }
}
