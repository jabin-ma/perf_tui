//
// Created by Jabin Ma on 2022/11/28.
//

#ifndef PERF_TUI_CPU_H
#define PERF_TUI_CPU_H
#include <cstdlib>
#include <vector>
namespace sysmonitor {

  class Core {
private:
    bool online;
  };

  class Cluster {
public:
    std::vector<Core> cores;
    std::vector<uint32_t> freq;
  };

  class Cpu {

public:
    Cpu();
    std::vector<Cluster> clusters;
  };

}// namespace sysmonitor
#endif//PERF_TUI_CPU_H
