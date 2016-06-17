#ifndef CAMERA_H
#define CAMERA_H
#include "stdafx.h"
#include <cmath>
#include <iostream>
#include <windows.h>
#include <SDL.h>
#include "vector3d.h"
#include "definitions.h"
#include "pipeline.h"

class camera{
	vector3d loc;
	float camPitch, camYaw;
	float movevel;
	float mousevel;
	bool mi, ismoved;
	void lockCamera();
	void moveCamera(float dir);
	void moveCameraUp(float dir);
public:
	camera();
	camera(vector3d loc);
	camera(vector3d loc, float yaw, float pitch);
	camera(vector3d loc, float yaw, float pitch, float mv, float mov);
	void Control(pipeline& trf);
	void UpdateCamera(pipeline& trf);
	vector3d getVector();
	vector3d getLocation();
	float getPitch();
	float getYaw();
	float getMovevel();
	float getMousevel();
	bool isMouseIn();

	void setLocation(vector3d vec);
	void lookAt(float pitch, float yaw);
	void mouseIn(bool b);
	void setSpeed(float mv, float mov);
	void invertPitch();
	bool isMoved();
};

#endif