#ifndef SKYBOX_H
#define SKYBOX_H

#include "stdafx.h"

class Skybox{
	public:
		enum faces{FRONT_FACE=0,BACK_FACE,TOP_FACE,BOTTOM_FACE,RIGHT_FACE,LEFT_FACE};
		unsigned int skybox[6];
		Skybox();
		virtual ~Skybox();
		void initSkybox();
		void drawSkybox(float size);
};

#endif