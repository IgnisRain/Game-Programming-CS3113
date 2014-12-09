#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class SpriteSheet {
public:
	SpriteSheet();
	SpriteSheet(GLuint, int, int, int);

	void draw(float scale);
	float getWidth();
	float getHeight();
	bool visible;
	int index;
	int spriteCountX;
	int spriteCountY;
	unsigned int getTextureID();

private:
	float u;
	float v;
	float width;
	float height;
	unsigned int textureID;
};