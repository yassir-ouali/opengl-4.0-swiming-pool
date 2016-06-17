#version 410


uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

in vec3 position;
in vec2 waterCoord;

out vec2 waterCoord2;
out vec4 waterCoordRefr;

out vec3 pos;
out vec3 normal;

out vec3 tangentSpaceSurf2Light;
out vec3 tangentSpaceSurf2Viewer;

out vec3 lightPosition;


//for reflection purposes
out vec3 cubeMapVector;
uniform vec3 cameraPosition;

vec3 cubeMapVectorCalculation();

void main()
{
	gl_Position=modelViewProjectionMatrix*vec4(position,1.0);
	//gl_Position=gl_ModelViewProjectionMatrix*vec4(position,1.0);
	waterCoord2=waterCoord;
	waterCoordRefr=gl_Position;
	pos=vec3(modelViewMatrix*vec4(position,1.0));
	vec3 maNormal=vec3(0.0,1.0,0.0);
	normal=normalMatrix*maNormal;

	vec3 tangent;

	vec3 v1=cross(maNormal,vec3(0.0,0.0,1.0));
	vec3 v2=cross(maNormal,vec3(0.0,1.0,0.0));

	if(length(v1)>length(v2)){
		tangent=v1;
	}else{
		tangent=v2;
	}

	vec3 n =normalize(normal);
	vec3 t =normalize(normalMatrix*tangent);
	vec3 b =cross(n,t);

	mat3 matrice=mat3(t.x,b.x,n.x,t.y,b.y,n.y,t.z,b.z,n.z);

	lightPosition=position+vec3(0.0,10.0,0.0);

	tangentSpaceSurf2Light=matrice*normalize(lightPosition-position);
	tangentSpaceSurf2Viewer=matrice*normalize(-pos);

	//reflection
	cubeMapVector=cubeMapVectorCalculation();
}

vec3 cubeMapVectorCalculation(){
	vec3 incident=normalize(vec3(modelMatrix*vec4(position,1.0))-cameraPosition);
	vec3 worldNormal=normalize(vec3(modelMatrix*vec4(normal,1.0)));
	vec3 reflectionVec=reflect(incident,worldNormal);
	return reflectionVec; 
}