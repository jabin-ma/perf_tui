//
// Created by Jabin Ma on 2022/11/28.
//

#ifndef PERF_TUI_CPU_H
#define PERF_TUI_CPU_H
#include "dirent.h"
#include <cstdlib>
class Core{

};

class Cluster{

};

class Cpu{

  public:
      Cpu(){
        struct dirent **dirlist;
        const char basepath[] = "/sys/devices/system/cpu/cpufreq";
        int ret = scandir(
                basepath, &dirlist ,[](auto a){return 0;},[](auto a,auto b){return 0;});
        if (ret == -1) {
           cluster_count = ret;
        }

        free(dirlist);
      }
      int cluster_count;
};


#endif//PERF_TUI_CPU_H
