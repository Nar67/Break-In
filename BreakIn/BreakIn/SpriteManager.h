#ifndef _SPRITE_MANAGER_INCLUDE
#define _SPRITE_MANAGER_INCLUDE

#include "SpriteSheet.h"
#include <vector>

class SpriteManager
{
public:
	static void createSprites();
	static SpriteSheet* getSprite(char type);

	static vector<SpriteSheet*> sprites;

private:
	//static SpriteSheet* sprite;
	//vector<char> types;

};
#endif // !_SPRITE_MANAGER_INCLUDE

