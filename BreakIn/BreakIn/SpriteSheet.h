#ifndef _SPRITESHEET_INCLUDE
#define _SPRITESHEET_INCLUDE

#include "Texture.h"
#include <string>
#include <glm/glm.hpp>

enum class SpriteType { BLOCK, KEY, WALL, NOTHING, MONEY, ALARM, ARROW };

class SpriteSheet
{
public:
	SpriteSheet() = default;
	SpriteSheet(char const& type);

	void setType(char const& type);
	void setType(SpriteType type);
	void loadSprite();
	int getWidth();
	int getHeight();
	void setCoords(char const& c);
	Texture getTexture();
	glm::vec2 getTexCoord0();
	glm::vec2 getTexCoord1();
	SpriteType getType();
	char getId();


private:
	SpriteType type;
	char id;
	Texture tilesheet;
	int width, height;
	glm::vec2 texCoordTile[2];

};
#endif

