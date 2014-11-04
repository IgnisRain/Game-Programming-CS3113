#include "Application.h"

Application::Application() {
	Init();
	BuildLevel();
	//Draw player sprite
	SpriteSheet playerSprite(ssTextureID, 219.0f / 257.0f, 0.0f / 257.0f, 36.0f / 257.0f, 52.0f / 257.0f);
	player.setSprite(playerSprite);
	player.setX(-0.0f);
	player.setY(-0.750f);
	player.setHeight(52.0f / 257.0f);
	player.setWidth(36.0f / 257.0f);
	player.setFrictionX(2.0f);
	player.setFrictionY(2.0f);
	player.isStatic = false;
}

void Application::Init()
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);

	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	keys = SDL_GetKeyboardState(NULL);
	done = false;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;

	fontText = LoadTexture("pixel_font.png", GL_RGBA);
	ssTextureID = LoadTexture("sprites.png", GL_RGBA);
	numCollected = 0;

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

	jumpSound = Mix_LoadWAV("jumpSound.wav");
	coinSound = Mix_LoadWAV("coinSound.wav");
}


Application::~Application() {

	Mix_Chunk(jumpSound);
	Mix_Chunk(coinSound);

	SDL_Quit();
}

//building outline of level and platforms
void Application::BuildLevel(){
	int blockIndex = 0;
	//top mid
	float currX = -0.735f;
	float currY = 0.90f;
	for (int i = 0; i < 10; i++) {
		blocks[blockIndex].sprite = SpriteSheet(ssTextureID, 0.0f / 257.0f, 70.0f / 257.0f, 70.0f / 257.0f, 70.0f / 257.0f);
		blocks[blockIndex].setWidth(70.0f / 257.f);
		blocks[blockIndex].setHeight(70.0f / 257.f);
		blocks[blockIndex].isStatic = true;
		blocks[blockIndex].setX(currX);
		blocks[blockIndex].setY(currY);
		Entities.push_back(&blocks[blockIndex]);
		blockIndex++;
		currX += (42.0 / 257.0f);
	}
	//bottom mid
	 currX = -0.735f;
	 currY = -0.90f;
	for (int i = 0; i < 10; i++) {
		blocks[blockIndex].sprite = SpriteSheet(ssTextureID, 146.0f / 257.0f, 0.0f / 257.0f, 70.0f / 257.0f, 70.0f / 257.0f);
		blocks[blockIndex].setWidth(70.0f / 257.f);
		blocks[blockIndex].setHeight(70.0f / 257.f);
		blocks[blockIndex].isStatic = true;
		blocks[blockIndex].setX(currX);
		blocks[blockIndex].setY(currY);
		Entities.push_back(&blocks[blockIndex]);
		blockIndex++;
		currX += (42.0 / 257.0f);
	}
	//left mid
	currX = -0.90f;
	currY = -0.735f;
	for (int i = 0; i < 10; i++) {
		blocks[blockIndex].sprite = SpriteSheet(ssTextureID, 72.0f / 257.0f, 72.0f / 257.0f, 70.0f / 257.0f, 70.0f / 257.0f);
		blocks[blockIndex].setWidth(70.0f / 257.f);
		blocks[blockIndex].setHeight(70.0f / 257.f);
		blocks[blockIndex].isStatic = true;
		blocks[blockIndex].setX(currX);
		blocks[blockIndex].setY(currY);
		Entities.push_back(&blocks[blockIndex]);
		blockIndex++;
		currY += (42.0 / 257.0f);
	}
	//right mid
	currX = 0.90f;
	currY = -0.735f;
	for (int i = 0; i < 10; i++) {
		blocks[blockIndex].sprite = SpriteSheet(ssTextureID, 72.0f / 257.0f, 0.0f / 257.0f, 70.0f / 257.0f, 70.0f / 257.0f);
		blocks[blockIndex].setWidth(70.0f / 257.f);
		blocks[blockIndex].setHeight(70.0f / 257.f);
		blocks[blockIndex].isStatic = true;
		blocks[blockIndex].setX(currX);
		blocks[blockIndex].setY(currY);
		Entities.push_back(&blocks[blockIndex]);
		blockIndex++;
		currY += (42.0 / 257.0f);
	}
	//top left corner
	currX = -0.90f;
	currY = 0.90f;
	blocks[blockIndex].sprite = SpriteSheet(ssTextureID, 0.0f / 257.0f, 143.0f / 257.0f, 70.0f / 257.0f, 70.0f / 257.0f);
	blocks[blockIndex].setWidth(70.0f / 257.f);
	blocks[blockIndex].setHeight(70.0f / 257.f);
	blocks[blockIndex].isStatic = true;
	blocks[blockIndex].setX(currX);
	blocks[blockIndex].setY(currY);
	Entities.push_back(&blocks[blockIndex]);
	blockIndex++;
	//top right corner
	currX = 0.90f;
	currY = 0.90f;
	blocks[blockIndex].sprite = SpriteSheet(ssTextureID, 0.0f / 257.0f, 0.0f / 257.0f, 70.0f / 257.0f, 70.0f / 257.0f);
	blocks[blockIndex].setWidth(70.0f / 257.f);
	blocks[blockIndex].setHeight(70.0f / 257.f);
	blocks[blockIndex].isStatic = true;
	blocks[blockIndex].setX(currX);
	blocks[blockIndex].setY(currY);
	Entities.push_back(&blocks[blockIndex]);
	blockIndex++;
	//bottom left corner
	currX = -0.90f;
	currY = -0.90f;
	blocks[blockIndex].sprite = SpriteSheet(ssTextureID, 146.5f / 257.0f, 73.0f / 257.0f, 70.0f / 257.0f, 70.0f / 257.0f);
	blocks[blockIndex].setWidth(70.0f / 257.f);
	blocks[blockIndex].setHeight(70.0f / 257.f);
	blocks[blockIndex].isStatic = true;
	blocks[blockIndex].setX(currX);
	blocks[blockIndex].setY(currY);
	Entities.push_back(&blocks[blockIndex]);
	blockIndex++;
	//bottom right corner
	currX = 0.90f;
	currY = -0.90f;
	blocks[blockIndex].sprite = SpriteSheet(ssTextureID, 73.0f / 257.0f, 146.5f / 257.0f, 70.0f / 257.0f, 70.0f / 257.0f);
	blocks[blockIndex].setWidth(70.0f / 257.f);
	blocks[blockIndex].setHeight(70.0f / 257.f);
	blocks[blockIndex].isStatic = true;
	blocks[blockIndex].setX(currX);
	blocks[blockIndex].setY(currY);
	Entities.push_back(&blocks[blockIndex]);
	blockIndex++;
	//platform 1
	currX = -0.3275f;
	currY = -0.4077f;
	for (int i = 0; i < 5; i++) {
		blocks[blockIndex].sprite = SpriteSheet(ssTextureID, 145.5f / 257.0f, 144.0f / 257.0f, 70.0f / 257.0f, 70.0f / 257.0f);
		blocks[blockIndex].setWidth(70.0f / 257.f);
		blocks[blockIndex].setHeight(70.0f / 257.f);
		blocks[blockIndex].isStatic = true;
		blocks[blockIndex].setX(currX);
		blocks[blockIndex].setY(currY);
		Entities.push_back(&blocks[blockIndex]);
		blockIndex++;
		currX += (42.0 / 257.0f);
	}
	
	//platform 2
    currX = -0.735f;
	currY = 0.0f;
	for (int i = 0; i < 3; i++) {
		blocks[blockIndex].sprite = SpriteSheet(ssTextureID, 145.5f / 257.0f, 144.0f / 257.0f, 70.0f / 257.0f, 70.0f / 257.0f);
		blocks[blockIndex].setWidth(70.0f / 257.f);
		blocks[blockIndex].setHeight(70.0f / 257.f);
		blocks[blockIndex].isStatic = true;
		blocks[blockIndex].setX(currX);
		blocks[blockIndex].setY(currY);
		Entities.push_back(&blocks[blockIndex]);
		blockIndex++;
		currX += (42.0 / 257.0f);
	}
	//platform 3
	currX = 0.408f;
	currY = 0.0f;
	for (int i = 0; i < 3; i++) {
		blocks[blockIndex].sprite = SpriteSheet(ssTextureID, 145.5f / 257.0f, 144.0f / 257.0f, 70.0f / 257.0f, 70.0f / 257.0f);
		blocks[blockIndex].setWidth(70.0f / 257.f);
		blocks[blockIndex].setHeight(70.0f / 257.f);
		blocks[blockIndex].isStatic = true;
		blocks[blockIndex].setX(currX);
		blocks[blockIndex].setY(currY);
		Entities.push_back(&blocks[blockIndex]);
		blockIndex++;
		currX += (42.0 / 257.0f);
	}
	//platform 4
	currX = -0.3275f;
	currY = 0.4073f;
	for (int i = 0; i < 5; i++) {
		blocks[blockIndex].sprite = SpriteSheet(ssTextureID, 145.5f / 257.0f, 144.0f / 257.0f, 70.0f / 257.0f, 70.0f / 257.0f);
		blocks[blockIndex].setWidth(70.0f / 257.f);
		blocks[blockIndex].setHeight(70.0f / 257.f);
		blocks[blockIndex].isStatic = true;
		blocks[blockIndex].setX(currX);
		blocks[blockIndex].setY(currY);
		Entities.push_back(&blocks[blockIndex]);
		blockIndex++;
		currX += (42.0 / 257.0f);
	}
	//jewels platform 1
	currX = -0.3275f;
	currY = -0.3050f;
	for (int i = 0; i < 5; i++) {
		blocks[blockIndex].sprite = SpriteSheet(ssTextureID, 0.0f / 257.0f, 218.5f / 257.0f, 35.0f / 257.0f, 35.0f / 257.0f);
		blocks[blockIndex].setWidth(35.0f / 257.f);
		blocks[blockIndex].setHeight(35.0f / 257.f);
		blocks[blockIndex].isStatic = true;
		blocks[blockIndex].setX(currX);
		blocks[blockIndex].setY(currY);
		Entities.push_back(&blocks[blockIndex]);
		blockIndex++;
		currX += (42.0 / 257.0f);
	}
	
	//jewels platform 2
	currX = -0.735f;
	currY = 0.1030f;
	for (int i = 0; i < 3; i++) {
		blocks[blockIndex].sprite = SpriteSheet(ssTextureID, 0.0f / 257.0f, 218.5f / 257.0f, 35.0f / 257.0f, 35.0f / 257.0f);
		blocks[blockIndex].setWidth(35.0f / 257.f);
		blocks[blockIndex].setHeight(35.0f / 257.f);
		blocks[blockIndex].isStatic = true;
		blocks[blockIndex].setX(currX);
		blocks[blockIndex].setY(currY);
		Entities.push_back(&blocks[blockIndex]);
		blockIndex++;
		currX += (42.0 / 257.0f);
	}
	//jewels platform 3
	currX = 0.406f;
	currY = 0.1030f;
	for (int i = 0; i < 3; i++) {
		blocks[blockIndex].sprite = SpriteSheet(ssTextureID, 0.0f / 257.0f, 218.5f / 257.0f, 35.0f / 257.0f, 35.0f / 257.0f);
		blocks[blockIndex].setWidth(35.0f / 257.f);
		blocks[blockIndex].setHeight(35.0f / 257.f);
		blocks[blockIndex].isStatic = true;
		blocks[blockIndex].setX(currX);
		blocks[blockIndex].setY(currY);
		Entities.push_back(&blocks[blockIndex]);
		blockIndex++;
		currX += (42.0 / 257.0f);
	}
	
	//jewels platform 4
	currX = -0.3275f;
	currY = 0.5093f;
	for (int i = 0; i < 5; i++) {
		blocks[blockIndex].sprite = SpriteSheet(ssTextureID, 0.0f / 257.0f, 218.5f / 257.0f, 35.0f / 257.0f, 35.0f / 257.0f);
		blocks[blockIndex].setWidth(35.0f / 257.f);
		blocks[blockIndex].setHeight(35.0f / 257.f);
		blocks[blockIndex].isStatic = true;
		blocks[blockIndex].setX(currX);
		blocks[blockIndex].setY(currY);
		Entities.push_back(&blocks[blockIndex]);
		blockIndex++;
		currX += (42.0 / 257.0f);
	}
}

//collision
void Application::collisionDetect() {
	int nonVisibleCount = 0;

	for (int i = 0; i < 60; i++) {
		if (player.collidesWith(Entities[i]) && (Entities[i]->getY() > player.getY())){
			player.collidedTop = true;
		}
		else if (player.collidesWith(Entities[i]) && (Entities[i]->getX() < player.getX())){
			player.collidedLeft = true;
		}
		else if (player.collidesWith(Entities[i]) && (Entities[i]->getX() > player.getX())){
			player.collidedRight = true;
		}
		if (player.collidesWith(Entities[i]) && (Entities[i]->getY() < player.getY())){
			player.collidedBottom = true;
		}
	}

	if (player.collidedBottom){
		for (int i = 0; i < 60; i++) {
			if (player.collidesWith(Entities[i])) {
				yPenetration = fabs((player.getY() - (player.getHeight() / 2 * 0.3 - 0.035f)) - (Entities[i]->getY() + (Entities[i]->getHeight() / 2 * 0.3 + 0.097)));
				player.setY(player.getY() + yPenetration);
				break;
			}
		}
	}
	if (player.collidedTop){
		for (int i = 0; i < 60; i++) {
			if (player.collidesWith(Entities[i])) {
				yPenetration = fabs((player.getY() + (player.getHeight() / 2 * 0.3 - 0.025)) - (Entities[i]->getY() - (Entities[i]->getHeight() / 2 * 0.3 + .097f)));
				if (player.getY() - yPenetration > -0.90)
					player.setY(player.getY() - yPenetration);
				player.collidedTop = false;
				player.setAccelerationY(0.0f);
				break;
			}              
		}
	}
	if (player.collidedLeft){
		for (int i = 0; i < 60; i++) {
			if (player.collidesWith(Entities[i]) &&
				(player.getY() + player.getHeight() * 0.3) > Entities[i]->getY() - 0.07 &&
				(player.getY() - player.getHeight() * 0.3) < (Entities[i]->getY() + 0.07))  {
				xPenetration = fabs((player.getX() - (player.getWidth() / 2 * 0.3 - 0.1)) - (Entities[i]->getX() + (Entities[i]->getWidth() / 2 * 0.3 + .097f)));
				player.setX(player.getX() + xPenetration + .093);
				player.setVelocityX(0.0f);
				break;
			}
		}
	}
	if (player.collidedRight){
			for (int i = 0; i < 60; i++) {
				if (player.collidesWith(Entities[i]) &&
					(player.getY() + player.getHeight() * 0.3) > Entities[i]->getY() - 0.07 &&
					(player.getY() - player.getHeight() * 0.3) < (Entities[i]->getY() + 0.07)) {
					xPenetration = fabs((player.getX() + (player.getWidth() / 2 * 0.3 + 0.1)) - (Entities[i]->getX() - (Entities[i]->getWidth() / 2 * 0.3 + .097f)));
					player.setX(player.getX() - xPenetration - .033);
					player.setVelocityX(0.0f);
					break;
			}
		}
	}
	if (player.getX() - 0.14f < -0.90)
		player.setX(-0.76);
	if (player.getX() + 0.14f > 0.90)
		player.setX(0.76);
	if (player.getY() - 0.14 < -0.90)
		player.setY(-0.76);

	for (int i = 60; i < Entities.size(); i++) {
		if (player.collidesWith(Entities[i])){
			if (Entities[i]->sprite.visible){
				Entities[i]->sprite.visible = false;
				Mix_PlayChannel(-1, coinSound, 0);
			}
		}
	}

	for (int i = 60; i < Entities.size(); i++){
		if (!Entities[i]->sprite.visible)
			nonVisibleCount++;
	}

	numCollected = nonVisibleCount;
}


void Application::render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < 60; i++)
		Entities[i]->ssDraw(0.3f);
	for (int i = 60; i < Entities.size(); i++){
		if (Entities[i]->sprite.visible)
			Entities[i]->ssDraw(0.3f);
	}

	player.ssDraw(0.3f);
	glLoadIdentity();
	glTranslatef(-0.4f, 0.85f, 0.0f);
	drawText(fontText, string("Diamonds Collected: ") + to_string(numCollected), 0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	SDL_GL_SwapWindow(displayWindow);
}


void Application::FixedUpdate(){
	player.setVelocityX(lerp(player.getVelocityX(), 0.0f, FIXED_TIMESTEP * player.getFrictionX()));
	player.setVelocityY(lerp(player.getVelocityY(), 0.0f, FIXED_TIMESTEP * player.getFrictionY()));
	player.setVelocityX(player.getVelocityX() + player.getAccelerationX() * FIXED_TIMESTEP);
	player.setVelocityY(player.getVelocityY() + player.getAccelerationY() * FIXED_TIMESTEP);
	player.setVelocityX(player.getVelocityX() + gravityX * FIXED_TIMESTEP);
	player.setVelocityY(player.getVelocityY() + gravityY * FIXED_TIMESTEP);
}

void Application::Update(float elapsed) {
	player.setAccelerationX(0.0f);
	player.setAccelerationY(0.0f);

	if (keys[SDL_SCANCODE_RIGHT]){
		player.setAccelerationX(2.0f);
	}
	else if (keys[SDL_SCANCODE_LEFT]){
		player.setAccelerationX(-2.0f);
	}
	else if (keys[SDL_SCANCODE_SPACE]){
		if (player.collidedBottom){
			player.setVelocityY(1.8f);
			player.collidedBottom = false;
			Mix_PlayChannel(-1, jumpSound, 0);
		}
	}
	player.setX(player.getX() + player.getVelocityX() * elapsed);
	player.setY(player.getY() + player.getVelocityY() * elapsed);
}

bool Application::UpdateAndRender()
{
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
		{
			done = true;
		}
	}
	 
	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS)
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;

	while (fixedElapsed >= FIXED_TIMESTEP)
	{
		fixedElapsed -= FIXED_TIMESTEP;
		FixedUpdate();
	}
	timeLeftOver = fixedElapsed;
	collisionDetect();
	render();
	Update(elapsed);
	return done;
}






