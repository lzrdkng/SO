#include "Application.hpp"
#include "Error.hpp"

using namespace SDL;

void Application::init(InitFlags flags)
{
    static bool isInit = false;

    if (!isInit)
    {
        if (SDL_Init(flags) != 0)
            throw Error(SDL_GetError());

        isInit = true;
    }
    else
    {
        if (SDL_InitSubSystem(flags) != 0)
            throw Error(SDL_GetError());
    }
}

#ifdef _SDL_IMAGE_H
void Application::initImage(ImageInitFlags flags)
{
    if ((IMG_Init(flags) & flags) != flags)
        throw Error(IMG_GetError());
}
#endif

void Application::delay(Uint32 ms)
{
    SDL_Delay(ms);
}

Application::~Application()
{
#ifdef _SDL_IMAGE_H
    IMG_Quit();
#endif

    SDL_QuitSubSystem(SDL_WasInit(0));
    SDL_Quit();
}


