#include <SDL.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include<unistd.h>

#include "misc/experimenting.h"

#define persistent_variable static
#define global_variable static
#define private_function static

// Segmentation fault occurs when quickly resizing window when using mmap
// https://gist.github.com/ITotalJustice/eb9c47d0fdbc34e275b2ca79460bf0e3
#define USE_MMAP 1
#define BYTES_PER_PIXEL 4

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

global_variable SDL_Texture* texture;
global_variable void* bitmap;
global_variable int bitmapWidth;
global_variable int bitmapHeight;

void handleFatalError() {
    const char *errorMessage = SDL_GetError();
    printf("A fatal error occurred: %s", errorMessage);

    SDL_Quit();

    exit((unsigned char)255);
}

private_function void renderGradient(int blueOffset, int greenOffset)
{    
    int width = bitmapWidth;
    int height = bitmapHeight;

    int pitch = width * BYTES_PER_PIXEL;
    
    uint8 *row = (uint8 *)bitmap;    
    for(int y = 0; y<bitmapHeight;++y) {
        uint32 *pixel = (uint32 *)row;
        for(int x = 0; x < bitmapWidth; ++x) {
            uint8 Blue = (x + blueOffset);
            uint8 Green = (y + greenOffset);

            *pixel++ = ((Green << 8) | Blue);
        }

        row += pitch;
    }
}

private_function void resizeTexture(SDL_Renderer *renderer, int width, int height) {
    int bitmapSize = width * height * BYTES_PER_PIXEL;

    if(texture) {
        SDL_DestroyTexture(texture);
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888, // 4 bytes per pixel
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );

    if(bitmap) {
        if(USE_MMAP) {
            munmap(bitmap, bitmapSize);
        } else {
            free(bitmap);
        }
    }

    if(USE_MMAP) {
        bitmap = mmap(
            0,
            bitmapSize,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS,
            -1,
            0
        );
        if (bitmap == MAP_FAILED) {
            perror("mmap");
            exit(1);
        }
    } else {
        bitmap = malloc(bitmapSize);
    }

    bitmapWidth = width;
    bitmapHeight = height;
}

private_function void rerenderWindow(SDL_Window *window, SDL_Renderer *renderer) {
    if(!texture) {
        printf("Initializing texture!\n");
        int width, height;
        SDL_GetWindowSize(window, &width, &height);
        resizeTexture(renderer, width, height);
    }

    if(SDL_UpdateTexture(texture, 0, bitmap, bitmapWidth * BYTES_PER_PIXEL) != 0) {
        handleFatalError();
    }

    if(SDL_RenderCopy(renderer, texture, 0, 0) != 0) {
        handleFatalError();
    }

    SDL_RenderPresent(renderer);
}

private_function bool handleEvent(SDL_Event *event) {
    bool keepRunning = true;
    // https://wiki.libsdl.org/SDL3/SDL_EventType
    switch (event->type) {
        case SDL_QUIT:
        {
            printf("SDL_EVENT_QUIT\n");
            keepRunning = false;
        } break;

        case SDL_WINDOWEVENT:
        {
            switch(event->window.event) {
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                {
                    int newWidth = ((SDL_WindowEvent*)event)->data1;
                    int newHeight = ((SDL_WindowEvent*)event)->data2;

                    SDL_Window *window = SDL_GetWindowFromID(event->window.windowID);
                    SDL_Renderer *renderer = SDL_GetRenderer(window);
                    resizeTexture(renderer, newWidth, newHeight);
                    printf("Resized to %d x %d\n", newWidth, newHeight);
                } break;

                case SDL_WINDOWEVENT_EXPOSED:
                {
                    SDL_Window *window = SDL_GetWindowFromID(event->window.windowID);
                    SDL_Renderer *renderer = SDL_GetRenderer(window);

                    rerenderWindow(window, renderer);
                } break;
            }
        } break;
    }

    return(keepRunning);
}

int main(int argc, char *argv[]) {
    // SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Handmade Thrash", "This is Handmade Thrash", 0);

    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        handleFatalError();
    }

    SDL_Window *window;

    window = SDL_CreateWindow(
        "Handmade Thrash",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800,
        600,
        0 | SDL_WINDOW_RESIZABLE
    );

    if(!window) {
        handleFatalError();
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer) {
        handleFatalError();
    }

    int xOffset = 0;
    int yOffset = 0;
    bool keepRunning = true;
    while(keepRunning) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            keepRunning = handleEvent(&event);
        }
        renderGradient(xOffset, yOffset);
        rerenderWindow(window, renderer);

        ++xOffset;
        yOffset += 2;
     
        // Sleep not to use 100% CPU
        usleep(10 * 1000); // ms * 1000
    }

    SDL_Quit();
    return(0);
}
