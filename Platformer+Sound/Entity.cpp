#include "Entity.h"

Entity::Entity(float x, float y, float rotation, GLuint textureID, float width, float height)
	: x(x), y(y), rotation(rotation), textureID(textureID), width(width), height(height) {
	velocityX = 0.0f;
	velocityY = 0.0f;
	accelerationX = 0.0f;
	accelerationY = 0.0f;
	frictionX = 2.0f;
	frictionY = 2.0f;
}

Entity::Entity() {}

void Entity::Draw() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	GLfloat quad[] = { -width * 0.5f, height * 0.5f, -width * 0.5f, -height * 0.5f, width * 0.5f, -height * 0.5f, width * 0.5f, height * 0.5f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

void Entity::ssDraw(float scale){
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);
	sprite.draw(scale);
}

void Entity::setX(float newX){ x = newX; }

void Entity::setY(float newY){ y = newY; }

void Entity::setWidth(float w) { width = w; }

void Entity::setHeight(float h) { height = h; }

void Entity::setAccelerationX(float newA) { accelerationX = newA; }

void Entity::setAccelerationY(float newA) { accelerationY = newA; }

void Entity::setVelocityX(float newV) { velocityX = newV; }

void Entity::setVelocityY(float newV) { velocityY = newV; }

void Entity::setFrictionX(float newF) { frictionX = newF; }

void Entity::setFrictionY(float newf) { frictionY = newf; }

void Entity::setSprite(SpriteSheet temp){ sprite = temp; }

float Entity::getVelocityX() { return velocityX; }

float Entity::getVelocityY() { return velocityY; }

float Entity::getAccelerationX() { return accelerationX; }

float Entity::getAccelerationY() { return accelerationY; }

float Entity::getFrictionX() { return frictionX; }

float Entity::getFrictionY() { return frictionY; }

float Entity::getX(){ return x; }

float Entity::getY() { return y; }

float Entity::getHeight() { return height; }

float Entity::getWidth() { return width; }



bool Entity::collidesWith(Entity* other)
{
	float thisTop = this->getY() + (this->getHeight() * 0.3);
	float thisBottom = this->getY() - (this->getHeight()* 0.3);
	float thisLeft = this->getX() - (this->getWidth()* 0.3);
	float thisRight = this->getX() + (this->getWidth()* 0.3);
	float otherTop = other->getY() + (other->getHeight() * 0.3);
	float otherBottom = other->getY() - (other->getHeight()* 0.3);
	float otherLeft = other->getX() - (other->getWidth() * 0.3);
	float otherRight = other->getX() + (other->getWidth()* 0.3);

	if (thisBottom > otherTop || thisTop < otherBottom || thisLeft > otherRight || thisRight < otherLeft)
		return false;
	else
		return true;
}







