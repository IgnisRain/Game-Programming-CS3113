#include "Bullet.h"

Bullet::Bullet(float x, float y, float speed, GLuint textureID, float spriteX, float spriteY, float spriteW, float spriteH)
	: x(x), y(y), speed(speed), visible(false) {
	SpriteSheet temp(textureID, spriteX / 128.0f, spriteY / 128.0f, spriteW / 128.0f, spriteH / 128.0f);
	sprites = temp;
}

void Bullet::Draw() {
	if (!visible)
		return;
	glLoadIdentity();
	glTranslatef(x, y, 0.0f);
	sprites.draw(0.05f);
}

void Bullet::setVisibility(bool vis){ visible = vis; }

void Bullet::updatePos(float elapsed) { y += speed * elapsed; }

float Bullet::getX() { return x; }

float Bullet::getY() { return y; }