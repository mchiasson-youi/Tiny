#include <glad/glad.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <memory>
#include <SDL2/SDL.h>

#include "ShaderProgram.h"
#include "VertexBuffer.h"

#include <glm/gtc/matrix_transform.hpp>

/* macro for a safe call to SDL2 functions */
#define SDLCHECK(x) \
if ((x) < 0) \
{ \
    SDL_LogCritical(0, "Error occured while calling '%s'at '%s:%d' : %s\n", #x, __FILE__, __LINE__, SDL_GetError()); \
    SDL_Quit(); \
    return EXIT_FAILURE; \
}

SDL_Window *window = nullptr;
SDL_GLContext context = nullptr;

#ifdef WIN32
int WinMain(int argc, char* argv[])
#else
int main(int argc, char* argv[])
#endif
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

    int width = 1600;
    int height = 900;
    float fieldOfView = 45;
    bool vsync = true;
    bool useOrtho = true;

    glm::vec4 clear_color = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);
    glm::mat4 projection;

    // Create SDL Window
    SDL_Window *window = SDL_CreateWindow("Tiny", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
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

    if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        SDL_LogCritical(0,"Error initializing GLAD!");

        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);

        return EXIT_FAILURE;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init(NULL);

    Shader defaultVert("assets/default.vert");
    SDLCHECK (defaultVert.compile())

    Shader defaultFrag("assets/default.frag");
    SDLCHECK(defaultFrag.compile())

    std::unique_ptr<ShaderProgram> defaultProgram = std::make_unique<ShaderProgram>();
    defaultProgram->attach(&defaultVert);
    defaultProgram->attach(&defaultFrag);
    SDLCHECK(defaultProgram->link());

    float rect_size = 200;

    std::vector<Vertex> vertices =
    {
                  //  X          Y         Z
        {glm::vec3(0,           0,         0.0f)},
        {glm::vec3(rect_size,   0,         0.0f)},
        {glm::vec3(0,           rect_size, 0.0f)},
        {glm::vec3(rect_size,   rect_size, 0.0f)},

    };

    glm::vec3 translation(0.0);
    glm::vec3 scale(1.0f);
    glm::vec3 rotation(0.0);

    VertexBuffer buffer;
    buffer.activate();
    buffer.upload(vertices, VertexBuffer::Static);
    buffer.deactivate();

    bool isRunning = true;
    while(isRunning)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0)
        {
            ImGui_ImplSDL2_ProcessEvent(&e);
            switch (e.type)
            {
                case SDL_QUIT:
                    isRunning = false;
                    break;
                default:
                    break;
            }
        }

        SDL_GL_SetSwapInterval(vsync ? 1 : 0);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Tiny Debug Panel");
        ImGui::Checkbox("VSync", &vsync);
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
        ImGui::SliderFloat3("Bottom Left", (float*)&vertices[0], 0, width);
        ImGui::SliderFloat3("Bottom Right", (float*)&vertices[1], 0, width);
        ImGui::SliderFloat3("Top Left", (float*)&vertices[2], 0, width);
        ImGui::SliderFloat3("Top Right", (float*)&vertices[3], 0, width);
        ImGui::SliderFloat3("translation", (float*)&translation[0], 0, width);
        ImGui::SliderFloat3("scale", (float*)&scale[0], 0.0f, 4.0f);
        ImGui::SliderFloat3("rotation", (float*)&rotation[0], 0, 360);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Render();

        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.r * clear_color.a, clear_color.g * clear_color.a, clear_color.b * clear_color.a, clear_color.a);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 translationMat = glm::translate(glm::mat4(1.0), translation);

        glm::mat4 rotationYMat = glm::rotate(glm::mat4(1.0), glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 rotationXMat = glm::rotate(glm::mat4(1.0), glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
        glm::mat4 rotationZMat = glm::rotate(glm::mat4(1.0), glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));

        glm::mat4 scaleMat = glm::scale(glm::mat4(1.0), scale);

        if (useOrtho)
        {
            projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -8000.0f, 8000.0f);
        }
        else
        {
            float fieldOfViewRad = glm::radians(fieldOfView);
            float aspectRatio = (float)width / (float)height;

            projection = glm::perspective(fieldOfViewRad, aspectRatio, -8000.0f, 8000.0f);
        }

        glm::mat4 model = translationMat * rotationYMat * rotationXMat * rotationZMat * scaleMat;

        defaultProgram->activate();
        defaultProgram->setProjection(projection);
        defaultProgram->setModel(model);


        buffer.activate();

        // Just for fun, remove in the future.
        buffer.upload(vertices, VertexBuffer::Dynamic);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());

        defaultProgram->deactivate();
        buffer.deactivate();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
