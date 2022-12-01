#include "cpu.h"
#include "strings_utils.h"
#include <cinttypes>
#include <dirent.h>
#include <iostream>
using namespace sysmonitor;
using namespace std;

Cpu::Cpu() {
  struct dirent **pDirent;
  int scan_count = scandir(
          cpu_base_path.c_str(), &pDirent,
          [](auto dire) { return (int) StartsWith(dire->d_name, "policy"); },
          [](auto dire_a, auto dire_b) {
            uint32_t policyN1, policyN2;
            if (sscanf((*dire_a)->d_name, "policy%" SCNu32 "", &policyN1) != 1 ||
                sscanf((*dire_b)->d_name, "policy%" SCNu32 "", &policyN2) != 1) {
              return 0;
            }
            return (int) (policyN1 - policyN2);
          });

  if (scan_count <= 0) { /*error */
    cout << "ERROR!" << endl;
    return;
  }
  // 解析节点内容 (例如 policy0 policy3 policy7)
  auto const ptrStub = pDirent;
  clusters.resize(scan_count);
  for (auto &item: clusters) {
    // cluster begin
    item.name = (*(pDirent++))->d_name;
    item.update();
    free(*pDirent);//free **
  }
  free(ptrStub);//free *
}

bool readNumbersFromFileToVector(const string &file, vector<uint32_t> &des) {
  auto nums = readNumbersFromFile(file);
  if (nums) {
    des.insert(des.begin(), nums->begin(), nums->end());
  } else {
    return false;
  }
  return true;
}

void Cluster::update() {
  // 将节点里面的频率信息以及cpu集所对应的cpuid读到数组中
  readNumbersFromFileToVector(StringPrintf("%s/%s/%s", cpu_base_path.c_str(), name.c_str(), "scaling_available_frequencies"), freq);
  readNumbersFromFileToVector(StringPrintf("%s/%s/%s", cpu_base_path.c_str(), name.c_str(), "related_cpus"), cores);
}
