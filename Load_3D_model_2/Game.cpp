#include "Game.h"
#include "InputHandler.h"

#include <iostream>
#include <assert.h>

#include "GL\glew.h"
#include "GL\freeglut.h"

#include "IL\il.h"
#include "IL\ilu.h"
#include "IL\ilut.h"

Game::Game()
{
	std::cout << "konstructor Game()" << std::endl;

	screen_width = 800;
	screen_height = 600;

	running = true;
}


Game::~Game()
{

}

void Game::init()
{
	assert(2 < 5);

	/*
	��� �������� ����� �� ��������� :
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); - ������� ������ (������� �������� � ���������� ������ � ������������ �� �����)
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	���������� ������ ����������� ������ ����� - 8 ��� ��������, ��������, ������ � �����:
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	*/

	// initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) >= 0)
	{
		// SET ATTRIBUTE ONLY after initialize
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1); // on antialiasing sdl
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); //subsamples for each pixel

		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1); // set to 1 to require hardware acceleration

		window = SDL_CreateWindow("Chapter 1",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			screen_width, screen_height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | 0);

		//SDL_SetWindowOpacity(window, 0.0f); // make window transparent ( prozra4noe )

		if (window != 0)
		{
			glcontext = SDL_GL_CreateContext(window);
			std::cout << "SDL init " << std::endl;
		}
	}
	else
	{
		std::cout << "window create error " << std::endl;
		return; // sdl could not initialize
	}
	SDL_SetWindowFullscreen(window, 0); // ����������� ������� / ������������� ����� � ������� ��� ������ ��������� GL 

	glewExperimental = GL_TRUE; //��� ���������� � ��������������� ������� ����� ����� ����������.
	if (glewInit())// if all is init return 0 !! if not init return 1 and if() run
	{
		std::cout << "glew init error !" << std::endl;
	}

	ilInit(); // init image load library
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	ilEnable(IL_ORIGIN_SET); 
	// OpenGL have the 0.0 coordinate on the bottom y-axis, but images usually have 0.0 at the top of the y-axis.
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT); // 0, 0 textures in upper left side

	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	glViewport(0, 0, w, h); // ��������� ��� �������������� ������ ����������

	glEnable(GL_DEPTH_TEST); // �������� ���� �������
	//glEnable(GL_MULTISAMPLE); // for standart antialiasing in opengl
							  //glDepthFunc(GL_LESS); // �� ��������� ������� ( GL_LESS = �������� ���� �������� �������� ������� ������ ������������)
							  //glEnable(GL_DITHER);
							  //glEnable(GL_COLOR_LOGIC_OP);

							  // ��������� ���������� ������ ��� ���������� �����
	glEnable(GL_LINE_SMOOTH); // ����������� �����
	glEnable(GL_BLEND); // �������� ����������
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // ���������� ������ ����� ��� ���������� ����� 
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); // ������� ���� �������� ������� ����� ������� ��� �����
											// ����� ��������� ���������� ������ ��� ���������� �����
											// ��� ���������
											//glEnable(GL_POLYGON_SMOOTH);
											//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST); //GL_FASTEST = ������� ���� �������� ������� (��� �� ����� �������������� )
											//glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE); // ���������� ������ ����� ��� ���������� ������

											// �����

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // ������������� ������� ����
	glClearDepth(1.0);

	//glMatrixMode(GL_PROJECTION); ����� ������� �������.( GL_MODELVIEW- ��������� � ���������� �����(�� ���������))
	//glLoadIdentity(); // replace the current matrix(modelview ) with the user matrix (GL_PROJECTION)
	//glOrtho(0.0, 920.0, 900.0, 0.0, -1.0, 1.0);  - ���� 2� ����� ����������
	//glMatrixMode(GL_MODELVIEW);

	int ff;
	glGetIntegerv(GL_SAMPLE_BUFFERS, &ff); // ��������� ��� ��������(ff ������ = 1) � ��� ���(ff = 0)?
	//SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &ff);
	std::cout << "GL_CHECK = " << ff << std::endl;


	triangle.init();	 

}

void Game::handleEvents()
{
	InputHandler::Instance()->updateEvent();

}

void Game::update()
{
	triangle.update();
}

void Game::render()
{
	// �������� ��� ��������� �������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	triangle.render();

	glFlush(); // ����������� �������, ��������� ���������� ������ // �� ���� ����������
	//glFinish(); //���� ���� ��� ���������� ������� �� ����� ����������
	SDL_GL_SwapWindow(window);//���������� ������ ��� ������������ ���� �� ������.
	 
}


void Game::clean()
{
	std::cout << "cleaning " << std::endl;
	running = false;
	SDL_DestroyWindow(window);
	SDL_Quit();

}