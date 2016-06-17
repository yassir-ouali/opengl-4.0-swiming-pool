#include "stdafx.h"
#include "pipeline.h"

pipeline::pipeline()
{
	modelMatrix.push_back(glm::mat4(1.0));
	viewMatrix.push_back(glm::mat4(1.0));
	projectionMatrix.push_back(glm::mat4(1.0));

	modelViewMatrix = glm::mat4(1.0);
	modelViewProjectionMatrix = glm::mat4(1.0);
	normalMatrix = glm::mat3(1.0);
	matricesReady = true;
	currentMatrix = 0;
}

void pipeline::loadIdentity()
{
	if (currentMatrix == MODEL_MATRIX || currentMatrix == VIEW_MATRIX)
	{
		modelMatrix[modelMatrix.size() - 1] = glm::mat4(1.0);
		viewMatrix[modelMatrix.size() - 1] = glm::mat4(1.0);
	}
	else
		projectionMatrix[viewMatrix.size() - 1] = glm::mat4(1.0);
	matricesReady = false;
}

void pipeline::matrixMode(int m)
{
	if (m == MODEL_MATRIX || m == VIEW_MATRIX || m == PROJECTION_MATRIX)
		currentMatrix = m;
}

//modelview
void pipeline::translate(float x, float y, float z)
{
	if (currentMatrix == MODEL_MATRIX)
		modelMatrix[modelMatrix.size() - 1] = glm::translate(/*glm::mat4(1.0f)*/modelMatrix[modelMatrix.size() - 1], glm::vec3(x, y, z));//modelMatrix[modelMatrix.size()-1]*=glm::translate(x,y,z);
	else if (currentMatrix == VIEW_MATRIX)
		viewMatrix[viewMatrix.size() - 1] = glm::translate(viewMatrix[viewMatrix.size() - 1], glm::vec3(-x, -y, -z));//viewMatrix[viewMatrix.size()-1]*=glm::translate(-x,-y,-z);
	matricesReady = false;
}

void pipeline::scale(float x, float y, float z)
{
	if (currentMatrix == MODEL_MATRIX)
		modelMatrix[modelMatrix.size() - 1] = glm::scale(modelMatrix[modelMatrix.size() - 1], glm::vec3(x, y, z));//modelMatrix[modelMatrix.size()-1]*=glm::scale(x,y,z);
	else if (currentMatrix == VIEW_MATRIX)
		viewMatrix[viewMatrix.size() - 1] = glm::scale(viewMatrix[viewMatrix.size() - 1], glm::vec3(x, y, z));//viewMatrix[viewMatrix.size()-1]*=glm::scale(x,y,z);
	matricesReady = false;
}

void pipeline::scale(float v)
{
	if (currentMatrix == MODEL_MATRIX)
		modelMatrix[modelMatrix.size() - 1] = glm::scale(modelMatrix[modelMatrix.size() - 1], glm::vec3(v, v, v));//modelMatrix[modelMatrix.size()-1]*=glm::scale(v,v,v);
	else if (currentMatrix == VIEW_MATRIX)
		viewMatrix[viewMatrix.size() - 1] = glm::scale(viewMatrix[viewMatrix.size() - 1], glm::vec3(v, v, v));//viewMatrix[viewMatrix.size()-1]*=glm::scale(v,v,v);
	matricesReady = false;
}

void pipeline::rotateX(float angle)
{
	if (currentMatrix == MODEL_MATRIX)
		modelMatrix[modelMatrix.size() - 1] = glm::rotate(modelMatrix[modelMatrix.size() - 1], glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));//modelMatrix[modelMatrix.size()-1]*=glm::rotate(angle,1.0f,0.0f,0.0f);
	else if (currentMatrix == VIEW_MATRIX)
		viewMatrix[viewMatrix.size() - 1] = glm::rotate(viewMatrix[viewMatrix.size() - 1], glm::radians(-angle), glm::vec3(1.0f, 0.0f, 0.0f)); //viewMatrix[viewMatrix.size() - 1] *= glm::rotate(-angle, 1.0f, 0.0f, 0.0f);
	matricesReady = false;
}

void pipeline::rotateY(float angle)
{
	if (currentMatrix == MODEL_MATRIX)
		modelMatrix[modelMatrix.size() - 1] = glm::rotate(modelMatrix[modelMatrix.size() - 1], glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));//modelMatrix[modelMatrix.size()-1]*=glm::rotate(angle,0.0f,1.0f,0.0f);
	else if (currentMatrix == VIEW_MATRIX)
		viewMatrix[viewMatrix.size() - 1] = glm::rotate(viewMatrix[viewMatrix.size() - 1], glm::radians(-angle), glm::vec3(0.0f, 1.0f, 0.0f));//viewMatrix[viewMatrix.size()-1]*=glm::rotate(-angle,0.0f,1.0f,0.0f);
	matricesReady = false;
}

void pipeline::rotateZ(float angle)
{
	if (currentMatrix == MODEL_MATRIX)
		modelMatrix[modelMatrix.size() - 1] = glm::rotate(modelMatrix[modelMatrix.size() - 1], angle, glm::vec3(0.0f, 0.0f, 1.0f));//modelMatrix[modelMatrix.size()-1]*=glm::rotate(angle,0.0f,0.0f,1.0f);
	else if (currentMatrix == VIEW_MATRIX)
		viewMatrix[viewMatrix.size() - 1] = glm::rotate(viewMatrix[viewMatrix.size() - 1], -angle, glm::vec3(0.0f, 0.0f, 1.0f));//viewMatrix[viewMatrix.size()-1]*=glm::rotate(-angle,0.0f,0.0f,1.0f);
	matricesReady = false;
}

//projection
void pipeline::ortho(float left, float right, float bottom, float top, float near, float far) //==glOrtho
{
	projectionMatrix[projectionMatrix.size() - 1] = glm::ortho(left, right, bottom, top, near, far);
	matricesReady = false;
}

void pipeline::perspective(float angle, float aRatio, float near, float far)	//==gluPerspective
{
	projectionMatrix[projectionMatrix.size() - 1] = glm::perspective(angle, aRatio, near, far);
	matricesReady = false;
}

//getters
glm::mat4 pipeline::getModelMatrix()
{
	return modelMatrix[modelMatrix.size() - 1];
}

glm::mat4 pipeline::getViewMatrix()
{
	return viewMatrix[viewMatrix.size() - 1];
}

glm::mat4 pipeline::getModelViewMatrix()
{
	if (!matricesReady)
		return viewMatrix[viewMatrix.size() - 1] * modelMatrix[modelMatrix.size() - 1];
	else
		return modelViewMatrix;
}

glm::mat4 pipeline::getProjectionMatrix()
{
	return projectionMatrix[projectionMatrix.size() - 1];
}

glm::mat4 pipeline::getModelViewProjectionMatrix()
{
	if (!matricesReady)
		return viewMatrix[viewMatrix.size() - 1] * modelMatrix[modelMatrix.size() - 1] * projectionMatrix[projectionMatrix.size() - 1];
	else
		return modelViewProjectionMatrix;
}

//GLSL
void pipeline::updateMatrices(unsigned int programId)
{
	if (!matricesReady)
	{
		modelViewMatrix = viewMatrix[viewMatrix.size() - 1] * modelMatrix[modelMatrix.size() - 1];
		modelViewProjectionMatrix = projectionMatrix[projectionMatrix.size() - 1] * viewMatrix[viewMatrix.size() - 1] * modelMatrix[modelMatrix.size() - 1];
		normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelViewMatrix)));
		//normalMatrix = glm::mat3(modelViewMatrix);
	}
	glUniformMatrix4fv(glGetUniformLocation(programId, "modelMatrix"), 1, GL_FALSE, &modelMatrix[modelMatrix.size() - 1][0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId, "viewMatrix"), 1, GL_FALSE, &viewMatrix[viewMatrix.size() - 1][0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId, "projectionMatrix"), 1, GL_FALSE, &projectionMatrix[projectionMatrix.size()-1][0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId, "modelViewMatrix"), 1, GL_FALSE, &modelViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId, "modelViewProjectionMatrix"), 1, GL_FALSE, &modelViewProjectionMatrix[0][0]);
	glUniformMatrix3fv(glGetUniformLocation(programId, "normalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
}
