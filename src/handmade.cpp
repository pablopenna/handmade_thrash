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

#define persistent_variable static
#define global_variable static
#define private_function static

global_variable bool isRunning;

bool handleEvent(SDL_Event *event) {
    bool shouldQuit = false;

    // https://wiki.libsdl.org/SDL3/SDL_EventType
    switch (event->type) {
        case SDL_EVENT_QUIT:
        {
            printf("SDL_EVENT_QUIT\n");
            shouldQuit = true;
        } break;

        case SDL_EVENT_WINDOW_RESIZED:
        {
            int newWidth = ((SDL_WindowEvent*)event)->data1;
            int newHeight = ((SDL_WindowEvent*)event)->data2;
            printf("Resized to %d x %d\n", newWidth, newHeight);
        } break;

        case SDL_EVENT_WINDOW_EXPOSED:
        {
            SDL_Window *window = SDL_GetWindowFromID(event->window.windowID);
            SDL_Renderer *renderer = SDL_GetRenderer(window);
            static bool isWhite = true;

            if(isWhite) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }
            isWhite = !isWhite;

            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
        } break;
    }

    return(shouldQuit);
}

void handleFatalError() {
    const char *errorMessage = SDL_GetError();
    printf("A fatal error occurred: %s", errorMessage);

    SDL_Quit();

    exit((unsigned char)255);
}

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

int main(int argc, char *argv[]) {
    // SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Handmade Thrash", "This is Handmade Thrash", 0);

    if(!SDL_Init(SDL_INIT_VIDEO)) {
        handleFatalError();
    }

    SDL_Window *window;

    window = SDL_CreateWindow(
        "Handmade Thrash",
        800,
        600,
        0 | SDL_WINDOW_RESIZABLE
    );

    if(!window) {
        handleFatalError();
    }

    showAvailableDrivers();

    claimWindowForGpu(window);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0);
    if(!renderer) {
        handleFatalError();
    }
    const char* render_name = SDL_GetRendererName(renderer);

    for(;;) {
        SDL_Event event;
        if(!SDL_WaitEvent(&event)) {
            handleFatalError();
        }
        if(handleEvent(&event)) {
            break;
        }
    }

    SDL_Quit();
    return(0);
}
