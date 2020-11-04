#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(char const& type) {
	id = type;
	setType(type);
	loadSprite();
	setCoords(type);
}

void SpriteSheet::setType(char const& type) {
	if (type == 'j' or type == 'k')
		this->type = SpriteType::KEY;
	else if (type == 'b' or type == 'c')
		this->type = SpriteType::ALARM;
	else if (type >= 'l' and type <= 'q')
		this->type = SpriteType::MONEY;
	else if (type >= 'x' and type <= 'z')
		this->type = SpriteType::ARROW;
	else if (type >= '1' and type <= '8')
		this->type = SpriteType::WALL;
	else if (type == 'a')
		this->type = SpriteType::NOTHING;
	else
		this->type = SpriteType::BLOCK;
}

void SpriteSheet::setType(SpriteType type)
{
	this->type = type;
}

void SpriteSheet::loadSprite() {
	switch (type) {
	case SpriteType::KEY:
		tilesheet.loadFromFile("images/keyOriginal.png", TEXTURE_PIXEL_FORMAT_RGBA);
		width = 1;
		height = 2;
		break;
	case SpriteType::MONEY:
		tilesheet.loadFromFile("images/finalStage.png", TEXTURE_PIXEL_FORMAT_RGBA);
		width = 3;
		height = 2;
		break;
	case SpriteType::ALARM:
		tilesheet.loadFromFile("images/alarm.png", TEXTURE_PIXEL_FORMAT_RGBA);
		width = 1;
		height = 2;
		break;
	case SpriteType::ARROW:
		tilesheet.loadFromFile("images/arrows.png", TEXTURE_PIXEL_FORMAT_RGBA);
		width = 3;
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

void SpriteSheet::setCoords(char const& c) {
	switch (c) {
	case '1': //pink brick wall top
		texCoordTile[0] = glm::vec2(float(1) / width, float(21) / height);
		break;
	case '2': //pink brick wall bottom
		texCoordTile[0] = glm::vec2(float(1) / width, float(22) / height);
		break;
	case '3': //gray neon wall top
		texCoordTile[0] = glm::vec2(float(4) / width, float(23) / height);
		break;
	case '4': //gray neon wall bottom
		texCoordTile[0] = glm::vec2(float(4) / width, float(24) / height);
		break;
	case '5': //cyan puzzle wall top
		texCoordTile[0] = glm::vec2(float(4) / width, float(9) / height);
		break;
	case '6': //cyan puzzle wall bottom
		texCoordTile[0] = glm::vec2(float(4) / width, float(10) / height);
		break;
	case '7': //half cyan
		texCoordTile[0] = glm::vec2(float(1) / width, float(11) / height);
		break;
	case '8': //half gray
		texCoordTile[0] = glm::vec2(float(3) / width, float(11) / height);
		break;
	case '9': //half pink
		texCoordTile[0] = glm::vec2(float(1) / width, float(13) / height);
		break;
	case 'a': //nothing
		texCoordTile[0] = glm::vec2(float(10) / width, float(0) / height);
		break;
	case 'b': //alarm top
		texCoordTile[0] = glm::vec2(float(0) / width, float(0) / height);
		break;
	case 'c': //alarm bottom
		texCoordTile[0] = glm::vec2(float(0) / width, float(1) / height);
		break;
	case 'd': //vermell
		texCoordTile[0] = glm::vec2(float(0) / width, float(1) / height);
		hits = 5;
		//texCoordTile[0] += halfTexel;
		break;
	case 'e'://blau
		texCoordTile[0] = glm::vec2(float(2) / width, float(1) / height);
		hits = 1;
		//texCoordTile[0] += halfTexel;
		break;
	case 'f'://green
		texCoordTile[0] = glm::vec2(float(3) / width, float(0) / height);
		//texCoordTile[0] += halfTexel;
		hits = 4;
		break;
	case 'g'://white
		texCoordTile[0] = glm::vec2(float(1) / width, float(1) / height);
		hits = 3;
		break;
	case 'h': //pink
		texCoordTile[0] = glm::vec2(float(1) / width, float(0) / height);
		hits = 2;
		break;
	case 'j'://key top
		texCoordTile[0] = glm::vec2(float(0) / width, float(0) / height);
		break;
	case 'k'://key bottom
		texCoordTile[0] = glm::vec2(float(0) / width, float(1) / height);
		break;
	case 'l'://pouch top
		texCoordTile[0] = glm::vec2(float(0) / width, float(0) / height);
		break;
	case 'm'://pouch bottom
		texCoordTile[0] = glm::vec2(float(0) / width, float(1) / height);
		break;
	case 'n'://coin top
		texCoordTile[0] = glm::vec2(float(1) / width, float(0) / height);
		break;
	case 'o'://coin bottom
		texCoordTile[0] = glm::vec2(float(1) / width, float(1) / height);
		break;
	case 'p'://calculator top
		texCoordTile[0] = glm::vec2(float(2) / width, float(0) / height);
		break;
	case 'q'://calculator bottom
		texCoordTile[0] = glm::vec2(float(2) / width, float(1) / height);
		break;
	case 'x': // pink arrow
		texCoordTile[0] = glm::vec2(float(0) / width, float(0) / height);
		break;
	case 'y': // gray arrow
		texCoordTile[0] = glm::vec2(float(1) / width, float(0) / height);
		break;
	case 'z': // blue arrow
		texCoordTile[0] = glm::vec2(float(2) / width, float(0) / height);
		break;
	default:
		break;
	}

	glm::vec2 halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	glm::vec2 tileTexSize = glm::vec2(1.f / width, 1.f / height);
	texCoordTile[1] = texCoordTile[0] + tileTexSize;
	texCoordTile[1] -= halfTexel;
}
int SpriteSheet::getHeight() {
	return height;
}

int SpriteSheet::getWidth() {
	return width;
}

Texture SpriteSheet::getTexture() {
	return tilesheet;
}

glm::vec2 SpriteSheet::getTexCoord0() {
	return texCoordTile[0];
}

glm::vec2 SpriteSheet::getTexCoord1() {
	return texCoordTile[1];
}

SpriteType SpriteSheet::getType() {
	return type;
}

char SpriteSheet::getId() {
	return id;
}

int SpriteSheet::getHits() {
	return hits;
}