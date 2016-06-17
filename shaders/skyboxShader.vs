attribute vec3 vertex;
attribute vec3 color;

varying vec3 outColor;
varying vec3 vertexVector;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

void main()
{
	gl_Position=modelViewProjectionMatrix*vec4(vertex,1.0);
	outColor=color;
	vertexVector=vertex;
}
