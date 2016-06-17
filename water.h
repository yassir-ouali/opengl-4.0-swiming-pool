#ifndef WATER_H
#define WATER_H

#include "stdafx.h"
#include "GL\glew.h"
#include "shader.h"
#include "pipeline.h"
#include "camera.h"

class Water
{
public:
	Water();
	Water(float size, shader *program,camera* cam);
	virtual ~Water();
	void draw(pipeline& trf, unsigned int heightMap, unsigned int refraction, unsigned int normalMap, unsigned int skybox, float delta);
	float getSize();
	void setSize(float size);
	float* getColors();
	void setColors(float colors[]);
private:
	float size=0;
	float* colors;
	shader *program;
	unsigned int vbo,vao;
	unsigned int indiceBuff;
	camera* cam;
};

#endif