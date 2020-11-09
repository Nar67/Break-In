#ifndef _VIGILANT_INCLUDE
#define _VIGILANT_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include "SoundManager.h"

class Vigilant
{
public:
	Vigilant();
	~Vigilant();
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void setPlayer(Player* player);
	void setStop(bool stop);

private:
	bool collidedWithPlayer();

private:
	bool stop = true;
	glm::ivec2 tileMapDispl, posVigilant;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	Player* player;
	SoundManager sound;
};

#endif // !_VIGILANT_INCLUDE