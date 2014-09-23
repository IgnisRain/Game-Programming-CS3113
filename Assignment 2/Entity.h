#include <SDL.h>
#include <SDL_opengl.h>
#include<SDL_image.h>

class Entity{
public:

	Entity(float, float, float, GLuint, float, float, float = 0, float = 0, float = 0);
	void Draw();
	float getX();
	float getY(); 
	float getXDirec();
	float getYDirec();
	float getSpeed();
	float updateX();
	float updateY();
	void setX(float newX);
	void setY(float newY);
	void setNewDirec(float X, float Y);
private:

	float x;
	float y;
	float rotation;
	float width;
	float height;
	float speed;
	float xDirect;
	float yDirect;
	GLuint textureID;


};