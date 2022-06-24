#include <glad/glad.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <memory>
#include <SDL2/SDL.h>

#include "ShaderProgram.h"
#include "VertexBuffer.h"

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
    bool vsync = true;

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


    glm::vec4 clear_color = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);



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
                  //  X     Y     Z
        {glm::vec3(0,           0, 0.0f)},
        {glm::vec3(rect_size,   0, 0.0f)},
        {glm::vec3(0,           rect_size, 0.0f)},
        {glm::vec3(rect_size,   rect_size, 0.0f)},

    };

    glm::vec2 translation;
    glm::vec2 scale(1.0f);
    float rotation = 0.0f;

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
        ImGui::SliderFloat2("translation", (float*)&translation[0], 0, width);
        ImGui::SliderFloat2("scale", (float*)&scale[0], 0.0f, 4.0f);
        ImGui::SliderFloat("rotation", &rotation, 0, 360);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Render();

        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.r * clear_color.a, clear_color.g * clear_color.a, clear_color.b * clear_color.a, clear_color.a);
        glClear(GL_COLOR_BUFFER_BIT);

        float rotation_rad = glm::radians(rotation);

        std::vector<Vertex> transformed_vertices =
        {
            {glm::vec3((cos(rotation_rad) * vertices[0].aPos.x - sin(rotation_rad) * vertices[0].aPos.y) * scale.x + translation.x, 
                       (sin(rotation_rad) * vertices[0].aPos.x + cos(rotation_rad) * vertices[0].aPos.y) * scale.y + translation.y, 0.0f)},
            {glm::vec3((cos(rotation_rad) * vertices[1].aPos.x - sin(rotation_rad) * vertices[1].aPos.y) * scale.x + translation.x, 
                       (sin(rotation_rad) * vertices[1].aPos.x + cos(rotation_rad) * vertices[1].aPos.y) * scale.y + translation.y, 0.0f)},
            {glm::vec3((cos(rotation_rad) * vertices[2].aPos.x - sin(rotation_rad) * vertices[2].aPos.y) * scale.x + translation.x, 
                       (sin(rotation_rad) * vertices[2].aPos.x + cos(rotation_rad) * vertices[2].aPos.y) * scale.y + translation.y, 0.0f)},
            {glm::vec3((cos(rotation_rad) * vertices[3].aPos.x - sin(rotation_rad) * vertices[3].aPos.y) * scale.x + translation.x, 
                       (sin(rotation_rad) * vertices[3].aPos.x + cos(rotation_rad) * vertices[3].aPos.y) * scale.y + translation.y, 0.0f)},
        };

        defaultProgram->activate();
        defaultProgram->set_screen_size(width, height);
        buffer.activate();
        buffer.upload(transformed_vertices, VertexBuffer::Dynamic);


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
