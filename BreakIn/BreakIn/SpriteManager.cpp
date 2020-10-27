#include "SpriteManager.h"
#include <iostream>

vector<SpriteSheet*> SpriteManager::sprites;

void SpriteManager::createSprites() {

	
	vector<char> types = { 'a', 'b', 'c', 'd', 'e','f', 'g', 'j', 'k', 'o', 'p', 'l' };
	sprites.resize(types.size());
	for (int i = 0; i < types.size(); ++i) {
		SpriteSheet* sprite = new SpriteSheet(types[i]);
		sprites[i] = sprite;
	}
}

SpriteSheet* SpriteManager::getSprite(char type) {
	for (SpriteSheet* s : sprites) {
		if (s->getId() == type)
			return s;
	}
}


