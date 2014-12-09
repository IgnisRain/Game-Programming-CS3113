#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(){}

SpriteSheet::SpriteSheet(GLuint textureID, int index, int spriteCountX, int spriteCountY)
	:textureID(textureID), index(index), spriteCountX(spriteCountX), spriteCountY(spriteCountY), visible(true){}

void SpriteSheet::draw(float scale) {
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	GLfloat quad[] = { -0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5 };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
	float v = (float)(((int)index) / spriteCountX) / (float)spriteCountY;
	float spriteWidth = 1.0f / (float)spriteCountX;
	float spriteHeight = 1.0f / (float)spriteCountY;

	GLfloat quadUVs[] = { u, v, u, v + height, u + width, v + height, u + width, v };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}


float SpriteSheet::getHeight() {
	return height;
}

float SpriteSheet::getWidth() {
	return width;
}

unsigned int SpriteSheet::getTextureID(){ return textureID; }
