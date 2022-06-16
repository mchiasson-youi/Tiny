#include <SDL2/SDL.h>

/* macro for a safe call to SDL2 functions */
#define SDLCHECK(x...) \
if ((x) < 0) \
{ \
    SDL_LogCritical(0, "Error occured while calling '%s'at '%s:%d' : %s\n", #x, __FILE__, __LINE__, SDL_GetError()); \
    SDL_Quit(); \
    return EXIT_FAILURE; \
}

SDL_Window *window = nullptr;
SDL_GLContext context = nullptr;

int main(int argc, char * argv[])
{
    SDLCHECK(SDL_Init(SDL_INIT_VIDEO));

    // Force 32 bit double buffer
    SDLCHECK(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
    SDLCHECK(SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8));
    SDLCHECK(SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8));
    SDLCHECK(SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8));
    SDLCHECK(SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8));
    SDLCHECK(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24));
    SDLCHECK(SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8));

    // MSAA (Multi-sample Anti-Aliasing).
    SDLCHECK(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1));
    SDLCHECK(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4));

    // Use OpenGL 3.3 core
    SDLCHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3));
    SDLCHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3));
    SDLCHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE));
    SDLCHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG));

    // Create SDL Window
    SDL_Window *window = SDL_CreateWindow("Tiny", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1600, 900, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window)
    {
        SDL_LogCritical(0, "Window could not be created: %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Create GL Context
    context = SDL_GL_CreateContext(window);
    if (!context)
    {
        SDL_LogCritical(0, "GL context creation error: %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Activate the context by making it the current context to render with.
    SDLCHECK(SDL_GL_MakeCurrent(window, context));

    // do not 'SDLCHECK'. this might not be supported so it's okay to ignore.
    SDL_GL_SetSwapInterval(1);

    bool isRunning = true;

    while(isRunning)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                isRunning = false;
            }
        }

        SDL_GL_SwapWindow(window);
    }

    return EXIT_SUCCESS;
}
