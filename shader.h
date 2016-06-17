#ifndef SHADER_H
#define SHADER_H
#include "stdafx.h"
#include "GL\glew.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class shader{
	unsigned int vs,fs,tcs,tes,program;
	void loadFile(const char* fn,std::string& str);
	unsigned int loadShader(std::string& source,unsigned int mode);
	public:
		shader(const char* vss,const char* fss);
		shader(const char* vss, const char* fss,const char* tsc,const char* tes);
		virtual ~shader();
		void useShader();
		void delShader();
		unsigned int getProgramId();
};

#endif
