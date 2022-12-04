//
// Created by Jabin Ma on 2022/11/28.
//

#ifndef PERF_TUI_CPU_H
#define PERF_TUI_CPU_H
#include <vector>
#include <string>
namespace sysmonitor {
  static const std::string cpu_base_path = "/sys/devices/system/cpu/cpufreq";

  class Core {
private:
    bool online;
  };

  class Cluster {
public:
    std::string name;
    std::vector<uint32_t> cores;
    std::vector<uint32_t> avl_freq;

    void updateMateData();

  };

  class Cpu {
private:
    void init();

public:
    Cpu(){ init(); }
    std::vector<Cluster> clusters;
  };

}// namespace sysmonitor
#endif//PERF_TUI_CPU_H
