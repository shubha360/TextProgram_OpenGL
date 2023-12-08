#pragma once

#include <SDL/SDL.h>
#include <TTF/SDL_ttf.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>

class TextProgram {
public:
	TextProgram(const std::string& text);
	~TextProgram();

	void run();

private:

	std::string m_text;

	SDL_Window* m_window = nullptr;

	glm::ivec2 m_windowDimensions = glm::ivec2(1024, 768);

	bool m_sdlInitialized = false;

	void init();
	void close();
	void reportError(const std::string& error);
};