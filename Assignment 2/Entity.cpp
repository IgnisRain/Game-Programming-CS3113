#include "Entity.h"


Entity::Entity(float x, float y, float rotation, GLuint textureID, float width, float height, float xDirect, float yDirect, float speed) 
	: x(x), y(y), rotation(rotation), textureID(textureID), width(width), height(height), xDirect(xDirect), yDirect(yDirect), speed(speed) {}

void Entity::Draw(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	GLfloat quad[] = {-width * 0.5f, height * 0.5f, -width * 0.5f, -height * 0.5f, width * 0.5f, -height * 0.5f, width * 0.5f, height * 0.5f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
};

float Entity::getX(){ return x; }

float Entity::getY(){ return y; }

float Entity::getXDirec(){ return xDirect; }

float Entity::getYDirec(){ return yDirect; }

float Entity::getSpeed(){ return speed; }

float Entity::updateX(){ return speed * xDirect; }

float Entity::updateY(){ return speed * yDirect; }

void Entity::setX(float newX){ x = newX; }

void Entity::setY(float newY){ y = newY; }

void Entity::setNewDirec(float newX, float newY){ 
	xDirect = newX;
	yDirect = newY; 
}









