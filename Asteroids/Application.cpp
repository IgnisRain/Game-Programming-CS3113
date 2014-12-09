#include "Application.h"


Application::Application() {
	Init();
	player.sprite = SpriteSheet(spriteSheet, 1.0f, 115.0f / 1024.0f, 870.0f / 1024.0f, 105.0f / 1024.0f, 70.0f / 1024.0f);
	Entities.push_back(&player);
}

void Application::Init()
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);

	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	keys = SDL_GetKeyboardState(NULL);
	done = false;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;

	fontText = LoadTexture("pixel_font.png");
	spriteSheet = LoadTexture("sprites.png");

	for (int i = 0; i < MAX_ASTEROIDS; i++) 
	{
		asteroidBelt[i].sprite = SpriteSheet(spriteSheet, 1.0f, 520.0f / 1024.0f, 730.0f / 1024.0f, 85.0f / 1024.0f, 80.0f / 1024.0f);
		asteroidBelt[i].x = RNG();
		asteroidBelt[i].y = RNG();
		float scale = (RNG() + 2.0f);
		asteroidBelt[i].scaleX = scale;
		asteroidBelt[i].scaleY = scale;
		asteroidBelt[i].acceleration = 1.0f;
		asteroidBelt[i].rotation = RNG() * 6.0f;
		Entities.push_back(&asteroidBelt[i]);
	}

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	BGM = Mix_LoadMUS("BGM.ogg");
	Mix_PlayMusic(BGM, -1);

}

Application::~Application() {

	SDL_Quit();
}

void Application::render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

		player.Draw();
		for (int i = 0; i < MAX_ASTEROIDS; i++)
			asteroidBelt[i].Draw();

	SDL_GL_SwapWindow(displayWindow);
}

void Application::ProcessEvents()
{
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE){
			done = true;
		}
			player.rotationAcceleration = 0.0f;
			player.acceleration = 0.0f;

			if (keys[SDL_SCANCODE_RIGHT]){
				player.rotationAcceleration = -5.0f;
			}
			else if (keys[SDL_SCANCODE_LEFT]){
				player.rotationAcceleration = 5.0f;
			}
			if (keys[SDL_SCANCODE_UP]){
				player.acceleration = 2.0f;
		}
	}
}

void Application::FixedUpdate()
{

	//Update asteroids
	for (int i = 0; i < MAX_ASTEROIDS; i++){
		asteroidBelt[i].FixedUpdate();
		asteroidBelt[i].collided = false;
		if (asteroidBelt[i].x < -1.33)
			asteroidBelt[i].x = 1.33f;
		else if (asteroidBelt[i].x > 1.33)
			asteroidBelt[i].x = -1.33f;
		if (asteroidBelt[i].y < -1)
			asteroidBelt[i].y = 1.0f;
		else if (asteroidBelt[i].y > 1)
			asteroidBelt[i].y = -1.0f;
	}

	//player update
	player.FixedUpdate();
	player.collided = false;
	if (player.x < -1.33)
		player.x = 1.33f;
	else if (player.x > 1.33)
		player.x = -1.33f;
	if (player.y < -1)
		player.y = 1.0f;
	else if (player.y > 1)
		player.y = -1.0f;
}

void Application::collisionDetect()
{
		for (int i = 0; i < Entities.size(); i++){
			for (int j = 0; j < Entities.size(); j++){
				if (j != i){
					if (collidesWith(Entities[i], Entities[j])){
						Vector distance = Vector(Entities[i]->x - Entities[j]->x, Entities[i]->y - Entities[j]->y, 0.0f);
						float length = distance.length();
						distance.normalize();
						Entities[i]->collided = true;
						Entities[j]->collided = true;

						Entities[i]->x += distance.x * (0.001f / length);
						Entities[i]->y += distance.y * (0.001f / length);
						Entities[j]->x -= distance.x * (0.001f / length);
						Entities[j]->y -= distance.y * (0.001f / length);
				}
			}
		}
	}
}

bool Application::collidesWith(Entity* Entity1, Entity* Entity2)
{
	Entity1->BuildMatrix();
	Entity2->BuildMatrix();

	Matrix Entity1inverse = Entity1->matrix.inverse();
	Matrix Entity2inverse = Entity2->matrix.inverse();

	Vector Ent1TL = Vector(-Entity1->getWidth(), Entity1->getHeight(), 0.0f);
	Vector Ent1BL = Vector(-Entity1->getWidth(), -Entity1->getHeight(), 0.0f);
	Vector Ent1TR = Vector(Entity1->getWidth(), Entity1->getHeight(), 0.0f);
	Vector Ent1BR = Vector(Entity1->getWidth(), -Entity1->getHeight(), 0.0f);

	Vector ent2TL = Vector(-Entity2->getWidth(), Entity2->getHeight(), 0.0f);
	Vector ent2BL = Vector(-Entity2->getWidth(), -Entity2->getHeight(), 0.0f);
	Vector ent2TR = Vector(Entity2->getWidth(), Entity2->getHeight(), 0.0f);
	Vector ent2BR = Vector(Entity2->getWidth(), -Entity2->getHeight(), 0.0f);

	//Entity2 -> WS
	ent2TL = Entity2->matrix * ent2TL;  
	ent2BL = Entity2->matrix * ent2BL;
	ent2TR = Entity2->matrix * ent2TR;
	ent2BR = Entity2->matrix * ent2BR;

	//Entity2 -> Entity1 WS
	ent2TL = Entity1inverse * ent2TL;
	ent2BL = Entity1inverse * ent2BL;
	ent2TR = Entity1inverse * ent2TR;
	ent2BR = Entity1inverse * ent2BR;

	//check x axis
	float minX = min(min(min(ent2TL.x, ent2BL.x), ent2TR.x), ent2BR.x);
	float maxX = max(max(max(ent2TL.x, ent2BL.x), ent2TR.x), ent2BR.x);
	if (minX > Entity1->getWidth() || maxX < -Entity1->getWidth())
		return false;

	//check y axis
	float minY = min(min(min(ent2TL.y, ent2BL.y), ent2TR.y), ent2BR.y);
	float maxY = max(max(max(ent2TL.y, ent2BL.y), ent2TR.y), ent2BR.y);
	if (minY > Entity1->getHeight() || maxY < -Entity1->getHeight())
		return false;

	//reset Entity2's vectors
	ent2TL = Vector(-Entity2->getWidth(), Entity2->getHeight(), 0.0f);
	ent2BL = Vector(-Entity2->getWidth(), -Entity2->getHeight(), 0.0f);
	ent2TR = Vector(Entity2->getWidth(), Entity2->getHeight(), 0.0f);
	ent2BR = Vector(Entity2->getWidth(), -Entity2->getHeight(), 0.0f);

	//Entity1 -> WS
	Ent1TL = Entity1->matrix * Ent1TL;
	Ent1BL = Entity1->matrix * Ent1BL;
	Ent1TR = Entity1->matrix * Ent1TR;
	Ent1BR = Entity1->matrix * Ent1BR;

	//Entity1 -> Entity2 OS
	Ent1TL = Entity2inverse * Ent1TL;
	Ent1BL = Entity2inverse * Ent1BL;
	Ent1TR = Entity2inverse * Ent1TR;
	Ent1BR = Entity2inverse * Ent1BR;

	//check x axis
	minX = min(min(min(Ent1TL.x, Ent1BL.x), Ent1TR.x), Ent1BR.x);
	maxX = max(max(max(Ent1TL.x, Ent1BL.x), Ent1TR.x), Ent1BR.x);
	if (minX > Entity2->getWidth() || maxX < -Entity2->getWidth())
		return false;

	//check y axis
	minY = min(min(min(Ent1TL.y, Ent1BL.y), Ent1TR.y), Ent1BR.y);
	maxY = max(max(max(Ent1TL.y, Ent1BL.y), Ent1TR.y), Ent1BR.y);
	if (minY > Entity2->getHeight() || maxY < -Entity2->getHeight())
		return false;

	return true;
}

bool Application::UpdateAndRender()
{
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS)
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;

	while (fixedElapsed >= FIXED_TIMESTEP)
	{
		fixedElapsed -= FIXED_TIMESTEP;
		FixedUpdate();
		collisionDetect();
	}
	timeLeftOver = fixedElapsed;

	ProcessEvents();
	render();
	return done;
}







