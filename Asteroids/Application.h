#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include "ToolBox.h"
#include "Entity.h"
#include "SDL_mixer.h"
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>

#define MAX_TIMESTEPS 6
#define MAX_ASTEROIDS 10

class Application{
public:
	Application();
	~Application();
	void Init();
	bool UpdateAndRender(); 
	void render();
	void FixedUpdate();
	void ProcessEvents();
	void collisionDetect();
	bool collidesWith(Entity* entity1, Entity* entity2);

private:
	SDL_Window* displayWindow;
	SDL_Event event;
	const Uint8 *keys;
	Entity player;
	float lastFrameTicks;
	float timeLeftOver;
	bool done;
	int state;

	Mix_Music *BGM;

	GLuint fontText;
	GLuint spriteSheet;
	Entity asteroidBelt[MAX_ASTEROIDS];
	vector<Entity*> Entities;
};