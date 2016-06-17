#version 410

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;

uniform vec3 lightPosition;

in vec3 Position_VS_in;
in vec2 TexCoord_VS_in;
uniform vec3 Normal_VS_in;

out vec3 WorldPos_FS_in;
out vec2 TexCoord_FS_in;
out vec3 Normal_FS_in;

out vec3 tangentSurf2Player;
out vec3 tangentSurf2Light;

//for reflection purposes
out vec3 cubeMapVector;
uniform vec3 cameraPosition;

void tangentCalculation();
vec3 cubeMapVectorCalculation();

void main(){
	gl_Position=modelViewProjectionMatrix*vec4(Position_VS_in,1.0);
	
	WorldPos_FS_in=vec3(modelViewMatrix*vec4(Position_VS_in,1.0));
	TexCoord_FS_in=TexCoord_VS_in;
	Normal_FS_in=normalMatrix*Normal_VS_in;
	
	tangentCalculation();
	cubeMapVector=cubeMapVectorCalculation();
	
}

void tangentCalculation(){
	
	vec3 v1=cross(Normal_VS_in,vec3(1.0,0.0,0.0));
	vec3 v2=cross(Normal_VS_in,vec3(0.0,1.0,0.0));
	
	vec3 tangent;
	
	if(length(v1)>length(v2)){
		tangent=v1;
	}else{
		tangent=v2;
	}
	
	vec3 n=Normal_FS_in;
	vec3 t=normalMatrix*tangent;
	vec3 b=cross(n,t);
	
	mat3 tangentMatrix=mat3(t.x,b.x,n.x,t.y,b.y,n.y,t.z,b.z,n.z);
	
	vec3 surf2light=normalize(lightPosition-WorldPos_FS_in);
	tangentSurf2Light=tangentMatrix*surf2light;
	
	vec3 surf2player=normalize(-WorldPos_FS_in);
	tangentSurf2Player=tangentMatrix*surf2player;
	
}

vec3 cubeMapVectorCalculation(){
	//vec3 incident=normalize(vec3(modelMatrix*vec4(Position_VS_in,1.0))-cameraPosition);
	vec3 incident=normalize(vec3(modelViewMatrix*vec4(Position_VS_in,1.0)));
	//vec3 worldNormal=normalize(vec3(modelMatrix*vec4(Normal_VS_in,1.0)));
	vec3 worldNormal=normalize(vec3(normalMatrix*Normal_VS_in));
	vec3 reflectionVec=reflect(incident,worldNormal);
	return reflectionVec; 
}