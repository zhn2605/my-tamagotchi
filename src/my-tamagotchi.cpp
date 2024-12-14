// C++ STL
#include <iostream> 
#include <string>
#include <vector>

#include <SDL.h>
#include <glad/glad.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Libraries
#include "App.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "Shader.hpp"

// Globals
float deltaTime = 0.0f;
float lastFrame = 0.0f;

std::string vertexShaderSource = "./assets/shaders/vert.glsl";
std::string fragmentShaderSource = "./assets/shaders/frag.glsl";

App app;
Shader* graphicsShader;
Camera camera;
Scene scene;

void CreateGraphicsPipeline() {
    std::string vertexShaderSource = "./assets/shaders/vert.glsl";
    std::string fragmentShaderSource = "./assets/shaders/frag.glsl";

    graphicsShader = new Shader(vertexShaderSource, fragmentShaderSource);
    graphicsShader->useProgram();
    scene.SetShaderProgram(graphicsShader->shaderProgram);
}

void GetOpenGLVersionInfo() {
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version: %s\n", glGetString(GL_VERSION));
    printf("Shading Language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void InitializeProgram() {
    app.Create(640, 480, "myTamagotchi.exe");
    app.Initialize();

    // Initialize the Glad Library
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cout << "Could not initialize glad.\n" << std::endl;
        exit(1);
    }

    GetOpenGLVersionInfo();
}

// SDL Input Handling
void Input() {
    static int mouseX = app.getWidth() / 2;
    static int mouseY = app.getHeight() / 2;

    SDL_Event e;
    // Retrieve keyboard state
    const Uint8* state = SDL_GetKeyboardState(NULL);

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            std::cout << "goodbye!!" << std::endl;
            app.setActive(false);
        }
        else if (e.type == SDL_WINDOWEVENT) {
            if (e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {

                int newWidth = e.window.data1;
                int newHeight = e.window.data2;

                // Update OpenGL viewport
                glViewport(0, 0, newWidth, newHeight);

                // Update camera's aspect ratio
                float aspect = static_cast<float>(newWidth) / static_cast<float>(newHeight);
                camera.UpdateAspectRatio(aspect);

                app.setWidth(newWidth);
                app.setHeight(newHeight);

            }
        }
        else if (e.type == SDL_MOUSEMOTION) {
            mouseX += e.motion.xrel;
            mouseY += e.motion.yrel;
            camera.MouseLook(mouseX, mouseY);
        }
    }

    // Screen Logic
    Uint32 windowFlags = SDL_GetWindowFlags(app.getWindow());
    if (state[SDL_SCANCODE_F]) {
        SDL_SetWindowFullscreen(app.getWindow(), SDL_WINDOW_FULLSCREEN_DESKTOP);
    }

    float multiplier = 1.0;
    const float base_fov = 60.0f;
    const float max_fov = 100.0f;
    const float fov_tween = 2.0f;

    if (state[SDL_SCANCODE_LSHIFT]) {
        multiplier = 3.0;

        float fov_delta = (max_fov - base_fov) * (log(multiplier) / fov_tween);

        camera.SetFovy(base_fov + fov_delta);
    }
    else {
        camera.SetFovy(base_fov);
    }

    float speed = 0.01f * deltaTime * multiplier;
    if (state[SDL_SCANCODE_W]) {
        camera.MoveForward(speed);
    }
    if (state[SDL_SCANCODE_S]) {
        camera.MoveBackward(speed);
    }
    if (state[SDL_SCANCODE_D]) {
        camera.MoveRight(speed);
    }
    if (state[SDL_SCANCODE_A]) {
        camera.MoveLeft(speed);
    }
    if (state[SDL_SCANCODE_C]) {
        camera.MoveDown(speed);
    }
    if (state[SDL_SCANCODE_SPACE]) {
        camera.MoveUp(speed);
    }

    if (state[SDL_SCANCODE_ESCAPE]) {
        if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
            SDL_SetRelativeMouseMode(SDL_FALSE);
        }
        else {
            SDL_SetRelativeMouseMode(SDL_TRUE);
        }
    }
}

void InitializeModels() {
    std::cout << "Initializing Models" << std::endl;
    Model* cat = scene.CreateModel("cat", "./assets/models/tamagotchi/Kitten/kitten_01.obj");
    cat->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    cat->SetScale(glm::vec3(.2f));

    // Light object
    graphicsShader->setUniformVec3("light.position", glm::vec3(0.0f, 1.0f, 0.2f));
    graphicsShader->setUniformVec3("light.ambient", glm::vec3(0.0f));
    graphicsShader->setUniformVec3("light.diffuse", glm::vec3(1.0f));
    graphicsShader->setUniformVec3("light.specular", glm::vec3(1.2f));
}

void PrepareDraw() {
    scene.PrepareDraw(app.getWidth(), app.getHeight());

    glEnable(GL_DEPTH);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    graphicsShader->setUniformMat4("u_ModelMatrix", scene.GetObject("cat")->GetModelMatrix());
}

void Draw() {
    scene.DrawObjects(camera.GetViewMatrix(), camera.GetProjectionMatrix(), graphicsShader);
    // scene.UpdateAll();
}

void MainLoop() {
    // Set mouse in middle of window
    SDL_WarpMouseInWindow(app.getWindow(), app.getWidth() / 2, app.getHeight() / 2);

    // Set mouse to move relatively within application
    SDL_SetRelativeMouseMode(SDL_TRUE);

    while (app.isActive()) {
        // Create delta time
        float currentFrame = 1.0f * SDL_GetTicks();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        Input();

        PrepareDraw();

        Draw();

        // Update the screen
        SDL_GL_SwapWindow(app.getWindow());
    }
}

void CleanUp() {
    graphicsShader->deleteProgram();
    
    // Terminate app
    app.Terminate();
}

#undef main // ??? why is there the same bug
// bug fix: overlap main declarations in SDL
int main()
{
    InitializeProgram();

    CreateGraphicsPipeline();

    InitializeModels();

    MainLoop();

    CleanUp();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
