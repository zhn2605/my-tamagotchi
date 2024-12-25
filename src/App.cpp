#include "app.hpp"
#include <iostream>

App::App() {
    screenWidth = 0;
    screenHeight = 0;
    title = "test";
}

void App::Create(int width, int height, const char* windowTitle) {
	// Assign settings
	screenWidth = width;
	screenHeight = height;
    title = windowTitle;
    active = true;
}

void App::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL2 could not initialize video subsystem." << std::endl;
        exit(1);
    }

    // Set GL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create window
    window = SDL_CreateWindow(
        title,            // title name
        SDL_WINDOWPOS_CENTERED, // x pos
        SDL_WINDOWPOS_CENTERED, // y pos
        screenWidth,            // width
        screenHeight,           // height
        SDL_WINDOW_OPENGL       // Unit32 flags
    );

    if (window == nullptr) {
        std::cout << "Could not create window: %s\n" << SDL_GetError() << std::endl;
        exit(1);
    }

    // Create context
    openGlContext = SDL_GL_CreateContext(window);

    if (openGlContext == nullptr) {
        std::cout << "Could not create openGL context : % s\n" << SDL_GetError() << std::endl;
        exit(1);
    }

    // Enable Resize window
    SDL_SetWindowResizable(window, SDL_TRUE);
}

void App::Terminate() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Setters
void App::setActive(bool value) {
    active = value;
}

void App::setWidth(int value) {
    screenWidth = value;
}

void App::setHeight(int value) {
    screenHeight = value;
}

// Getters
SDL_Window* App::getWindow() { return window; }
SDL_GLContext App::getContext() { return openGlContext; }
bool App::isActive() { return active; }
int App::getWidth() { return screenWidth; }
int App::getHeight() { return screenHeight; }