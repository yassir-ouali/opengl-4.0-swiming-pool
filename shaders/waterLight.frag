#version 410

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;

in vec3 WorldPos_FS_in;
in vec2 TexCoord_FS_in;
in vec3 Normal_FS_in;


uniform vec3 lightPosition;
uniform float dt;
uniform sampler2D normalMap;
uniform sampler2D refraction;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

vec3 lightCalculation();
vec3 ambientCalculation();
vec3 diffuseCalculation();
vec3 specularCalculation();

in vec3 tangentSurf2Player;
in vec3 tangentSurf2Light;

in vec3 cubeMapVector;
uniform samplerCube skybox;

void main(){
	//calculate uv coordinate for refraction:
	vec4 waterCoordRefr = projectionMatrix*vec4(WorldPos_FS_in,1.0);
	vec2 refrCoord=(waterCoordRefr.xy/waterCoordRefr.w)/2 +0.5;
	
	gl_FragColor=vec4(lightCalculation(),1.0)+textureCube(skybox,cubeMapVector)*0.5+texture2D(refraction,refrCoord)*0.4;
}

vec3 ambientCalculation(){
	//ambient
	return ambient;
}

vec3 diffuseCalculation(){
	//diffuse
	vec3 surf2light=normalize(tangentSurf2Light);
	float diffuseContribution=max(0.0,dot(normalize(vec3(texture2D(normalMap,TexCoord_FS_in+vec2(0.0,dt)))*2-1),surf2light));
	return diffuseContribution*diffuse;
}

vec3 specularCalculation(){
	//specular
	vec3 surf2light=normalize(tangentSurf2Light);
	vec3 surf2player=normalize(tangentSurf2Player);
	vec3 reflect=reflect(-surf2light,normalize(vec3(texture2D(normalMap,TexCoord_FS_in+vec2(0.0,dt))*2-1)));
	float specularContribution=pow(max(0.0,dot(reflect,surf2player)),shininess);
	return specularContribution*specular;

}
vec3 lightCalculation(){

	//attenuation
	float dist=length(WorldPos_FS_in-lightPosition);
    float att=1.0/(1.0+0.1*dist+0.01*dist*dist);
	
	vec3 ambientTmp=ambientCalculation();
	vec3 diffuseTmp=diffuseCalculation();
	vec3 specularTmp=specularCalculation();
	
	return (ambientTmp+diffuseTmp+specularTmp)*att;
}