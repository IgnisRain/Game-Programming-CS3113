#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include "ToolBox.h"
#include "Entity.h"
#include "SDL_mixer.h"

#define MAX_TIMESTEPS 6
#define DIAMONDS 16
#define gravityX 0.0
#define gravityY -1.0

class Application{
public:
	Application();
	~Application();
	void Init();
	bool UpdateAndRender(); 
	void render();
	void FixedUpdate();
	void Update(float elapsed);
	void collisionDetect();
	void BuildLevel();
private:
	SDL_Window* displayWindow;
	SDL_Event event;
	const Uint8 *keys;
	Entity player;
	vector<Entity*> Entities;
	Entity blocks[248];
	Entity Diamonds[DIAMONDS];
	int numCollected;
	bool done;
	Mix_Chunk *jumpSound;
	Mix_Chunk *coinSound;

	float lastFrameTicks;
	float timeLeftOver;
	float yPenetration;
	float xPenetration;

	GLuint fontText;
	GLuint ssTextureID;

};