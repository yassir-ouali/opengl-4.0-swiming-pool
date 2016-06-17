varying vec3 outColor;
varying vec3 vertexVector;
uniform samplerCube cubeMap;

void main()
{
	
	gl_FragColor=textureCube(cubeMap,vertexVector);
	//gl_FragColor=vec4(1.0,0.0,0.0,1.0);
}
