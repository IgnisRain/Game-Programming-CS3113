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

	float u = (float)(((int)sprite.index) % sprite.spriteCountX) / (float)sprite.spriteCountX;
	float v = (float)(((int)sprite.index) / sprite.spriteCountX) / (float)sprite.spriteCountY;
	float spriteWidth = 1.0f / (float)sprite.spriteCountX;
	float spriteHeight = 1.0f / (float)sprite.spriteCountY;

	if (sprite.visible && !flip)
	{
		glMatrixMode(GL_MODELVIEW);
		glBindTexture(GL_TEXTURE_2D, sprite.getTextureID());
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPushMatrix();
		glTranslatef(x, y, 0.0);

		GLfloat quad[] = { -TILE_SIZE, TILE_SIZE, -TILE_SIZE, -TILE_SIZE, TILE_SIZE, -TILE_SIZE, TILE_SIZE, TILE_SIZE };
		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);

		GLfloat uvs[] = { u, v, u, v + spriteHeight, u + spriteWidth, v + spriteHeight, u + spriteWidth, v };
		glTexCoordPointer(2, GL_FLOAT, 0, uvs);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisableClientState(GL_COLOR_ARRAY);

		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	else if (sprite.visible && flip)
	{
		glMatrixMode(GL_MODELVIEW);
		glBindTexture(GL_TEXTURE_2D, sprite.getTextureID());
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPushMatrix();
		glTranslatef(x, y, 0.0);

		GLfloat quad[] = { TILE_SIZE, TILE_SIZE, TILE_SIZE, -TILE_SIZE, -TILE_SIZE, -TILE_SIZE, -TILE_SIZE, TILE_SIZE };
		glVertexPointer(2, GL_FLOAT, 0, quad);
		glEnableClientState(GL_VERTEX_ARRAY);

		GLfloat uvs[] = { u, v, u, v + spriteHeight, u + spriteWidth, v + spriteHeight, u + spriteWidth, v };
		glTexCoordPointer(2, GL_FLOAT, 0, uvs);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisableClientState(GL_COLOR_ARRAY);

		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
}

void Entity::FixedUpdate()
{
	if (!isStatic && sprite.visible)
	{
		velocityX = lerp(velocityX, 0.0f, FIXED_TIMESTEP * frictionX);
		velocityY = lerp(velocityY, 0.0f, FIXED_TIMESTEP * frictionY);
		velocityX += accelerationX * FIXED_TIMESTEP;
		velocityY += accelerationY * FIXED_TIMESTEP;

		y += velocityY * FIXED_TIMESTEP;
		x += velocityX * FIXED_TIMESTEP;
	}
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

float Entity::getHeight(){
	return 1.0f / (float)sprite.spriteCountY;
}

float Entity::getWidth(){
	return 1.0f / (float)sprite.spriteCountX;
}



bool Entity::collidesWith(Entity* other)
{
	float spriteWidth = 1.0f / (float)sprite.spriteCountX;
	float spriteHeight = 1.0f / (float)sprite.spriteCountY;

	float thisTop = this->y + spriteHeight / 2.0f;
	float thisBottom = this->y - spriteHeight / 2.0f;
	float thisLeft = this->x - spriteWidth / 2.0f;
	float thisRight = this->x + spriteWidth / 2.0f;
	float otherTop = other->y + other->getHeight() / 2.0f;
	float otherBottom = other->y - other->getHeight() / 2.0f;
	float otherLeft = other->x - other->getWidth() / 2.0f;
	float otherRight = other->x + other->getWidth() / 2.0f;

	if (thisBottom > otherTop || thisTop < otherBottom || thisLeft > otherRight || thisRight < otherLeft)
		return false;
	else
		return true;
}

void Entity::ssDraw() {
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);
	sprite.draw(0.5);
	glPopMatrix();
}








