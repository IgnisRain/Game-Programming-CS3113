#include "Application.h"


Application::Application() {
	Init();
}

Application::~Application() {

	for (int i = 0; i < MAX_ENEMIES - 1; ++i) {
		if (AlienArray[i] != NULL) {
			delete AlienArray[i];
			AlienArray[i] = NULL;
		}
	}
	for (int j = 0; j < MAX_BULLETS - 1; ++j) {
		if (Lazers[j] != NULL) {
			delete Lazers[j];
			Lazers[j] = NULL;
		}
	}
	for (int i = 0; i < MAX_BULLETS - 1; ++i) {
		if (EnemyLazer[i] != NULL) {
			delete EnemyLazer[i];
			EnemyLazer[i] = NULL;
		}
	}
	SDL_Quit();
}

void Application::Init() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	srand(time(NULL));

	lastFrameTicks = 0.0f;
	mainMenuElapsed = 0.0f;
	mainMenuFPS = 3.0f;
	mainMenuCurrentIndex = 0;
	LazerCD = 0.0;
	EnemyLazerCD = 0.0;
	LazerIndex = 0;
	EnemyLazerIndex = 0;
	playerScore = 0;
	state = STATE_MAIN_MENU;
	gameCondition = START_SCREEN;
	start = false;
	keys = SDL_GetKeyboardState(NULL);
	fontTextureID = LoadTexture("pixel_font.png", GL_RGBA);
	ssTextureID = LoadTexture("sprites.png", GL_RGBA);
}

void Application::RenderMainMenu() {

	SpriteSheet playerSprite(ssTextureID, 0.0f / 128.0f, 0.0f / 128.0f, 112.0f / 128.0f, 38.0f / 128.0f);
	playerShip.setSprite(playerSprite);
	playerShip.setX(0.0f);
	playerShip.setY(-0.825f);
	playerShip.ssDraw(0.20f);
	//game title
	glLoadIdentity();
	glTranslatef(-0.16f, 0.75f, 0.0f);
	drawText(fontTextureID, "SPACE", 0.1f, 0.0f, 1.0f, 1.0f, 0.2f, 1.0f);

	glLoadIdentity();
	glTranslatef(-0.3f, 0.55f, 0.0f);
	drawText(fontTextureID, "INVADERS", 0.1f, 0.0f, 1.0f, 1.0f, 0.2f, 1.0f);

	// start button
	glLoadIdentity();
	GLfloat startBox[] = { -0.25f, 0.1f, -0.25f, -0.1f, 0.25f, -0.1f, 0.25f, 0.1f };
	glVertexPointer(2, GL_FLOAT, 0, startBox);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat boxColor[] = { 0.6f, 0.6f, 0.6f, 1.0f, 0.6f, 0.6f, 0.6f, 1.0f, 0.6f, 0.6f, 0.6f, 1.0f, 0.6f, 0.6f, 0.6f, 1.0f };
	glColorPointer(4, GL_FLOAT, 0, boxColor);
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_COLOR_ARRAY);

	glLoadIdentity();
	glTranslatef(-0.1f, 0.0f, 0.0f);
	drawText(fontTextureID, "START", 0.05f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);


}

void Application::RenderGameLevel() {
	// render "SCORE"
	glLoadIdentity();
	glTranslatef(-0.1f, 0.95f, 0.0f);
	drawText(fontTextureID, "SCORE", 0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);

	// render actual score
	glLoadIdentity();
	glTranslatef(0.2f, 0.95f, 0.0f);
	drawText(fontTextureID, std::to_string(playerScore), 0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);

	playerShip.ssDraw(0.20f);
	shootBullet();
	renderEnemyLazer();
	renderEnemy();

	// message to inform player how to start game
	if (start != true) {
		glLoadIdentity();
		glTranslatef(-0.4f, -0.45f, 0.0f);
		drawText(fontTextureID, "Press 'S' to start!", 0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void Application::RenderGameOver() {
	if (gameCondition == WIN) {
		glLoadIdentity();
		glTranslatef(-0.2f, 0.3f, 0.0f);
		drawText(fontTextureID, "YOU WON!", 0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.35f, 0.05f, 0.0f);
		drawText(fontTextureID, "CONGRATULATIONS", 0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.43f, -0.2f, 0.0f);
		drawText(fontTextureID, "YOUR SCORE WAS: ", 0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(0.32f, -0.2f, 0.0f);
		drawText(fontTextureID, std::to_string(playerScore), 0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	}
	if (gameCondition == LOSE) {
		glLoadIdentity();
		glTranslatef(-0.2f, 0.3f, 0.0f);
		drawText(fontTextureID, "YOU LOST!", 0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.35f, 0.05f, 0.0f);
		drawText(fontTextureID, "PLAY AGAIN SOON!", 0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(-0.43f, -0.2f, 0.0f);
		drawText(fontTextureID, "YOUR SCORE WAS: ", 0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		glLoadIdentity();
		glTranslatef(0.32f, -0.2f, 0.0f);
		drawText(fontTextureID, std::to_string(playerScore), 0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void Application::render() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	switch (state) {
	case STATE_MAIN_MENU:
		RenderMainMenu();
		break;
	case STATE_GAME_LEVEL:
		RenderGameLevel();
		break;
	case STATE_GAME_OVER:
		RenderGameOver();
		break;
	}

	SDL_GL_SwapWindow(displayWindow);
}


void Application::update(float elapsed) {
	float playerShipMovement = 0.0f;
	switch (state){
	case STATE_MAIN_MENU:
	case STATE_GAME_LEVEL:
		if (start == true){  //start by pressing s
			if (keys[SDL_SCANCODE_LEFT]) {
				playerShipMovement = -1.5;
			}
			if (keys[SDL_SCANCODE_RIGHT]) {
				playerShipMovement = 1.5;
			}
			if (playerShip.getX() + (playerShipMovement * elapsed) < 1.22 && playerShip.getX() + (playerShipMovement * elapsed) > -1.22) {
				playerShip.setX((elapsed * playerShipMovement) + playerShip.getX());
			}

			//bullet movement and logic
			if (keys[SDL_SCANCODE_SPACE]) {
				if (LazerIndex < MAX_BULLETS - 1) {
					if (LazerCD > 0.5f) { // can shoot every .5 seconds
						if (Lazers[LazerIndex] != NULL)
							delete Lazers[LazerIndex]; // prevent memory leak
						Lazers[LazerIndex] = new Bullet(playerShip.getX(), playerShip.getY() + 0.12f, 2.0f, ssTextureID, 0.0f, 101.0f, 10.0f, 22.0f);
						LazerIndex++;
						LazerCD = 0.0f;
						increasedSpeedY += 0.005;
					}
				}
				else {
					if (LazerCD > 0.5f) { // can shoot every .5 seconds
						LazerIndex = 0;
						delete Lazers[LazerIndex];
						Lazers[LazerIndex] = new Bullet(playerShip.getX(), playerShip.getY() + 0.12f, 1.0f, ssTextureID, 0.0f, 101.0f, 10.0f, 22.0f);
						LazerIndex++;
						LazerCD = 0.0f;
					}
				}
			}
			// updates Lazer projectile in array
		for (int i = 0; i < MAX_BULLETS - 1; ++i) {
				if (Lazers[i] != NULL) {
					if (Lazers[i]->getY() < 0.95f) {
						Lazers[i]->updatePos(elapsed);
					}
					else if (Lazers[i]->getY() > 0.95f) {
						delete Lazers[i];
						Lazers[i] = NULL;
					}
				}
			}

			//Alien Movement logic
			if ((rightMost + (ENEMY_SPEED_X * elapsed)) > 1.3f){  //Collision with right wall
				for (int i = 0; i < MAX_ENEMIES - 1; i++) {
					if (AlienArray[i] != NULL){
						AlienArray[i]->setY(AlienArray[i]->getY() - ENEMY_SPEED_Y - increasedSpeedY);
						AlienArray[i]->setX(AlienArray[i]->getX() - 0.03f);
					}
				}
				rightMost -= 0.03;
				leftMost -= 0.03;
				orientation = 0;
			}
			else if ((leftMost - (ENEMY_SPEED_X * elapsed)) < -1.3f){  //Collision with left wall
				for (int i = 0; i < MAX_ENEMIES; i++){
					if (AlienArray[i] != NULL) {
						AlienArray[i]->setY(AlienArray[i]->getY() - ENEMY_SPEED_Y - increasedSpeedY);
						AlienArray[i]->setX(AlienArray[i]->getX() + 0.03f);

					}
				}
				orientation = 1;
				leftMost += 0.03;
				rightMost += 0.03;
			}
			else if (orientation == 1){   //moving Aliens to the right
				for (int i = 0; i < MAX_ENEMIES; i++){
					if (AlienArray[i] != NULL){
						AlienArray[i]->setX(AlienArray[i]->getX() + ENEMY_SPEED_X * elapsed);
					}
				}
				rightMost += ENEMY_SPEED_X * elapsed;
				leftMost += ENEMY_SPEED_X * elapsed;
			}
			else if (orientation == 0){   //moving Aliens to the left
				for (int i = 0; i < MAX_ENEMIES; i++){
					if (AlienArray[i] != NULL){
						AlienArray[i]->setX(AlienArray[i]->getX() + (-ENEMY_SPEED_X * elapsed));
					}
				}
				leftMost -= ENEMY_SPEED_X * elapsed;
				rightMost -= ENEMY_SPEED_X *elapsed;
			}


			//Alien Collision with lazer logic
			for (int i = 0; i < MAX_BULLETS; i++) {
				if (Lazers[i] != NULL){
					for (int j = 0; j < MAX_ENEMIES; j++){
						if (AlienArray[j] != NULL && Lazers[i] != NULL){
							if (Lazers[i]->getX() > AlienArray[j]->getX() - 0.12 && Lazers[i]->getX() < AlienArray[j]->getX() + 0.12 &&
								Lazers[i]->getY() - 0.03 < AlienArray[j]->getY() + 0.03 && Lazers[i]->getY() + 0.02 > AlienArray[j]->getY() - 0.01) {
								delete AlienArray[j];
								AlienArray[j] = NULL;
								delete Lazers[i];
								Lazers[i] = NULL;
							    playerScore += 100;
							}
						}
					}
				}
			}

			//Enemy Lazer logic
			if (EnemyLazerCD > 0.5f) {
				if (EnemyLazerIndex < MAX_BULLETS - 1){
					int randomEnemyIndex = rand() % (MAX_ENEMIES - 1);
					while (AlienArray[randomEnemyIndex] == NULL)
						randomEnemyIndex = rand() % (MAX_ENEMIES - 1);
					if (EnemyLazer[EnemyLazerIndex] != NULL)
						delete EnemyLazer[randomEnemyIndex];
					EnemyLazer[EnemyLazerIndex] = new Bullet(AlienArray[randomEnemyIndex]->getX(), AlienArray[randomEnemyIndex]->getY() - 0.05f, 0.5f, ssTextureID, 0.0f, 79.0f, 10.0f, 22.0f);
					EnemyLazerIndex++;
					EnemyLazerCD = 0.0f;
				}
				else {
					EnemyLazerIndex = 0.0f;
					int randomEnemyIndex = rand() % (MAX_ENEMIES - 1);
					while (AlienArray[randomEnemyIndex] == NULL)
						randomEnemyIndex = rand() % (MAX_ENEMIES - 1);
					if (EnemyLazer[EnemyLazerIndex] != NULL)
						delete EnemyLazer[randomEnemyIndex];
					EnemyLazer[EnemyLazerIndex] = new Bullet(AlienArray[randomEnemyIndex]->getX(), AlienArray[randomEnemyIndex]->getY() - 0.05f, 0.5f, ssTextureID, 0.0f, 202.0f, 9.0f, 37.0f);
					EnemyLazerIndex++;
					EnemyLazerCD = 0.0f;
				}
			}

			//updating Enemy lazer array
			for (int i = 0; i < MAX_BULLETS - 1; ++i) {
				if (EnemyLazer[i] != NULL) {
					if (EnemyLazer[i]->getY() > -0.95f) {
						EnemyLazer[i]->updatePos(-elapsed);
					}
					else if (EnemyLazer[i]->getY() < -0.95f) {
						delete EnemyLazer[i];
						EnemyLazer[i] = NULL;
					}
				}
			}
			bool defeat = true;
			//Enemy Lazer collision with player logic
			for (int i = 0; i < MAX_BULLETS - 1; ++i) {
				if (EnemyLazer[i] != NULL) {
					if (EnemyLazer[i]->getX() > playerShip.getX() - 0.14 && EnemyLazer[i]->getX() < playerShip.getX() + 0.14 && EnemyLazer[i]->getY() + 0.05 < playerShip.getY() + 0.065  && EnemyLazer[i]->getY() > -0.90){
						delete EnemyLazer[i];
						EnemyLazer[i] = NULL;
						defeat = true;
						gameCondition = LOSE;
						break;
					}
				}
			}
			// collision detection for Aliens and Playership
			for (int i = 0; i < MAX_ENEMIES - 1; ++i) {
				if (AlienArray[i] != NULL) {
					// check if Alien has collided with player reached the bottom of the screen
					if ((AlienArray[i]->getX() - 0.05f < playerShip.getX() + 0.1f && AlienArray[i]->getX() + 0.05f > playerShip.getX() - 0.1f)
						&& AlienArray[i]->getY() - 0.05f < playerShip.getY() + 0.125f) {
						defeat = true;
						gameCondition = LOSE;
						break;
					}
					else if (AlienArray[i]->getY() < -0.95f) { // Just in case
						defeat = true;
						gameCondition = LOSE;
						break;
					}
				}
			}

			for (int i = 0; i < MAX_ENEMIES - 1; ++i) {
				// check if array is empty
				if (AlienArray[i] != NULL) {
					defeat = false;
					break;
				}
			}

			//cleaning up allocated memory
			if (defeat || gameCondition == LOSE) {
				for (int i = 0; i < MAX_ENEMIES - 1; i++) {
					if (AlienArray[i] != NULL) {
						delete AlienArray[i];
						AlienArray[i] = NULL;
					}
				}
				for (int j = 0; j < MAX_BULLETS - 1; j++) {
					if (Lazers[j] != NULL) {
						delete Lazers[j];
						Lazers[j] = NULL;
					}
				}
				for (int i = 0; i < MAX_BULLETS - 1; i++) {
					if (EnemyLazer[i] != NULL) {
						delete EnemyLazer[i];
						EnemyLazer[i] = NULL;
					}
				}
				if (gameCondition != LOSE)
					gameCondition = WIN;
				state = STATE_GAME_OVER;
			}
		}
		LazerCD += elapsed;
		EnemyLazerCD += elapsed;
		break;
	case STATE_GAME_OVER:
		break;
	}
}

bool Application::updateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return true;
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN && state == STATE_MAIN_MENU) { // load into game level
			if (event.button.button == 1) {
				float xPos = (((float)event.button.x / 800.0f) * 2.666f) - 1.333f;
				float yPos = (((float)(600 - event.button.y) / 600.0f) * 2.0f) - 1.0f;
				if (xPos > -0.25f && xPos < 0.25f && yPos > -0.1f && yPos < 0.1f) {
					state = STATE_GAME_LEVEL;
					initializeGame();
				}
			}
		}
		else if (event.type == SDL_KEYDOWN && state == STATE_GAME_LEVEL && start == false) { // start game
			if (event.key.keysym.scancode == SDL_SCANCODE_S) {
				start = true;
			}
		}
	}
	update(elapsed);
	render();
	return false;
}

void Application::initializeGame() {
	// initialize varibles for the game level
	LazerCD = 0.0;
	EnemyLazerCD = 0.0;
	LazerIndex = 0;
	EnemyLazerIndex = 0;
	playerScore = 0;
	start = false;
	gameCondition = IN_PROGRESS;

	// initializing shooter
	SpriteSheet playerSprite(ssTextureID, 0.0f / 128.0f, 0.0f / 128.0f, 112.0f / 128.0f, 38.0f / 128.0f);
	playerShip.setSprite(playerSprite);
	playerShip.setX(0.0f);
	playerShip.setY(-0.825f);

	// initializing Alien array
	float startX = -0.8f;
	float startY = 0.8f;
	float counter = 0.0f;
	float increment = 0.3f;
	SpriteSheet test(ssTextureID, 0.0f / 128.0f, 39.0f / 128.0f, 100.0f / 128.0f, 40.0f / 128.0f);
	for (int i = 0; i < MAX_ENEMIES - 1; i++) {
		if (startX + counter < 0.8f) {
			AlienArray[i] = new Entity(test, startX + counter, startY);
			counter += increment;
		}
		else {
			counter = 0.0f;
			startY -= 0.2f;
			AlienArray[i] = new Entity(test, startX + counter, startY);
			counter += increment;
		}
	}
	leftMost = AlienArray[0]->getX() - 0.14f;
	rightMost = AlienArray[5]->getX() + 0.14f;
	orientation = 1;
	increasedSpeedY = 0.0;
}



void Application::shootBullet() {
	for (int i = 0; i < MAX_BULLETS - 1; ++i) {
		if (Lazers[i] != NULL) {
			if (Lazers[i]->getY() < 0.95f) {
				Lazers[i]->setVisibility(true);
				Lazers[i]->Draw();
			}
		}
	}
}

void Application::renderEnemy() {
	for (int i = 0; i < MAX_ENEMIES - 1; ++i) {
		if (AlienArray[i] != NULL) {
			AlienArray[i]->ssDraw(0.15f);
		}
	}
}

void Application::renderEnemyLazer() {
	for (int i = 0; i < MAX_BULLETS - 1; ++i) {
		if (EnemyLazer[i] != NULL) {
			if (EnemyLazer[i]->getY() > -0.95f) {
				EnemyLazer[i]->setVisibility(true);
				EnemyLazer[i]->Draw();
			}
		}
	}
}

