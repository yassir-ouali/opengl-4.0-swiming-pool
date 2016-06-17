
#include "stdafx.h"
#include "definitions.h"
#include "skybox.h"


Skybox::Skybox()
{
	
}

Skybox::~Skybox()
{
	glDeleteTextures(6, skybox);
}

void Skybox::initSkybox()
{
	skybox[FRONT_FACE] = loadTexture("images/front.bmp");
	skybox[BACK_FACE] = loadTexture("images/back.bmp");
	skybox[RIGHT_FACE] = loadTexture("images/right.bmp");
	skybox[LEFT_FACE] = loadTexture("images/left.bmp");
	skybox[TOP_FACE] = loadTexture("images/top.bmp");
	skybox[BOTTOM_FACE] = loadTexture("images/bottom.bmp");
}

void Skybox::drawSkybox(float size)
{
	bool b1 = glIsEnabled(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	//back face
	glBindTexture(GL_TEXTURE_2D, skybox[BACK_FACE]);
	glBegin(GL_QUADS);      //and draw a face
	glTexCoord2f(0, 0);      //use the correct texture coordinate
	glVertex3f(size / 2, size / 2, size / 2);       //and a vertex
	glTexCoord2f(1, 0);      //and repeat it...
	glVertex3f(-size / 2, size / 2, size / 2);
	glTexCoord2f(1, 1);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(size / 2, -size / 2, size / 2);
	glEnd();

	//left face
	glBindTexture(GL_TEXTURE_2D, skybox[LEFT_FACE]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-size / 2, size / 2, size / 2);
	glTexCoord2f(1, 0);
	glVertex3f(-size / 2, size / 2, -size / 2);
	glTexCoord2f(1, 1);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glEnd();
	
	//front face
	glBindTexture(GL_TEXTURE_2D, skybox[FRONT_FACE]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(size / 2, size / 2, -size / 2);
	glTexCoord2f(0, 0);
	glVertex3f(-size / 2, size / 2, -size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glTexCoord2f(1, 1);
	glVertex3f(size / 2, -size / 2, -size / 2);
	glEnd();
	
	//right face
	glBindTexture(GL_TEXTURE_2D, skybox[RIGHT_FACE]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(size / 2, size / 2, -size / 2);
	glTexCoord2f(1, 0);
	glVertex3f(size / 2, size / 2, size / 2);
	glTexCoord2f(1, 1);
	glVertex3f(size / 2, -size / 2, size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(size / 2, -size / 2, -size / 2);
	glEnd();

	//bottom face
	glBindTexture(GL_TEXTURE_2D, skybox[BOTTOM_FACE]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);
	glVertex3f(size / 2, -size / 2, size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glTexCoord2f(0, 0);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glTexCoord2f(1, 0);
	glVertex3f(size / 2, -size / 2, -size / 2);
	glEnd();


	//top face
	glBindTexture(GL_TEXTURE_2D, skybox[TOP_FACE]);
	glBegin(GL_QUADS);                      
	glTexCoord2f(1, 0);
	glVertex3f(size / 2, size / 2, size / 2);
	glTexCoord2f(0, 0);
	glVertex3f(-size / 2, size / 2, size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-size / 2, size / 2, -size / 2);
	glTexCoord2f(1, 1);
	glVertex3f(size / 2, size / 2, -size / 2);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_LIGHTING);  //turn everything back, which we turned on, and turn everything off, which we have turned on.
	glEnable(GL_DEPTH_TEST);
	if (!b1)
		glDisable(GL_TEXTURE_2D);
}