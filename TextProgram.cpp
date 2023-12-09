#include "TextProgram.h"

TextProgram::TextProgram(const std::string& text) {

	m_text = text;
}

TextProgram::~TextProgram() {
	close();
}

void
TextProgram::init() {

	// initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		reportError("SDL_Init() error: " + std::string(SDL_GetError()));
	}
	else {
		m_sdlInitialized = true;
	}

	// create SDL window
	m_window = SDL_CreateWindow("Text Program", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		m_windowWidth, m_windowHeight, SDL_WINDOW_OPENGL);

	if (m_window == nullptr) {
		reportError("SDL_CreateWindow() error: " + std::string(SDL_GetError()));
	}

	// initialize OpenGL
	SDL_GLContext glContext = SDL_GL_CreateContext(m_window);

	if (glContext == nullptr) {
		reportError("SDL_GL_CreateContext() error: " + std::string(SDL_GetError()));
	}

	GLenum glewErrorCode = glewInit();

	if (glewErrorCode != GLEW_OK) {
		reportError("glewInit() error.");
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetSwapInterval(1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);

	initVertexArray();
	initShaders();
	generateTexture();
}

void 
TextProgram::initVertexArray() {
	if (m_vaoID == 0) {
		glGenVertexArrays(1, &m_vaoID);
	}

	glBindVertexArray(m_vaoID);

	if (m_vboID == 0) {
		glGenBuffers(1, &m_vboID);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	// first triangle
	// top left
	m_vertices[0].setPosition(m_textureTopLeftPosX, m_textureTopLeftPosY);
	m_vertices[0].setColor(0, 255, 255, 255);
	m_vertices[0].setUV(0.0f, 1.0f);

	// bottom left
	m_vertices[1].setPosition(m_textureTopLeftPosX, m_textureTopLeftPosY - m_textureHeight);
	m_vertices[1].setColor(0, 255, 255, 255);
	m_vertices[1].setUV(0.0f, 0.0f);

	// bottom right
	m_vertices[2].setPosition(m_textureTopLeftPosX + m_textureWidth, m_textureTopLeftPosY - m_textureHeight);
	m_vertices[2].setColor(0, 150, 150, 255);
	m_vertices[2].setUV(1.0f, 0.0f);

	//second triangle
	// top left
	m_vertices[3].setPosition(m_textureTopLeftPosX, m_textureTopLeftPosY);
	m_vertices[3].setColor(0, 255, 255, 255);
	m_vertices[3].setUV(0.0f, 1.0f);

	// top right
	m_vertices[4].setPosition(m_textureTopLeftPosX + m_textureWidth, m_textureTopLeftPosY);
	m_vertices[4].setColor(0, 150, 150, 255);
	m_vertices[4].setUV(1.0f, 1.0f);

	// bottom right
	m_vertices[5].setPosition(m_textureTopLeftPosX + m_textureWidth, m_textureTopLeftPosY - m_textureHeight);
	m_vertices[5].setColor(0, 150, 150, 255);
	m_vertices[5].setUV(1.0f, 0.0f);

	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * sizeof(Vertex), m_vertices);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
TextProgram::initShaders() {
	m_shaderProgram.compileShaders("shaders/shader.vert", "shaders/shader.frag");
	m_shaderProgram.addAttribute("vertexPosition");
	m_shaderProgram.addAttribute("vertexColor");
	m_shaderProgram.addAttribute("vertexUV");
	m_shaderProgram.linkShaders();
}

void 
TextProgram::generateTexture() {
	if (TTF_Init() != 0) {
		reportError("TTF_Init() error: " + std::string(TTF_GetError()));
	}

	TTF_Font* font = TTF_OpenFont("fonts/freedom.ttf", 64);

	if (font == nullptr) {
		reportError("TTF_OpenFont() error: " + std::string(TTF_GetError()));
	}

	SDL_Color textColor = { 0,0,0,255 };

	SDL_Surface* textSurface = TTF_RenderText_Blended(font, m_text.c_str(), textColor);

	if (textSurface == nullptr) {
		reportError("TTF_RenderText_Blended() error: " + std::string(TTF_GetError()));
	}

	glGenTextures(1, &m_textureID);

	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textSurface->w, textSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, textSurface->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);

	SDL_FreeSurface(textSurface);
	textSurface = nullptr;

	TTF_CloseFont(font);
	font = nullptr;

	TTF_Quit();
}

void TextProgram::run() {
	init();

	bool mainLoop = true;

	while (mainLoop) {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				mainLoop = false;
				close();
			}
			else {
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				m_shaderProgram.use();

				// accessing uniform sampler
				glActiveTexture(GL_TEXTURE0);
				GLint textureLocation = m_shaderProgram.getUniformLocation("textSampler");
				glUniform1i(textureLocation, 0);

				// drawing
				glBindVertexArray(m_vaoID);

				glBindTexture(GL_TEXTURE_2D, m_textureID);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glBindTexture(GL_TEXTURE_2D, 0);
				
				glBindVertexArray(0);

				m_shaderProgram.unuse();

				SDL_GL_SwapWindow(m_window);
			}
		}
	}
}

void 
TextProgram::close() {
	if (m_vboID != 0) {
		glDeleteBuffers(1, &m_vboID);
	}
	
	if (m_window != nullptr) {
		SDL_DestroyWindow(m_window);
	}

	if (m_sdlInitialized) {
		SDL_Quit();
	}
}

void 
TextProgram::reportError(const std::string& error) {
	std::printf("%s\n\n", error.c_str());
	if (m_sdlInitialized) {
		SDL_Quit();
	}
	exit(94);
}