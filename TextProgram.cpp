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
		m_windowDimensions.x, m_windowDimensions.y, SDL_WINDOW_OPENGL);

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

				SDL_GL_SwapWindow(m_window);
			}
		}
	}
}

void 
TextProgram::close() {
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