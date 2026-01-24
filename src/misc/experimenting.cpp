#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include "SDL3/SDL_gpu.h"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void showAvailableDrivers() {
    int numDrivers = SDL_GetNumRenderDrivers();
    printf("Nº available drivers: %d\n", numDrivers);
    for(int i=0;i<numDrivers;i++) {
        const char* info = SDL_GetRenderDriver(i);
        printf("Available driver: %s\n", info);
    }
}

void claimWindowForGpu(SDL_Window *window) {
    SDL_GPUDevice *gpuDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_PRIVATE | SDL_GPU_SHADERFORMAT_SPIRV, true, 0);
    bool success = SDL_ClaimWindowForGPUDevice(gpuDevice, window);
    printf("was successful claiming window for GPU: %b\n", success);
}