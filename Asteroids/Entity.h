#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ToolBox.h"
#include "SpriteSheet.h"
#include "Matrix.h"

#define FIXED_TIMESTEP 0.0166666f
#define PI 3.14159265359f
using namespace std;

class Entity{
public:
	Entity();
	void Draw();
	void FixedUpdate();

	float x;
	float y;
	float rotation;
	float scaleX;
	float scaleY;
	float velocity;
	float rotationVelocity;
	float acceleration;
	float rotationAcceleration;
	float friction;
	bool collided;
	void BuildMatrix();
	float getHeight();
	float getWidth();

	Matrix matrix;
	SpriteSheet sprite;

};