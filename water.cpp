#include "stdafx.h"
#include "water.h"

Water::Water()
{

}
Water::Water(float size, shader *program, camera* cam)
{
	this->program = program;
	this->size = size;
	this->cam = cam;

	//unsigned int vbo, indiceBuff;
	float quad[] = {
		-size, 0.0, size,
		-size, 0.0, -size,
		size, 0.0, -size,
		size, 0.0, size
	};

	/*float quad[] = {
		-size / 2, size / 2, 0.0f,
		-size / 2, -size / 2, 0.0f,
		size / 2, size / 2, 0.0f,
		size / 2, -size / 2, 0.0f
	};*/
	int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	//----- water texture coordinate
	float textCoord[] = {
		1.0,0.0,
		1.0,1.0,
		0.0,1.0,
		0.0,0.0
	};

	//-----Create the vertex array and bind it
	//	glGenVertexArrays(1, &vao);
	//	glBindVertexArray(vao);

	//-----create the vertex buffer and bind it
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//-----allocate space int vbo and push data to it
	glBufferData(GL_ARRAY_BUFFER, 5 * 4 * sizeof(float), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3* 4 * sizeof(float), quad);
	glBufferSubData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(float), 2 * 4 * sizeof(float), textCoord);
	glGenBuffers(1, &indiceBuff);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuff);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(int), indices, GL_STATIC_DRAW);



	

}
Water::~Water()
{
}
void Water::draw(pipeline& trf,unsigned int heightMap, unsigned int refraction, unsigned int normalMap,unsigned int skybox,float delta)
{
	//glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuff);

	glUseProgram(program->getProgramId());

	//-----binding data to shaders
	GLint posLoc = glGetAttribLocation(program->getProgramId(), "Position_VS_in");
	GLint waterLoc = glGetAttribLocation(program->getProgramId(), "TexCoord_VS_in");

	glUniform3f(glGetUniformLocation(program->getProgramId(), "Normal_VS_in"), 0.0f, 1.0f, 0.0f);

	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(waterLoc, 2, GL_FLOAT, GL_FALSE, 0, ((char*)NULL + (3 * 4 * sizeof(float))));
	//-----activate the attributes
	glEnableVertexAttribArray(posLoc);
	glEnableVertexAttribArray(waterLoc);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
	glUniform1i(glGetUniformLocation(program->getProgramId(), "skybox"), 3);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, heightMap);
	glUniform1i(glGetUniformLocation(program->getProgramId(), "heightMap"), 2);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refraction);
	glUniform1i(glGetUniformLocation(program->getProgramId(), "refraction"), 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, normalMap);
	glUniform1i(glGetUniformLocation(program->getProgramId(), "normalMap"), 0);

	//materails
	glUniform3f(glGetUniformLocation(program->getProgramId(), "mambient"), 1.0, 1.0, 1.0);
	glUniform3f(glGetUniformLocation(program->getProgramId(), "mdiffuse"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(program->getProgramId(), "mspecular"), 1.0f, 1.0f, 1.0f);

	//light position:
	glUniform3f(glGetUniformLocation(program->getProgramId(), "lightPosition"), 0.0f, 0.0f, 0.0f);
	//light

	glUniform3f(glGetUniformLocation(program->getProgramId(), "ambient"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(program->getProgramId(), "diffuse"), 0.3f, 0.3f, 0.3f);
	glUniform3f(glGetUniformLocation(program->getProgramId(), "specular"),2.0f, 2.9f, 2.0f);

	glUniform3f(glGetUniformLocation(program->getProgramId(), "cameraPosition"), cam->getLocation().x, cam->getLocation().y, cam->getLocation().z);

	glUniform1f(glGetUniformLocation(program->getProgramId(), "shininess"),52.0f);

	glUniform1f(glGetUniformLocation(program->getProgramId(), "dt"), delta);
	
	trf.updateMatrices(program->getProgramId());

	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_PATCHES, 6, GL_UNSIGNED_INT, 0);
	//glPatchParameteri(GL_PATCH_VERTICES,3);
	//glDrawArrays(GL_PATCHES,0,6);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
}
void Water::setSize(float s)
{
	size = s;
}
float Water::getSize()
{
	return size;
}
void Water::setColors(float c[])
{
	colors = c;
}
float* Water::getColors()
{
	return colors;
}