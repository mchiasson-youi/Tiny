#include <glad/glad.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <memory>
#include <SDL2/SDL.h>

#include "SceneNode.h"

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

    std::shared_ptr<SceneNode> sceneNodeA = SceneNode::Create("A");
    std::shared_ptr<SceneNode> sceneNodeB = SceneNode::Create("B");

    sceneNodeB->setParent(sceneNodeA);

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

    std::shared_ptr<ShaderProgram> solidColorProgram = std::make_shared<ShaderProgram>();
    solidColorProgram->attach(&defaultVert);
    solidColorProgram->attach(&defaultFrag);
    solidColorProgram->link();

    std::shared_ptr<Texture> baboon = Texture::Create("assets/baboon.png");

    std::shared_ptr<Material> solidColorMaterial = std::make_shared<Material>();
    solidColorMaterial->setProgram(solidColorProgram);

    sceneNodeA->setMaterial(solidColorMaterial);
    sceneNodeB->setMaterial(solidColorMaterial);

    float rect_size = 200;

    std::vector<Vertex> vertices =
    {
                  //  X               Y         Z               R G B A
        {glm::vec3(-rect_size/2, -rect_size/2, 0.0f), glm::vec4(1,0,0,1), glm::vec2(0, 1)},
        {glm::vec3( rect_size/2, -rect_size/2, 0.0f), glm::vec4(1,0,0,1), glm::vec2(1, 1)},
        {glm::vec3(-rect_size/2,  rect_size/2, 0.0f), glm::vec4(0,1,0,1), glm::vec2(0, 0)},
        {glm::vec3( rect_size/2,  rect_size/2, 0.0f), glm::vec4(0,1,0,1), glm::vec2(1, 0)},

        {glm::vec3(-rect_size/2, -rect_size/2, 0.0f), glm::vec4(1,1,0,1), glm::vec2(0, 1)},
        {glm::vec3( rect_size/2, -rect_size/2, 0.0f), glm::vec4(1,1,0,1), glm::vec2(1, 1)},
        {glm::vec3(-rect_size/2,  rect_size/2, 0.0f), glm::vec4(0,0,1,1), glm::vec2(0, 0)},
        {glm::vec3( rect_size/2,  rect_size/2, 0.0f), glm::vec4(0,0,1,1), glm::vec2(1, 0)},
    };

    std::shared_ptr<VertexBuffer> buffer = std::make_shared<VertexBuffer>();
    buffer->activate();
    buffer->upload(vertices, VertexBuffer::Static);
    buffer->deactivate();

    std::shared_ptr<Mesh> meshA = std::make_shared<Mesh>();
    meshA->setVertexBuffer(buffer);
    meshA->setPrimitive(Primitive::TriangleStrip);
    meshA->setVertexStartOffset(0);
    meshA->setVertexCount(4);

    std::shared_ptr<Mesh> meshB = std::make_shared<Mesh>();
    meshB->setVertexBuffer(buffer);
    meshB->setPrimitive(Primitive::TriangleStrip);
    meshB->setVertexStartOffset(4);
    meshB->setVertexCount(4);

    sceneNodeA->setMesh(meshA);
    sceneNodeB->setMesh(meshB);

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
                case SDL_WINDOWEVENT:
                    switch (e.window.event)
                    {
                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                            width = e.window.data1;
                            height = e.window.data2;
                            break;
                        default:
                            break;
                    }
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
        ImGui::Checkbox("use Ortho (Fez)", &useOrtho);
        if(!useOrtho)
        {
            ImGui::SliderFloat("fieldOfView", &fieldOfView, 0.1, 360);
        }
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
        sceneNodeA->renderImgui(width);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Render();

        glViewport(0, 0, width, height);
        glClearColor(clear_color.r * clear_color.a, clear_color.g * clear_color.a, clear_color.b * clear_color.a, clear_color.a);
        glClear(GL_COLOR_BUFFER_BIT);

        if (useOrtho)
        {
            projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -8000.0f, 8000.0f);
        }
        else
        {
            float fieldOfViewRad = glm::radians(fieldOfView);
            float cameraDistance = ((float)height/2.0f) / tan(fieldOfViewRad/2.0f);
            float aspectRatio = (float)width / (float)height;

            projection = glm::perspective(fieldOfViewRad, aspectRatio, 0.1f, cameraDistance*2.0f);
            projection = glm::translate(projection, glm::vec3(-width*0.5f, -height*0.5f, -cameraDistance));
        }

        solidColorProgram->activate();
        solidColorProgram->setProjection(projection);
        // solidColorProgram->setTexture0(0);

        baboon->activate();

        sceneNodeA->update();
        sceneNodeA->render();

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
