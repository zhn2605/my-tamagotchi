#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <SDL.h>
#include <glad/glad.h>
#include <string>

class App{
public:
	App();

	void Create(int width, int height, const char* windowTitle);

	void Initialize();
	void Terminate();

	// Setters
	void setActive(bool value);
	void setWidth(int value);
	void setHeight(int value);

	// Getters
	int getWidth();
	int getHeight();
	SDL_Window* getWindow();
	SDL_GLContext getContext();
	bool isActive();
	


private:
	const char* title;
	int screenWidth;
	int screenHeight;
	SDL_Window* window = nullptr;
	SDL_GLContext openGlContext = nullptr;
	bool active;
};


#endif