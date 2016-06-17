#version 410
uniform sampler2D img;//refraction
uniform sampler2D img2;//normal map
uniform sampler2D img3;//reflection

in vec2 waterCoord2;
in vec4 waterCoordRefr;

in vec3 pos;
in vec3 normal;

in vec3 lightPosition;

uniform vec3 mambient;
uniform vec3 mdiffuse;
uniform vec3 mspecular;

uniform vec3 lambient;
uniform vec3 ldiffuse;
uniform vec3 lspecular;

uniform float shininess;


in vec3 tangentSpaceSurf2Light;
in vec3 tangentSpaceSurf2Viewer;

uniform float dt;

//used for reflection
in vec3 cubeMapVector;
uniform samplerCube skybox;

void main()
{

	//calculate uv coordinate :
	vec2 ndc=(waterCoordRefr.xy/waterCoordRefr.w)/2 +0.5;
	vec2 refrCoord=ndc;
	vec2 reflCoord=vec2(ndc.x,-ndc.y);

	vec2 normalCoord=waterCoord2;
	vec3 texturColor1=vec3(texture2D(img,refrCoord));
	vec3 texturColor2=vec3(texture2D(img3,reflCoord));
	vec3 texturColor=mix(texturColor1,texturColor2,0.5);

	float dist=length(pos-lightPosition);   //distance from light-source to surface
    float att=1.0/(1.0+0.1*dist+0.001*dist*dist);    //attenuation (constant,linear,quadric)

	//ambient
	vec3 ambient=texturColor1*lambient;

	//diffuse
	vec3 surf2light=normalize(tangentSpaceSurf2Light);
	
	//vec3 norm=normalize(normal);
	vec3 norm=normalize(vec3(texture2D(img2,normalCoord+vec2(0.0,dt)))*2-1.0);
	float dcont=max(0.0,dot(normal,surf2light));
	vec3 diffuse=dcont*(texturColor1*ldiffuse);

	//specular
	vec3 surf2viewer=normalize(tangentSpaceSurf2Viewer);
	vec3 reflect=reflect(-surf2light,norm);
	float scont=pow(max(0.0,dot(surf2viewer,reflect)),shininess);
	vec3 specular=scont*lspecular*mspecular;
	
	//gl_FragColor=textureCube(skybox,cubeMapVector);
	gl_FragColor=vec4(texturColor1,1.0)*0.3+vec4((ambient+diffuse+specular)*att,1.0)*0.5+textureCube(skybox,cubeMapVector)*0.2;
	//gl_FragColor=mix(vec4(texturColor1,1.0),vec4((ambient+diffuse+specular)*att,1.0),0.5);
}