#ifndef GPUINFO_H
#define GPUINFO_H

#include <string>

struct GpuInfo {
    std::string name;
    uint32_t coreCount;
    bool isVrReady;
    uint32_t powerUtilization;
    uint64_t memorySizeMB;
    uint32_t gpuClockKHz;
    uint32_t vramClockKHz;
};

GpuInfo GetGpuInfo();

#endif
