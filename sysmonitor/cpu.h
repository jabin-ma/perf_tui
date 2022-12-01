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
    std::vector<uint32_t> freq;
    void update();
  };

  class Cpu {
public:
    Cpu();
    std::vector<Cluster> clusters;
  };

  struct myCluset{
    std::string name;
    std::string cores;
    std::vector<uint32_t> freq;
  };

}// namespace sysmonitor
#endif//PERF_TUI_CPU_H
