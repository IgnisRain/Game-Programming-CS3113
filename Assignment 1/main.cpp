
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);  //loading pictures
	GLuint textureID;
	glGenTextures(1, &textureID); //generates new OpenGL texture ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels); //sets texture data of specified target. Image format must be GL_RGBA or GL_RGB depending on the file type
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //melds the pixels for smoother effect
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //keeps things sharp and crisp

	SDL_FreeSurface(surface);
	return textureID;
}


void DrawSprite(GLuint texture, float x, float y, float rotation) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	GLfloat quad[] = { -0.200f, 0.200f, -0.200f, -0.200f, 0.200f, -0.200f, 0.200f, 0.200f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

void DrawSprite2(GLuint texture, float x, float y, float rotation) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	GLfloat quad[] = { -0.060f, 0.060f, -0.060f, -0.060f, 0.060f, -0.060f, 0.060f, 0.060f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}



int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	bool done = false;

	SDL_Event event;

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, 0.0);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);  //multiplies current matrix



	GLuint texture;
	GLuint texture2;
	GLuint texture3;

	texture = LoadTexture("gear.png");
	texture2 = LoadTexture("shocked.png");
	texture3 = LoadTexture("rocket.png");


	float lastFrameTicks = 0.0f;
	float increment = 0.0f;

	while (!done) {

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}

		}
		glClearColor(1.0, 1.0, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_QUADS, 0, 4);
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		increment += ticks;
		DrawSprite(texture, -0.1, 0.4, lastFrameTicks * 100.0);
		DrawSprite(texture, -0.7, -0.4, -lastFrameTicks * 100.0);
		DrawSprite2(texture2, (increment/200)-0.9, 0.0, 0.0);
		DrawSprite(texture3, (increment/200)-1.3, 0.0, 0.0);

		glLoadIdentity();
		GLfloat redblock[] = { 0.25f, 0.25f, -0.25f, 0.25f, -0.25f, 0.1f, 0.25f, 0.1f };
		glVertexPointer(2, GL_FLOAT, 0, redblock);
		glEnableClientState(GL_VERTEX_ARRAY);
		GLfloat blockColor[] = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
		glColorPointer(3, GL_FLOAT, 0, blockColor);
		glEnableClientState(GL_COLOR_ARRAY);
		glTranslated(1.0, 0.0, 0.0);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		glDrawArrays(GL_QUADS,0,4);
		glDisableClientState(GL_COLOR_ARRAY);
		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}