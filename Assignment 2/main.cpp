
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Entity.h"

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path, GLuint imageFormat) {
	SDL_Surface *surface = IMG_Load(image_path);  //loading pictures
	GLuint textureID;
	glGenTextures(1, &textureID); //generates new OpenGL texture ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //melds the pixels for smoother effect
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //keeps things sharp and crisp

	SDL_FreeSurface(surface);
	return textureID;
}

void setup(){
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}

bool ProcessEvents(SDL_Event& event){  //while loop
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return true;
		}
	}
	return false;
}

void Update(float& p1y, float& p2y, bool& scored, float elapsed, const Uint8* keys, Entity& p1, Entity& p2, Entity& ball, int& p1Score, int& p2Score){
	float paddle1Y = p1.getY();
	float paddle2Y = p2.getY();
	float ballY = ball.getY();
	float ballX = ball.getX();

	p1y = 0.0;
	p2y = 0.0;

	if (keys[SDL_SCANCODE_W]) { p1y = 1.3f * elapsed; }
	if (keys[SDL_SCANCODE_S]) { p1y = -1.3f * elapsed; }
	if (keys[SDL_SCANCODE_UP]) { p2y = 1.3f * elapsed; }
	if (keys[SDL_SCANCODE_DOWN]) { p2y = -1.3f * elapsed; }
	if (keys[SDL_SCANCODE_SPACE]) {
		scored = false;
		ball.setNewDirec(0.5,0.1);
	}

	// paddle movement validation
	if (p1.getY() + p1y > -0.83 && p1.getY() + p1y < 0.83) //makes sure p1 doesn't go off the screen
		p1.setY(paddle1Y += p1y);
	if (p2.getY() + p2y > -0.83 && p2.getY() + p2y < 0.83) //makes sure p2 doesn't go off the screen
		p2.setY(paddle2Y += p2y);

	//ball collision with top and bottom of screen
	if (scored == false){  
		if (ball.getY() > 0.95){
			if (ball.getYDirec() > 0)  //make sure the ball is travelling in that direction before reversing direction
				ball.setNewDirec(ball.getXDirec(), -(ball.getYDirec()));
		}
		else if (ball.getY() < -0.95){
			if (ball.getYDirec() < 0)
				ball.setNewDirec(ball.getXDirec(), -(ball.getYDirec()));
		}

		//ball collision with paddle
		if (((ball.getX() - 0.05f) < (p1.getX() + 0.05f)) &&  
			((ball.getY() + 0.05f) < (p1.getY() + 0.2f) && 
			(ball.getY() - 0.05f) > (p1.getY() - 0.2f))) {
			if (ball.getXDirec() < 0) //make sure the ball is travelling in that direction before reversing direction
				ball.setNewDirec(-(ball.getXDirec()), ball.getYDirec());
		}
		else if (((ball.getX() + 0.05f) > (p2.getX() - 0.05f)) && 
			((ball.getY() + 0.05f) < (p2.getY() + 0.2f) && 
			(ball.getY() - 0.05f) > (p2.getY() - 0.2))) {
			if (ball.getXDirec() > 0)
				ball.setNewDirec(-(ball.getXDirec()), ball.getYDirec());
		}

		//ball win
		else if (ball.getX() > 1.30f) {  //resetting the ball
			ballX = 0;
			ballY = 0;
			ball.setNewDirec(0.0f, 0.0f);
			scored = true;
			++p1Score;
		}
		else if (ball.getX() < -1.30f) {
			ballX = 0;
			ballY = 0;
			ball.setNewDirec(0.0f, 0.0f);
			scored = true;
			++p2Score;
		}
	}
	else if (scored == true){
		ball.setNewDirec(0.0f, 0.0f);
	}
	//ball movement
	ball.setX(ballX + ball.getXDirec() * ball.getSpeed() * elapsed);  
	ball.setY(ballY + ball.getYDirec() * ball.getSpeed() * elapsed);
}

void Render(Entity& p1, Entity& p2, Entity& ball, int& p1Score, int& p2Score){
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	GLfloat midLine[] = { -0.01f, 1.0f, -0.01f, -1.0f, 0.01f, -1.0f, 0.01f, 1.0f };
	glVertexPointer(2, GL_FLOAT, 0, midLine);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat midColor[] = { 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f };
	glColorPointer(3, GL_FLOAT, 0, midColor);
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	if (p1Score != 3 && p2Score != 3){
		ball.Draw();
		p1.Draw();
		p2.Draw();
	}

	SDL_GL_SwapWindow(displayWindow);
}

int main(int argc, char *argv[])
{

	setup();  //sets up window and matricies
	float lastFrameTicks = 0.0f;
	float p1y = 0.0;
	float p2y = 0.0;
	bool done = false;
	bool scored = true;
	int p1Score = 0;
	int p2Score = 0;
	SDL_Event event;

	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	Entity gPaddle(1.25f, 0.0f, 0.0f, LoadTexture("greenPaddle.png", GL_BGRA), 0.1f, 0.3f);  //loading textures
	Entity rPaddle(-1.25f, 0.0f, 0.0f, LoadTexture("redPaddle.png", GL_BGRA), 0.1f, 0.3f);
	Entity ball(0.0f, 0.0f, 0.0f, LoadTexture("Ccube.png", GL_BGRA), 0.1f, 0.1f, 0.0f, 0.0f, 4.0f);

	while (!ProcessEvents(event)) {
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		Update(p1y, p2y, scored, elapsed, keys, rPaddle, gPaddle, ball, p1Score, p2Score);
		Render(rPaddle, gPaddle, ball, p1Score, p2Score);
	}
	SDL_Quit();
	return 0;
}
