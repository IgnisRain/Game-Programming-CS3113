#include "Application.h"

enum GameState { STATE_GAME, STATE_GAMEOVER };

Application::Application() {
	Init();
	state = STATE_GAME;
	playerDeath = false;

	gravityX = 0.0f;
	gravityY = -1.0f;
	BuildLevel();
	Mix_PlayMusic(BGM, -1);

	player.sprite = SpriteSheet(spriteSheet, 98, SPRITE_COUNT_X, SPRITE_COUNT_Y);
	player.isStatic = false;
	player.setFrictionX(1.3);
}

void Application::Init()
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("SideScroller", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);

	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	//glOrtho(-2.66, 2.66, -2.0, 2.0, -2.0, 2.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	keys = SDL_GetKeyboardState(NULL);
	done = false;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;

	fontText = LoadTexture("pixel_font.png");
	spriteSheet = LoadTexture("sprites.png");

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	BGM = Mix_LoadMUS("BGM.ogg");
	Mix_PlayMusic(BGM, -1);
	jumpSound = Mix_LoadWAV("jumpSound.wav");
}

Application::~Application() {

	Mix_Chunk(jumpSound);

	SDL_Quit();
}

void Application::BuildLevel(){
	ifstream infile("myLevel.txt");
	string line;
	while (getline(infile, line)) {
		if (line == "[header]") {
			if (!readHeader(infile)) {
				return;
			}
		}
		else if (line == "[layer]") {
			readLayerData(infile);
		}
		else if (line == "[Object Layer 1]")
		{
			readEntityData(infile);
		}
	}
}

bool Application::readHeader(ifstream &stream)
{
	string line;
	mapWidth = -1;
	mapHeight = -1;
	while (getline(stream, line)) {
		if (line == "") { break; }

		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);

		if (key == "width") {
			mapWidth = atoi(value.c_str());
		}
		else if (key == "height"){
			mapHeight = atoi(value.c_str());
		}
	}

	if (mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else { // allocate our map data
		levelData = new unsigned char*[mapHeight];
		for (int i = 0; i < mapHeight; ++i) {
			levelData[i] = new unsigned char[mapWidth];
		}
		return true;
	}
}

bool Application::readLayerData(ifstream &stream)
{
	string line;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "data") {
			for (int y = 0; y < mapHeight; y++) {
				getline(stream, line);
				istringstream lineStream(line);
				string tile;
				for (int x = 0; x < mapWidth; x++) {
					getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						// be careful, the tiles in this format are indexed from 1 not 0
						levelData[y][x] = val - 1;
					}
					else {
						levelData[y][x] = 12;
					}
				}
			}
		}
	}
	return true;
}

bool Application::readEntityData(ifstream &stream){
	string line;
	string type;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "type") {
			type = value;
		}
		else if (key == "location") {
			istringstream lineStream(value);
			string xPosition, yPosition;
			getline(lineStream, xPosition, ',');
			getline(lineStream, yPosition, ',');
			float placeX = atoi(xPosition.c_str()) / 16 * TILE_SIZE;
			float placeY = atoi(yPosition.c_str()) / 16 * -TILE_SIZE;
			placeEntity(type, placeX, placeY);
		}
	}
	return true;
}

void Application::placeEntity(string type, float placeX, float placeY)
{
	if (type == "player")
	{
		player.setX(placeX);
		player.setY(placeY);
	}
}

void Application::render()
{
	glClearColor(0.3f, 0.7f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (state == STATE_GAME){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, spriteSheet);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		float translateX = -player.getX();
		float translateY = -player.getY() / 2;

		if (translateY < 0.0)
			translateY = 0.0;

		if (translateX > 0.0)
			translateX = 0.0;

		glTranslatef(translateX, translateY, 0.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, spriteSheet);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		vector<float> vertexData;
		vector<float> texCoordData;
		for (int y = 0; y < mapHeight; y++) {
			for (int x = 0; x < mapWidth; x++) {
				float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
				float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
				float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
				float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
				vertexData.insert(vertexData.end(), {
					TILE_SIZE * x, -TILE_SIZE * y,
					TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
				});
				texCoordData.insert(texCoordData.end(), { u, v,
					u, v + (spriteHeight),
					u + spriteWidth, v + (spriteHeight),
					u + spriteWidth, v
				});
			}
		}
		glPushMatrix();
		glTranslatef(TILE_SIZE * LEVEL_WIDTH / 2, -TILE_SIZE * LEVEL_HEIGHT / 2, 0.0f);

		glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
		glEnableClientState(GL_VERTEX_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glTranslatef(-TILE_SIZE* mapWidth / 2, TILE_SIZE* mapHeight / 2, 0.0f);

		glDrawArrays(GL_QUADS, 0, mapWidth * mapHeight * 4);
		glDisable(GL_TEXTURE_2D);

		glPopMatrix();
		player.Draw(); 
	}
	else
	{
		glLoadIdentity();
		glTranslatef(-0.55f, 0.2f, 0.0f);
		drawText(fontText, string("GAME OVER"), 0.15f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		glLoadIdentity();
		if (playerDeath)
		{
			glTranslatef(-0.2f, 0.0f, 0.0f);
			drawText(fontText, string("You Died!"), 0.05f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		}
		else{
			glTranslatef(-0.2f, 0.0f, 0.0f);
			drawText(fontText, string("You Won!"), 0.05f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		}
	}
	SDL_GL_SwapWindow(displayWindow);
}

void Application::ProcessEvents()
{
	player.setAccelerationX(0.0f);

	if (keys[SDL_SCANCODE_RIGHT])
	{
		player.flip = false;
		player.setAccelerationX(1.5f);
	}
	else if (keys[SDL_SCANCODE_LEFT])
	{
		player.flip = true;
		player.setAccelerationX(-1.5f);
	}
	if (player.collidedBottom) {
		if (keys[SDL_SCANCODE_SPACE]) {
			player.setVelocityY(0.9f);
			Mix_PlayChannel(-1, jumpSound, 0);
		}
	}
	// update player
	player.setX(player.getX() + player.getVelocityX() * FIXED_TIMESTEP);
	player.setY(player.getY() + player.getVelocityY() * FIXED_TIMESTEP);
}

void Application::FixedUpdate(){

	player.collidedBottom = false;
	player.collidedTop = false;
	player.collidedLeft = false;
	player.collidedRight = false;

	player.setVelocityX(lerp(player.getVelocityX(), 0.0f, FIXED_TIMESTEP * player.getFrictionX()));
	player.setVelocityY(lerp(player.getVelocityY(), 0.0f, FIXED_TIMESTEP * player.getFrictionY()));
	player.setVelocityX(player.getVelocityX() + player.getAccelerationX() * FIXED_TIMESTEP);
	player.setVelocityY(player.getVelocityY() + player.getAccelerationY() * FIXED_TIMESTEP);
	player.setVelocityX(player.getVelocityX() + gravityX * FIXED_TIMESTEP);
	player.setVelocityY(player.getVelocityY() + gravityY * FIXED_TIMESTEP);

	// player collision
	levelCollisionY(&player);
	levelCollisionX(&player);
}

void Application::worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY)
{
	*gridX = (int)((worldX + (WORLD_OFFSET_X)) / TILE_SIZE);
	*gridY = (int)((-worldY + (WORLD_OFFSET_Y)) / TILE_SIZE);
}

bool Application::isSolid(unsigned int tile)
{
	//Mark the tiles that are solid
	switch (tile){
	case 6:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
		return true;
		break;
	default:
		return false;
		break;
	}
}

float Application::checkforCollisionY(float x, float y)
{
	int gridX, gridY;
	worldToTileCoordinates(x, y, &gridX, &gridY);
	if (gridX < 0 || gridX > 128 || gridY < 0 || gridY > 40)
		return 0.0f;

	if (isSolid(levelData[gridY][gridX]))
	{
		float yCoord = (gridY * TILE_SIZE) - (TILE_SIZE*0.0f);
		return -y - yCoord;
	}
	return 0.0;
}

float Application::checkforCollisionX(float x, float y)
{
	int gridX, gridY;
	worldToTileCoordinates(x, y, &gridX, &gridY);
	if (gridX < 0 || gridX > 128 || gridY < 0 || gridY > 40)
		return 0.0f;

	if (isSolid(levelData[gridY][gridX]))
	{
		float xCoord = (gridX * TILE_SIZE) - (TILE_SIZE*0.0f);
		return x - xCoord;
	}
	return 0.0;
}

void Application::levelCollisionY(Entity* entity)
{
	//check bottom
	float adjust = checkforCollisionY(entity->getX(), entity->getY() - entity->getHeight() * 0.5f + 0.03);
	if (adjust != 0.0f)
	{
		entity->setY(entity->getY() + adjust);
		entity->setVelocityY(0.0f);
		entity->collidedBottom = true;
	}

	//check top
	adjust = checkforCollisionY(entity->getX(), entity->getY() + entity->getHeight() * 0.5f - 0.03);
	if (adjust != 0.0f)
	{
		entity->setY(entity->getY() + adjust - TILE_SIZE);
		entity->setVelocityY(0.0f);
		entity->collidedTop = true;
	}
}

void Application::levelCollisionX(Entity* entity)
{
	//check left
	float adjust = checkforCollisionX(entity->getX() - entity->getWidth() * 0.5f, entity->getY());
	if (adjust != 0.0f)
	{
		entity->setX(entity->getX() + adjust * TILE_SIZE);
		entity->setVelocityX(0.0f);
		entity->collidedLeft = true;
	}

	//check right
	adjust = checkforCollisionX(entity->getX() + entity->getWidth() * 0.5f, entity->getY());
	if (adjust != 0.0f)
	{
		entity->setX(entity->getX() + (adjust - TILE_SIZE) * TILE_SIZE);
		entity->setVelocityX(0.0f);
		entity->collidedRight = true;
	}
}

void Application::collisionDetect()
{
	if (state != STATE_GAME)
		return;
	//Check
	if (player.getY() < -1.5){
		playerDeath = true;
		state = STATE_GAMEOVER;
	}
	if (player.getX() > 6.33){
		state = STATE_GAMEOVER;
	}
	if (player.getX() < 0.0){
		player.setX(player.getX() + .01);
		player.setVelocityX(0.0f);
	}
}

bool Application::UpdateAndRender()
{
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return true;
		}
	}
	// timestep 
	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		if (state == STATE_GAME){
			FixedUpdate();
			ProcessEvents();
		}
	}

	timeLeftOver = fixedElapsed;

	collisionDetect();
	render();
	return false;
}







