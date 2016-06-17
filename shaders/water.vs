#version 410

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;

in vec3 Position_VS_in;
in vec2 TexCoord_VS_in;
uniform vec3 Normal_VS_in;

out vec3 WorldPos_CS_in;
out vec2 TexCoord_CS_in;
out vec3 Normal_CS_in;


void main()
{
	WorldPos_CS_in=vec3(modelViewMatrix*vec4(Position_VS_in,1.0));
	TexCoord_CS_in=TexCoord_VS_in;
	Normal_CS_in=normalMatrix*Normal_VS_in;	
}
