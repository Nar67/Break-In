#include "Sprite.h"

Sprite::Sprite(char const& type){
	setType(type);
	loadSprite();
	setCoords(type);
}

void Sprite::setType(char const& type) {
	if (type == 'j' or type == 'k')
		this->type = SpriteType::KEY;
	else if (type == 'l')
		this->type = SpriteType::BALL;
	else if (type == 'o' or type == 'p')
		this->type = SpriteType::PLATFORM;
	else
		this->type = SpriteType::BLOCK;
}

void Sprite::loadSprite() {
	switch (type) {
	case SpriteType::KEY:
		tilesheet.loadFromFile("images/key01.png", TEXTURE_PIXEL_FORMAT_RGBA);
		width = 1;
		height = 2;
		break;
	case SpriteType::BALL:
		tilesheet.loadFromFile("images/ball.png", TEXTURE_PIXEL_FORMAT_RGBA);
		width = 1;
		height = 1;
		break;
	case SpriteType::PLATFORM:
		tilesheet.loadFromFile("images/platform07-1.png", TEXTURE_PIXEL_FORMAT_RGBA);
		width = 2;
		height = 1;
		break;
	default:
		tilesheet.loadFromFile("images/sprite-sheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
		width = 12;
		height = 27;
		break;
	}
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
}

void Sprite::setCoords(char const& c) {
	switch (c) {
	case 'a': //nothing
		texCoordTile[0] = glm::vec2(float(10) / width, float(0) / height);
		break;
	case 'b': //brick top
		texCoordTile[0] = glm::vec2(float(1) / width, float(21) / height);
		break;
	case 'c': // brick bottom
		texCoordTile[0] = glm::vec2(float(1) / width, float(22) / height);
		break;
	case 'd': //vermell
		texCoordTile[0] = glm::vec2(float(0) / width, float(1) / height);
		//texCoordTile[0] += halfTexel;
		break;
	case 'e'://blau
		texCoordTile[0] = glm::vec2(float(2) / width, float(1) / height);
		//texCoordTile[0] += halfTexel;
		break;
	case 'f'://verd
		texCoordTile[0] = glm::vec2(float(3) / width, float(0) / height);
		//texCoordTile[0] += halfTexel;
		break;
	case 'g'://blanc
		texCoordTile[0] = glm::vec2(float(1) / width, float(1) / height);
		break;
	case 'j'://key top
		texCoordTile[0] = glm::vec2(float(0) / width, float(0) / height);
		break;
	case 'k'://key bottom
		texCoordTile[0] = glm::vec2(float(0) / width, float(1) / height);
		break;
	case 'o'://platform left
		texCoordTile[0] = glm::vec2(float(0) / width, float(0) / height);
		break;
	case 'p'://platform right
		texCoordTile[0] = glm::vec2(float(1) / width, float(0) / height);
		break;
	case 'l'://ball bottom
		texCoordTile[0] = glm::vec2(float(0) / width, float(0) / height);
		break;
	default:
		break;
	}

	glm::vec2 halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	glm::vec2 tileTexSize = glm::vec2(1.f / width, 1.f / height);
	texCoordTile[1] = texCoordTile[0] + tileTexSize;
	texCoordTile[1] -= halfTexel;
}
int Sprite::getHeight() {
	return height;
}

int Sprite::getWidth() {
	return width;
}

Texture Sprite::getTexture() {
	return tilesheet;
}

glm::vec2 Sprite::getTexCoord0() {
	return texCoordTile[0];
}

glm::vec2 Sprite::getTexCoord1() {
	return texCoordTile[1];
}