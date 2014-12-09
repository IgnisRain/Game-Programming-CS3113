#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ToolBox.h"
#include "SpriteSheet.h"

#define FIXED_TIMESTEP 0.0166666f
#define TILE_SIZE 0.03f
using namespace std;

class Entity{
public:
	Entity();
	Entity(float, float, float, GLuint, float, float);
	void Draw();
	void ssDraw();
	void setSprite(SpriteSheet);
	void FixedUpdate();

	void setY(float);
	void setX(float);
	void setWidth(float);
	void setHeight(float);
	void setAccelerationX(float);
	void setAccelerationY(float);
	void setVelocityX(float);
	void setVelocityY(float);
	void setFrictionX(float);
	void setFrictionY(float);
	
	float getY();
	float getX();
	float getFrictionX();
	float getFrictionY();
	float getAccelerationX();
	float getAccelerationY();
	float getVelocityX();
	float getVelocityY();
	float getHeight();
	float getWidth();

	bool isStatic;
	SpriteSheet sprite;

	//collision flags
	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;
	bool collidesWith(Entity* other);
	
	bool flip;


private:
	float x;
	float y;
	float rotation;
	GLuint textureID;
	float width;
	float height;
	float frictionX;
	float frictionY;
	float accelerationX;
	float accelerationY;
	float velocityX;
	float velocityY;
};