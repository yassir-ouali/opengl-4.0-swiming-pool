#include "stdafx.h"
#include "definitions.h"

unsigned int createTexture(int w, int h, bool isDepth)
{
	unsigned int textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, (!isDepth ? GL_RGBA8 : GL_DEPTH_COMPONENT), w, h, 0, isDepth ? GL_DEPTH_COMPONENT : GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	int i;
	i = glGetError();
	if (i != 0)
	{
		std::cout << "Error happened while loading the texture: " << i << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureId;
}

unsigned int loadTexture(const char* filename)
{
	unsigned int num;
	glGenTextures(1, &num);
	SDL_Surface* img = IMG_Load(filename);
	if (img == NULL)
	{
		std::cout << "img was not loaded" << std::endl;
		return -1;
	}
	SDL_PixelFormat form = { NULL, 32, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff, 0, 255 };
	SDL_Surface* img2 = SDL_ConvertSurface(img, &form, SDL_SWSURFACE);
	if (img2 == NULL)
	{
		std::cout << "img2 was not loaded" << std::endl;
		return -1;
	}
	glBindTexture(GL_TEXTURE_2D, num);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img2->w, img2->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, img2->pixels);
	SDL_FreeSurface(img);
	SDL_FreeSurface(img2);
	return num;
}

SDL_Surface* loadTexture2(std::string& filename)
{
	unsigned int num;
	glGenTextures(1, &num);
	SDL_Surface* img = IMG_Load(filename.c_str());
	if (img == NULL)
	{
		std::cout << "img was not loaded" << std::endl;
		return 0;
	}
	SDL_PixelFormat form = { NULL, 32, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff, 0, 255 };
	SDL_Surface* img2 = SDL_ConvertSurface(img, &form, SDL_SWSURFACE);
	SDL_FreeSurface(img);
	return img2;
}

unsigned int loadCubemap(std::string* filenames)//6 filename
{
	//right,left,top,bottom,near,far
	unsigned int tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

	for (int i = 0; i<6; i++)
	{
		SDL_Surface* img = loadTexture2(filenames[i]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, img->pixels);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		SDL_FreeSurface(img);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return tex;
}

void loadFile(const char* fn, std::string& str)
{
	std::ifstream in(fn);
	if (!in.is_open())
	{
		std::cout << "The file " << fn << " cannot be opened\n";
		return;
	}
	char tmp[300];
	while (!in.eof())
	{
		in.getline(tmp, 300);
		str += tmp;
		str += '\n';
	}
}
