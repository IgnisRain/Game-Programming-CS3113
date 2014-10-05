#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class SpriteSheet {
public:
	SpriteSheet();
	SpriteSheet(unsigned int textureID, float u, float v, float width, float height);

	void draw(float scale);
	float getWidth();
	float getHeight();
private:
	float u;
	float v;
	float width;
	float height;
	unsigned int textureID;
};