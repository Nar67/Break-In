#ifndef _SPRITESHEET_INCLUDE
#define _SPRITESHEET_INCLUDE

#include "Texture.h"
#include <string>
#include <glm/glm.hpp>

enum class SpriteType {BLOCK, PLATFORM, KEY, WALL};

class SpriteSheet
{
public:
	SpriteSheet() = default;
	SpriteSheet(char const& type);
	void setType(char const& type);
	void loadSprite();
	int getWidth();
	int getHeight();
	void setCoords(char const& c);
	char getId();


	Texture getTexture();
	glm::vec2 getTexCoord0();
	glm::vec2 getTexCoord1();
	SpriteType getType();
	
private:
	SpriteType type;
	Texture tilesheet;
	int width, height;
	glm::vec2 texCoordTile[2];
	char id;

};
#endif

