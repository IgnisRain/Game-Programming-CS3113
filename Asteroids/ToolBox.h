#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <random>
#include <vector>
#include <string>

using namespace std;

float RNG();

GLuint LoadTexture(const char*);

float lerp(float v0, float v1, float t);
