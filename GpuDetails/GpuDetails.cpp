#include <iostream>
#include <NvAPI.h>
#define GpuDetails _declspec(dllexport)

struct GpuInfo {
    char name[256];
    uint32_t coreCount;
    bool isVrReady;
    uint32_t powerUtilization;
    uint64_t memorySizeMB;
    uint32_t gpuClockKHz;
    uint32_t vramClockKHz;
};

extern "C" {
    GpuDetails GpuInfo GetGpuInfo() {
        NvU32 nGPU = 0;
        NvPhysicalGpuHandle hdlGPU[NVAPI_MAX_PHYSICAL_GPUS] = { 0 };
        NV_GPU_CLOCK_FREQUENCIES gpuClocks = { 0 };
        GpuInfo gpuInfo = {};

        if (NvAPI_Initialize() != NVAPI_OK) {
            return gpuInfo;
        }

        if (NvAPI_EnumPhysicalGPUs(hdlGPU, &nGPU) != NVAPI_OK || nGPU == 0) {
            NvAPI_Unload();
            return gpuInfo;
        }

        gpuClocks.version = NV_GPU_CLOCK_FREQUENCIES_VER;
        gpuClocks.ClockType = NV_GPU_CLOCK_FREQUENCIES_CURRENT_FREQ;

        if (NvAPI_GPU_GetAllClockFrequencies(hdlGPU[0], &gpuClocks) != NVAPI_OK) {
            NvAPI_Unload();
            return gpuInfo;
        }

        for (NvU32 i = 0; i < nGPU; ++i) {
            NvPhysicalGpuHandle gpuHandle = hdlGPU[i];

            NvAPI_ShortString gpuName;
            if (NvAPI_GPU_GetFullName(gpuHandle, gpuName) == NVAPI_OK) {
                strncpy(gpuInfo.name, gpuName, sizeof(gpuInfo.name));
            }

            NvU32 num;
            if (NvAPI_GPU_GetGpuCoreCount(gpuHandle, &num) == NVAPI_OK) {
                gpuInfo.coreCount = num;
            }

            NV_GPU_VR_READY vr;
            if (NvAPI_GPU_GetVRReadyData(gpuHandle, &vr) == NVAPI_OK) {
                gpuInfo.isVrReady = vr.isVRReady;
            }

            NV_GPU_DYNAMIC_PSTATES_INFO_EX statesInfo;
            statesInfo.version = NV_GPU_DYNAMIC_PSTATES_INFO_EX_VER;
            if (NvAPI_GPU_GetDynamicPstatesInfoEx(gpuHandle, &statesInfo) == NVAPI_OK) {
                gpuInfo.powerUtilization = statesInfo.utilization[0].percentage;
            }

            NV_DISPLAY_DRIVER_MEMORY_INFO memoryInfo = { 0 };
            memoryInfo.version = NV_DISPLAY_DRIVER_MEMORY_INFO_VER;
            if (NvAPI_GPU_GetMemoryInfo(gpuHandle, &memoryInfo) == NVAPI_OK) {
                gpuInfo.memorySizeMB = memoryInfo.availableDedicatedVideoMemory / 1024;
            }
        }

        gpuInfo.gpuClockKHz = gpuClocks.domain[NVAPI_GPU_PUBLIC_CLOCK_GRAPHICS].frequency / 1000;
        gpuInfo.vramClockKHz = gpuClocks.domain[NVAPI_GPU_PUBLIC_CLOCK_MEMORY].frequency / 1000;

        NvAPI_Unload();
        return gpuInfo;
    }
}