#include <cassert>
#include <cstring>
#include <iostream>
#include <SDL.h>

bool g_isRunning = true;
SDL_Renderer* g_mainRenderer;
SDL_Window* g_mainWindow;
SDL_Surface* g_mainWindowSurface;
SDL_Texture* g_mainRT;

bool g_isWindowed = true;
int g_width = 800;
int g_height = 600;

void* g_pixels;
int g_pitch;

unsigned* g_backbuffer;

void SDL_ErrorHandler(const std::string msg)
{
    std::cout << msg << ", SDL Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    exit(1);
}

void CheckSDL_Error(int line)
{
#ifndef NDEBUG
    const char* error = SDL_GetError();
    if (*error != '\0')
    {
        printf("SDL Error: %s\n", error);
        if (line != -1)
        {
            printf(" + line: %i\n", line);
        }
        SDL_ClearError();
    }
#endif
}

// 初始化
void Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_ErrorHandler("Couldn't initialize SDL Video Subsystem");
    }

    SDL_DisplayMode dm;
    if (SDL_GetCurrentDisplayMode(0, &dm) != 0)
    {
        SDL_ErrorHandler("Call SDL_GetCurrentDisplayMode() failed");
    }

    // Create main window
    g_mainWindow = SDL_CreateWindow("renderer", (dm.w - g_width) * 0.5f, (dm.h - g_height) * 0.5f, g_width, g_height, g_isWindowed ? SDL_WINDOW_SHOWN : SDL_WINDOW_FULLSCREEN);
    if (g_mainWindow == nullptr)
    {
        SDL_ErrorHandler("Could not create main window");
    }

    // Create rendering context for the main window
    g_mainRenderer = SDL_CreateRenderer(g_mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (g_mainRenderer == nullptr)
    {
        SDL_ErrorHandler("Could not create rendering context for the main window");
    }

    g_mainWindowSurface = SDL_GetWindowSurface(g_mainWindow);
    g_mainRT = SDL_CreateTexture(g_mainRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, g_width, g_height);

    CheckSDL_Error(__LINE__);

    g_backbuffer = new unsigned[g_width * g_height];
    for (int i = 0; i < g_height - 1; i++)
    {
        for (int j = 0; j < g_width - 1; j++)
        {
            g_backbuffer[i * g_width + j] = 0xFF00FFFF; // ARGB
        }
    }
}

// 结束
void Terminate()
{
    SDL_DestroyTexture(g_mainRT);
    SDL_DestroyRenderer(g_mainRenderer);
    SDL_DestroyWindow(g_mainWindow);
    SDL_Quit();
}

// 处理输入事件
void HandleEvents()
{

}

// 物理和逻辑计算
void Update()
{

}

// 渲染一帧
void RenderFrame()
{
    SDL_LockTexture(g_mainRT, nullptr, &g_pixels, &g_pitch);

    auto backbuffer = g_backbuffer;
    assert(backbuffer != nullptr);

    memcpy(g_pixels, backbuffer, g_mainWindowSurface->pitch * g_mainWindowSurface->h);

    SDL_UnlockTexture(g_mainRT);

    SDL_RenderCopy(g_mainRenderer, g_mainRT, nullptr, nullptr);
    SDL_RenderPresent(g_mainRenderer);
}

// 游戏循环
void Run()
{
    while (g_isRunning)
    {
        HandleEvents();
        Update();
        RenderFrame();
        SDL_Delay(33);
    }
}

int main(int argc, char* argv[])
{
    Initialize();

    Run();

    Terminate();

    return 0;
}