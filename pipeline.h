#ifndef PIPELINE_H
#define PIPELINE_H
#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "GL\glew.h"

enum matrixModes{
	MODEL_MATRIX=0,
	VIEW_MATRIX,
	PROJECTION_MATRIX
};

class pipeline{
	int currentMatrix;
	//members
	std::vector<glm::mat4> modelMatrix;
	std::vector<glm::mat4> viewMatrix;
	std::vector<glm::mat4> projectionMatrix;
	
	
	bool matricesReady;
	glm::mat4 modelViewMatrix;						//only to save time for vertexshader
	glm::mat4 modelViewProjectionMatrix;	//only to save time for vertexshader
	glm::mat3 normalMatrix;							  //only to save time for vertexshader
	public:
		
		pipeline();
		
		void loadIdentity();
		void matrixMode(int m);
		
		//modelview
		void translate(float x,float y,float z);
		void scale(float x,float y,float z);
		void scale(float v);
		void rotateX(float angle);
		void rotateY(float angle);
		void rotateZ(float angle);
		
		//projection
		void ortho(float left,float right,float bottom,float top,float near,float far); //==glOrtho
		void perspective(float angle,float aRatio,float near,float far);	//==gluPerspective
		
		//getters
		glm::mat4 getModelMatrix();
		glm::mat4 getViewMatrix();
		glm::mat4 getModelViewMatrix();
		glm::mat4 getProjectionMatrix();
		glm::mat4 getModelViewProjectionMatrix();
		
		//GLSL
		void updateMatrices(unsigned int programId);
};

#endif
