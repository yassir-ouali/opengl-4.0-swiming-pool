#ifndef MESH_H
#define MESH_H
#include "stdafx.h"
#include <iostream>
#include "meshData.h"
#include <GL/glew.h>
#include <vector>
#include <string>

class mesh{
	std::vector<vertexData> data;
	std::vector<textureData> textures;
	std::vector<unsigned int> indices;
	unsigned int VBO;
	unsigned int IND;
	public:
		mesh(std::vector<vertexData>* vd,std::vector<unsigned int>* id,std::vector<textureData>* td=NULL);
		~mesh();
		void draw(unsigned int programId);
};
#endif
