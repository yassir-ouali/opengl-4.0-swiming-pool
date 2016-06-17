#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include "stdafx.h"
#include <windows.h>
#include "GL/glew.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <fstream>

#define M_PI 3.14

unsigned int loadTexture(const char* filename);
SDL_Surface* loadTexture2(std::string& filename);//used for cube mapping
unsigned int loadCubemap(std::string* filenames);//6 filename
void loadFile(const char* fn, std::string &str);
unsigned int createTexture(int w, int h, bool isDepth = false);

#endif