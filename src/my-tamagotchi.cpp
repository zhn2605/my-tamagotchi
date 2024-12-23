// C++ STL
#include <iostream> 
#include <string>
#include <vector>

#include <SDL.h>
#include <glad/glad.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// irrKlang
#include <irrKlang.h>
using namespace irrklang;

// Libraries
#include "app.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include "shader.hpp"

// Globals
float currentFrame = 0.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;;

App app;
Shader* graphicsShader;
Shader* lightShader;
Camera camera;
Scene scene;
ISoundEngine* SoundEngine = createIrrKlangDevice();

void CreateGraphicsPipeline() {
    std::string vertexShaderSource = "./assets/shaders/vert.glsl";
    std::string fragmentShaderSource = "./assets/shaders/frag.glsl";
    std::string lightVertexSource = "./assets/shaders/lightvert.glsl";
    std::string lightFragmentSource = "./assets/shaders/lightfrag.glsl";

    graphicsShader = new Shader(vertexShaderSource, fragmentShaderSource);
    lightShader = new Shader(lightVertexSource, lightFragmentSource);
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

    if (!SoundEngine) {
        std::cout << "Failed to create sound engine from irrKlang.\n" << std::endl;
        exit(1);
    }
    else {
        ISound* rain = SoundEngine->play2D("./assets/audio/music/heavy-rain.wav", true, false, true);
        rain->setVolume(0.1f);
        rain->setMinDistance(1.0f);
        rain->setMaxDistance(3.0f);

        glm::vec3 cameraPos = camera.GetEye();
        glm::vec3 lookDir = camera.GetLookDir();

        SoundEngine->setListenerPosition(vec3df(cameraPos.x, cameraPos.y, cameraPos.z), vec3df(lookDir.x, lookDir.y, lookDir.z));
    }

    // Initialize random seed
    srand(time(NULL));

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
    Model* cat = scene.GetObject("cat");
    Model* frog = scene.GetObject("frog");

    // switch from cat to frog
    if (state[SDL_SCANCODE_1]) {
        cat->SetPosition(glm::vec3(0.0f));
        cat->SetRotation(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        cat->SetScale(glm::vec3(.2f));
        frog->SetPosition(glm::vec3(100.0f));
        frog->SetRotation(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        frog->SetScale(glm::vec3(0.1f));
    }
    if (state[SDL_SCANCODE_2]) {
        cat->SetPosition(glm::vec3(100.0f));
        cat->SetRotation(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        cat->SetScale(glm::vec3(.2f));
        frog->SetPosition(glm::vec3(0.0f));
        frog->SetRotation(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        frog->SetScale(glm::vec3(0.1f));
    }

    float speed = deltaTime * multiplier;
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
    cat->SetRotation(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    cat->SetScale(glm::vec3(.2f));

    Model* frog = scene.CreateModel("frog", "./assets/models/tamagotchi/Frog/frog_01.obj");
    frog->SetPosition(glm::vec3(100.0f, 0.0f, 0.0f));
    frog->SetRotation(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    frog->SetScale(glm::vec3(0.1f));
}

void MainLoop() {
    // Set camera position
    camera.SetEyePosition(glm::vec3(0.0f, 1.3f, 2.3f));

    // Set mouse in middle of window
    SDL_WarpMouseInWindow(app.getWindow(), app.getWidth() / 2, app.getHeight() / 2);

    // Set mouse to move relatively within application
    SDL_SetRelativeMouseMode(SDL_TRUE);

    // Set random timer
    int timer = rand() % 30 + 1;

    float nextSoundTime = 0.0f;
    float currentTime = 0.0f;

    while (app.isActive()) {
        // Create delta time
        currentFrame = 1.0f * SDL_GetTicks();
        deltaTime = (currentFrame - lastFrame) / 1000.0f;
        lastFrame = currentFrame;

        // Update current time
        currentTime += deltaTime;

        if (currentTime >= nextSoundTime) {
            SoundEngine->play2D("./assets/audio/sfx/meow.wav", false);  // false means don't loop the sound

            float randomInterval = 1.0f + (rand() % 30);
            nextSoundTime = currentTime + randomInterval;
        }

        // Player position
        glm::vec3 cameraPos = camera.GetEye();
        glm::vec3 lookDir = camera.GetLookDir();

        // Update sound
        SoundEngine->setListenerPosition(vec3df(cameraPos.x, cameraPos.y, cameraPos.z), vec3df(lookDir.x, lookDir.y, lookDir.z));

        Input();

        // Prepare Draw
        scene.PrepareDraw(app.getWidth(), app.getHeight());

        // Draw
        scene.DrawObjects(camera.GetViewMatrix(), camera.GetProjectionMatrix(), graphicsShader, lightShader);

        // Light object
        graphicsShader->setUniformVec3("light.position", glm::vec3(0.0f));
        graphicsShader->setUniformVec3("light.ambient", glm::vec3(0.03f, 0.0f, 0.0f));
        graphicsShader->setUniformVec3("light.diffuse", glm::vec3(1.0f));
        graphicsShader->setUniformVec3("light.specular", glm::vec3(1.2f));
        // scene.UpdateAll(); 

        // Update the screen
        SDL_GL_SwapWindow(app.getWindow());
    }
}

void CleanUp() {
    // Clean up objects
    scene.CleanUpAll();

    // Delete pipeline
    graphicsShader->deleteProgram();

    // Remove sound engine
    SoundEngine->drop();
    
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
