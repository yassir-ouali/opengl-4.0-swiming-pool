#include "stdafx.h"
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <GL/glew.h>
#include "camera.h"
#include "vector3d.h"
#include "skybox.h"
#include "water.h"
#include "definitions.h"
#include "sceneLoader.h"
#include "shader.h"
#include "pipeline.h"
#include "mesh.h"

//Skybox skybox;
Water water;
unsigned int normalMap, heightMap, heightMapDepth, refraction, refractionDepth;
unsigned int fboRefraction, fboReflection,cubeMap;
float delta = 0.0f;
meshLoader* scene;
shader *assimpShader, *waterProgram, *skyboxShader;
mesh* skybox;
pipeline trf;
camera cam;

void init()
{
	
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glEnable(GL_DEPTH_TEST);

	cam.setLocation(vector3d(0, 5, 5));

	trf.matrixMode(matrixModes::PROJECTION_MATRIX);
	glMatrixMode(GL_PROJECTION);

	trf.loadIdentity();
	glLoadIdentity();

	trf.perspective(45, 640.0 / 480.0, 0.5, 500.0);
	gluPerspective(45, 640.0 / 480.0, 0.5, 500.0);

	//skybox.initSkybox();
	scene = new meshLoader("test.obj");
	assimpShader = new shader("shaders/vertex.vs", "shaders/fragment.frag");
	waterProgram = new shader("shaders/water.vs", "shaders/water.frag","shaders/water.tcs","shaders/water.tes");
	//waterProgram = new shader("shaders/waterLight.vs", "shaders/waterLight.frag");
	skyboxShader = new shader("shaders/skyboxShader.vs","shaders/skyboxShader.frag");
	water = Water(5,waterProgram,&cam);

	normalMap = loadTexture("images/normal.jpg");

	heightMap = loadTexture("images/heightMap.jpg");

	refraction = createTexture(640, 480);

	refractionDepth = createTexture(640, 480, true);

	glGenFramebuffers(1, &fboRefraction);
	glGenFramebuffers(1, &fboReflection);
	glBindFramebuffer(GL_FRAMEBUFFER, fboRefraction);
	
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, refraction, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, refractionDepth, 0);
		int e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (e != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR FRAMEBUFFER"<< e << std::endl;
		}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	/*
	heightMap = createTexture(640, 480);

	heightMapDepth = createTexture(640, 480, true);

	
	glBindFramebuffer(GL_FRAMEBUFFER, fboReflection);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, heightMap, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, heightMapDepth, 0);
	int e2 = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (e2 != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR FRAMEBUFFER" << e2 << std::endl;
	}*/
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//create the cube map
	{
		std::vector<unsigned int> indices;
		std::vector<vertexData> vertices;
		vertexData tmp;
		tmp.position.change(-1.0, 1.0, 1.0);
		tmp.color.change(1.0, 0.0, 0.0);
		vertices.push_back(tmp);

		tmp.position.change(-1.0, -1.0, 1.0);
		tmp.color.change(0.0, 1.0, 0.0);
		vertices.push_back(tmp);

		tmp.position.change(1.0, -1.0, 1.0);
		tmp.color.change(0.0, 0.0, 1.0);
		vertices.push_back(tmp);

		tmp.position.change(1.0, 1.0, 1.0);
		tmp.color.change(1.0, 1.0, 0.0);
		vertices.push_back(tmp);

		tmp.position.change(-1.0, 1.0, -1.0);
		tmp.color.change(1.0, 0.0, 1.0);
		vertices.push_back(tmp);

		tmp.position.change(-1.0, -1.0, -1.0);
		tmp.color.change(1.0, 0.0, 0.0);
		vertices.push_back(tmp);

		tmp.position.change(1.0, -1.0, -1.0);
		tmp.color.change(1.0, 1.0, 1.0);
		vertices.push_back(tmp);

		tmp.position.change(1.0, 1.0, -1.0);
		tmp.color.change(0.0, 1.0, 1.0);
		vertices.push_back(tmp);


		//front face
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(2);


		//left face
		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(6);

		indices.push_back(3);
		indices.push_back(7);
		indices.push_back(6);

		//back face
		indices.push_back(4);
		indices.push_back(5);
		indices.push_back(6);

		indices.push_back(4);
		indices.push_back(7);
		indices.push_back(6);

		//right face
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(5);

		indices.push_back(0);
		indices.push_back(4);
		indices.push_back(5);


		//top face
		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(4);

		indices.push_back(3);
		indices.push_back(4);
		indices.push_back(7);


		//bottom face
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(6);

		indices.push_back(1);
		indices.push_back(5);
		indices.push_back(6);
		skybox = new mesh(&vertices, &indices);
	}

	std::string filename[6];
	filename[0] = "images/right.bmp";
	filename[1] = "images/left.bmp";
	filename[2] = "images/top.bmp";
	filename[3] = "images/bottom.bmp";
	filename[4] = "images/front.bmp";
	filename[5] = "images/back.bmp";
	cubeMap = loadCubemap(filename);

}

void display()
{
	if (delta < 100){
		delta += 0.001;
	}
	else{
		delta = 0;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	trf.matrixMode(matrixModes::MODEL_MATRIX);

	trf.loadIdentity();

	trf.matrixMode(matrixModes::VIEW_MATRIX);

	trf.loadIdentity();

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	
	glUseProgram(0);
	
	glDisable(GL_DEPTH_TEST);

	cam.Control(trf);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	trf.matrixMode(MODEL_MATRIX);
		skyboxShader->useShader();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
			glUniform1i(glGetUniformLocation(skyboxShader->getProgramId(), "cubeMap"), 0);
			trf.updateMatrices(skyboxShader->getProgramId());
			skybox->draw(skyboxShader->getProgramId());
		skyboxShader->delShader();
	cam.UpdateCamera(trf);
	
	glEnable(GL_DEPTH_TEST);
		assimpShader->useShader();
		trf.updateMatrices(assimpShader->getProgramId());
		scene->draw(assimpShader->getProgramId());
		assimpShader->delShader();

	glBindFramebuffer(GL_FRAMEBUFFER, fboRefraction);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CLIP_PLANE0);
		assimpShader->useShader();
		trf.updateMatrices(assimpShader->getProgramId());
		scene->draw(assimpShader->getProgramId());
		assimpShader->delShader();

	double plane[] = { 0.0, -1.0, 0.0, 0.5 };
	glClipPlane(GL_CLIP_PLANE0, plane);

	glDisable(GL_CLIP_PLANE0);
	//draw the 3D mesh

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	trf.translate(0.0,-1.0,0);
	water.draw(trf, heightMap, refraction, normalMap, cubeMap, delta);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );

}

void gameLoop()
{
	bool running = true;
	Uint32 start;
	SDL_Event event;

	while (running)
	{
		start = SDL_GetTicks();
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				cam.mouseIn(true);
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_p)
				{
					cam.mouseIn(false);
					break;
				}
			}
		}
		display();
		SDL_GL_SwapBuffers();
		if (1000 / 30 > (SDL_GetTicks() - start))
			SDL_Delay(1000 / 30 - (SDL_GetTicks() - start));//-----------------------
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface* screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE | SDL_OPENGL);
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}
	
	init();
	
	
	gameLoop();
	//skybox.~Skybox();
	scene->~meshLoader();
	waterProgram->~shader();
	assimpShader->~shader();
	SDL_Quit();
	return 0;
}