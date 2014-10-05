#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Sprites.h"

class Bullet {
public:
	Bullet(float, float, float, GLuint, float, float, float, float);
	void Draw();
	void setVisibility(bool);
	void updatePos(float);
	float getX();
	float getY();
private:
	float x;
	float y;
	float speed;
	bool visible;
	GLuint textureID;
	SpriteSheet sprites;
};