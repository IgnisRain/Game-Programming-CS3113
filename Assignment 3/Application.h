#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <random>
#include <time.h>
#include "ToolBox.h"
#include "Bullet.h"
#include "Entity.h"

#define MAX_BULLETS 25
#define MAX_ENEMIES 19
#define ENEMY_SPEED_X 0.5
#define ENEMY_SPEED_Y 0.02
using namespace std;

class Application {
public:
	Application();
	~Application();

	void Init();
	bool updateAndRender();
	void render();
	void update(float);
private:
	SDL_Window* displayWindow;

	const Uint8 *keys;

	SDL_Event event;

	bool start;

	float lastFrameTicks;
	float mainMenuFPS;
	float mainMenuElapsed;
	float LazerCD;
	float EnemyLazerCD;
	float leftMost;
	float rightMost;
	int orientation;
	float increasedSpeedY;

	enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };
	enum gameCondition { START_SCREEN, WIN, LOSE, IN_PROGRESS };
	int gameCondition;
	int state;
	int playerScore;
	unsigned int mainMenuCurrentIndex;
	unsigned int LazerIndex;
	unsigned int EnemyLazerIndex;
	GLuint fontTextureID;
	GLuint ssTextureID;

	Entity* AlienArray[MAX_ENEMIES];
	Bullet* Lazers[MAX_BULLETS];
	Bullet* EnemyLazer[MAX_BULLETS];
	Entity playerShip;

	void RenderMainMenu();
	void RenderGameLevel();
	void RenderGameOver();
	void renderEnemy();
	void renderEnemyLazer();
	void initializeGame();
	void shootBullet();
};