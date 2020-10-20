#ifndef _SPRITE_INCLUDE
#define _SPRITE_INCLUDE

#include "Texture.h"
#include <string>
#include <glm/glm.hpp>

enum class SpriteType {BLOCK, PLATFORM, BALL, KEY};

class Sprite
{
public:
	Sprite() = default;
	Sprite(char const& type);
	void setType(char const& type);
	void loadSprite();
	int getWidth();
	int getHeight();
	void setCoords(char const& c);
	Texture getTexture();
	glm::vec2 getTexCoord0();
	glm::vec2 getTexCoord1();
	


private:
	SpriteType type;
	Texture tilesheet;
	int width, height;
	glm::vec2 texCoordTile[2];

};
#endif

