#include <SDL.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "misc/experimenting.h"

#define persistent_variable static
#define global_variable static
#define private_function static

// Segmentation fault occurs when quickly resizing window when using mmap
// https://gist.github.com/ITotalJustice/eb9c47d0fdbc34e275b2ca79460bf0e3
#define USE_MMAP 1
#define BYTES_PER_PIXEL 4

global_variable SDL_Texture* texture;
global_variable int textureWidth;
global_variable void* pixels;

void handleFatalError() {
    const char *errorMessage = SDL_GetError();
    printf("A fatal error occurred: %s", errorMessage);

    SDL_Quit();

    exit((unsigned char)255);
}

private_function void resizeTexture(SDL_Renderer *renderer, int width, int height) {
    int pixelsSize = width * height * BYTES_PER_PIXEL;
    printf("hello?");

    if(texture) {
        SDL_DestroyTexture(texture);
    }

    if(pixels) {
        if(USE_MMAP) {
            munmap(pixels, pixelsSize);
        } else {
            free(pixels);
        }
    }

    printf("before");
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888, // 4 bytes per pixel
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );
    printf("after");

    if(USE_MMAP) {
        pixels = mmap(
            0,
            pixelsSize,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS,
            -1,
            0
        );
        if (pixels == MAP_FAILED) {
            perror("mmap");
            exit(1);
        }
    } else {
        pixels = malloc(pixelsSize);
    }
    textureWidth = width;
}

private_function bool handleEvent(SDL_Event *event) {
    bool shouldQuit = false;
    // https://wiki.libsdl.org/SDL3/SDL_EventType
    switch (event->type) {
        case SDL_QUIT:
        {
            printf("SDL_EVENT_QUIT\n");
            shouldQuit = true;
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
                    // This event is also emitted on window creation so the texture will always be initialized by this
                    resizeTexture(renderer, newWidth, newHeight);
                    printf("Resized to %d x %d\n", newWidth, newHeight);
                } break;

                case SDL_WINDOWEVENT_EXPOSED:
                {
                    SDL_Window *window = SDL_GetWindowFromID(event->window.windowID);
                    SDL_Renderer *renderer = SDL_GetRenderer(window);

                    if(!texture) {
                        printf("Initializing texture!\n");
                        int width, height;
                        SDL_GetWindowSize(window, &width, &height);
                        resizeTexture(renderer, width, height);
                    }

                    if(SDL_UpdateTexture(texture, 0, pixels, textureWidth * BYTES_PER_PIXEL) != 0) {
                        handleFatalError();
                    }

                    if(SDL_RenderCopy(renderer, texture, 0, 0) != 0) {
                        handleFatalError();
                    }

                    SDL_RenderPresent(renderer);
                } break;
            }
        } break;
    }

    return(shouldQuit);
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
