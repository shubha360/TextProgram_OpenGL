#pragma once

#include <SDL/SDL.h>
#include <TTF/SDL_ttf.h>
#include <GL/glew.h>

#include <string>
#include <cstddef>

#include "Vertex.h"
#include "ShaderProgram.h"

class TextProgram {
public:
	TextProgram(const std::string& text);
	~TextProgram();

	void run();

private:

	std::string m_text;

	SDL_Window* m_window = nullptr;
	int m_windowWidth = 1024;
	int m_windowHeight = 768;
	bool m_sdlInitialized = false;

	GLuint m_vaoID = 0;
	GLuint m_vboID = 0;
	Vertex m_vertices[6];
	float m_textureTopLeftPosX = -0.50f;
	float m_textureTopLeftPosY = 0.50f;
	float m_textureWidth = 1.0f;
	float m_textureHeight = 1.0f;

	GLuint m_textureID = 0;

	ShaderProgram m_shaderProgram;

	void init();
	void initShaders();
	void initVertexArray();
	void generateTexture();
	void close();
	void reportError(const std::string& error);
};