#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ToolBox.h"
#include "Sprites.h"

class Entity{
public:
	Entity(float, float, float, GLuint, float, float);
	Entity(SpriteSheet, float, float);
	Entity();
	
	void Draw();
	void ssDraw(float);
	void setSprite(SpriteSheet);
	void setY(float);
	void setX(float);
	float getY();
	float getX();
private:
	float x;
	float y;
	float rotation;
	GLuint textureID;
	float width;
	float height;
	SpriteSheet sprite;
};