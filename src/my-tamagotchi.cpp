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
#include "Shader.hpp"

// Application Instance
App app;
Shader* graphicsShader;
Camera camera;

// Globals
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// VAO
GLuint vertexArrayObject = 0;
// VBO
GLuint vertexBufferObject = 0;      // position
// Program Objects (for shaders)
GLuint elementBufferObject = 0;
GLuint graphicsPipelineShaderProgram = 0;

float uOffset = -2.0f;
float uRotate = 0.0f;
float uScale = 1.0f;

void CreateGraphicsPipeline() {
    std::string vertexShaderSource = "./assets/shaders/vert.glsl";
    std::string fragmentShaderSource = "./assets/shaders/frag.glsl";

    graphicsShader = new Shader(vertexShaderSource, fragmentShaderSource);
    graphicsShader->useProgram();
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

    // Move Logic
    // Speed
    float multiplier = state[SDL_SCANCODE_LSHIFT] ? 3.0f : 1.0f;
    float speed = 0.001f * deltaTime * multiplier;


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

void VertexSpecification() {
    const std::vector<GLfloat> vertices{  // Create dynamic array
        // Diamond
        -0.25f, 0.0f, 0.0f,     // left
        139.0f, 0.25f, 0.5f,     // color

        0.0f, 0.25f, 0.0f,       // top
        139.0f, 0.5f, 0.25f,      // color

        0.25f, 0.0f, 0.0f,      // right
        139.0f, 0.55f, 0.3f,     // color

        0.0f, -0.25f, 0.0f,     // bottom
        139.0f, 0.15f, 0.15f,     // color
    };


    const std::vector<GLint> vertexIndicies{
        0, 1, 2,                // top triangle
        0, 3, 2,                // bot triangle
    };

    // VAO Specification
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Create VBO
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3, // xyz
        GL_FLOAT,
        GL_FALSE,
        sizeof(GL_FLOAT) * 6,
        (void*)0
    );

    // Enable color 
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3, // rgb
        GL_FLOAT,
        GL_FALSE,
        sizeof(GL_FLOAT) * 6,
        (void*)(sizeof(GL_FLOAT) * 3)
    );

    // Create EBO
    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndicies.size() * sizeof(GLint), vertexIndicies.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

// Open GL Drawing
void PreDraw() {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, app.getWidth(), app.getHeight());        // creates a viewport starting left corner (0,0) 
    glClearColor(0.2f, 0.3f, .3f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    graphicsShader->useProgram();     // modifying shaders in program object will not affect curr executables

    // Important: ORDER MATTERS (T, R, S for rotation around object. R, T, S for simulated rotation around us
    // Rotation test
    uRotate -= .1f * deltaTime;
    //std::cout << "uRotate: " << uRotate << std::endl;

    // Translate
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, uOffset));
    // Rotate
    model = glm::rotate(model, glm::radians(uRotate), glm::vec3(0.0f, 1.0f, 0.0f));
    // Scale
    model = glm::scale(model, glm::vec3(uScale, uScale, uScale));

    GLint u_ModelMatrixLocation = graphicsShader->getUniformLocation("u_ModelMatrix");
    if (u_ModelMatrixLocation >= 0) {
        glUniformMatrix4fv(u_ModelMatrixLocation, 1, GL_FALSE, &model[0][0]);
    }
    else {
        std::cout << "could not find location of u_ModelMatrix. Mispelling?" << std::endl;
        exit(EXIT_FAILURE);
    }

    // View Matrix
    glm::mat4 view = camera.GetViewMatrix();
    GLint u_viewLocataion = graphicsShader->getUniformLocation("u_ViewMatrix");
    if (u_viewLocataion >= 0) {
        glUniformMatrix4fv(u_viewLocataion, 1, GL_FALSE, &view[0][0]);
    }
    else {
        std::cout << "could not find location of u_View. Mispelling?" << std::endl;
        exit(EXIT_FAILURE);
    }

    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)app.getWidth() / (float)app.getHeight(), 0.1f, 10.0f);
    GLint u_ProjectionLocation = graphicsShader->getUniformLocation("u_Projection");
    if (u_ProjectionLocation >= 0) {
        glUniformMatrix4fv(u_ProjectionLocation, 1, GL_FALSE, &perspective[0][0]);
    }
    else {
        std::cout << "could not find location of u_Projection. Mispelling?" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Draw() {
    // Sin diamond test
    glBindVertexArray(vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void MainLoop() {
    // Set mouse in middle of window
    SDL_WarpMouseInWindow(app.getWindow(), app.getWidth() / 2, app.getHeight() / 2);

    // Set mouse to move relatively within application
    SDL_SetRelativeMouseMode(SDL_TRUE);

    while (app.isActive()) {
        // Create delta time
        float currentFrame = 1.0 * SDL_GetTicks();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        Input();

        PreDraw();

        Draw();

        // Update the screen
        SDL_GL_SwapWindow(app.getWindow());
    }
}

void CleanUp() {
    glDeleteProgram(graphicsPipelineShaderProgram);
    
    // Terminate app
    app.Terminate();
}

#undef main // ??? why is there the same bug
// bug fix: overlap main declarations in SDL
int main()
{
    InitializeProgram();

    VertexSpecification();

    CreateGraphicsPipeline();

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
