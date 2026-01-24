#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

#include "misc/experimenting.h"

#define persistent_variable static
#define global_variable static
#define private_function static

#define BYTES_PER_PIXEL 4

global_variable SDL_Texture* texture;
global_variable int textureWidth;
global_variable void* pixels;

void handleFatalError() {
    const char *errorMessage = SDL_GetError();
    OutputDebugString("A fatal error occurred");
    OutputDebugString(errorMessage);

    SDL_Quit();

    exit((unsigned char)255);
}

private_function void resizeTexture(SDL_Renderer *renderer, int width, int height) {
    if(texture) {
        SDL_DestroyTexture(texture);
    }

    if(pixels) {
        free(pixels);
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888, // 4 bytes per pixel
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );
    pixels = malloc(width * height * BYTES_PER_PIXEL);
    textureWidth = width;
}

private_function bool handleEvent(SDL_Event *event) {
    bool shouldQuit = false;

    // https://wiki.libsdl.org/SDL3/SDL_EventType
    switch (event->type) {
        case SDL_EVENT_QUIT:
        {
            OutputDebugString("SDL_EVENT_QUIT\n");
            shouldQuit = true;
        } break;

        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
        {
            int newWidth = ((SDL_WindowEvent*)event)->data1;
            int newHeight = ((SDL_WindowEvent*)event)->data2;

            SDL_Window *window = SDL_GetWindowFromID(event->window.windowID);
            SDL_Renderer *renderer = SDL_GetRenderer(window);
            // This event is also emitted on window creation so the texture will always be initialized by this
            resizeTexture(renderer, newWidth, newHeight);

            OutputDebugString("Resized");
        } break;

        case SDL_EVENT_WINDOW_EXPOSED:
        {
            SDL_Window *window = SDL_GetWindowFromID(event->window.windowID);
            SDL_Renderer *renderer = SDL_GetRenderer(window);

            if(!SDL_UpdateTexture(texture, 0, pixels, textureWidth)) {
                handleFatalError();
            }

            if(!SDL_RenderTexture(renderer, texture, 0, 0)) {
                handleFatalError();
            }

            SDL_RenderPresent(renderer);
        } break;
    }

    return(shouldQuit);
}

int WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nShowCmd
) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Handmade Thrash", "This is Handmade Thrash", 0);

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

    claimWindowForGpu(window);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0);
    if(!renderer) {
        handleFatalError();
    }

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
