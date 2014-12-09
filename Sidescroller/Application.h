#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include "ToolBox.h"
#include "Entity.h"
#include "SDL_mixer.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#define MAX_TIMESTEPS 6
#define LEVEL_HEIGHT 32
#define LEVEL_WIDTH 128
#define TILE_SIZE 0.05f
#define SPRITE_COUNT_X 16
#define SPRITE_COUNT_Y 8
#define WORLD_OFFSET_X 0.0f
#define WORLD_OFFSET_Y 0.0f
#define FIXED_TIMESTEP 0.0166666f

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
	void BuildLevel();
	bool readHeader(ifstream& stream);
	bool readLayerData(ifstream& stream);
	bool readEntityData(ifstream& stream);
	bool isSolid(unsigned int tile);
	bool playerDeath;

	void levelCollisionX(Entity* entity);
	void levelCollisionY(Entity* entity);
	float checkforCollisionX(float x, float y);
	float checkforCollisionY(float x, float y);
	void placeEntity(string type, float placeX, float placeY);
	void worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY);

private:
	SDL_Window* displayWindow;
	SDL_Event event;
	const Uint8 *keys;
	Entity player;
	float lastFrameTicks;
	float timeLeftOver;
	bool done;

	Mix_Chunk *jumpSound;
	Mix_Music *BGM;
	int state;
	int mapWidth;
	int mapHeight;

	float gravityX;
	float gravityY;

	GLuint fontText;
	GLuint spriteSheet;

	unsigned char** levelData;
};