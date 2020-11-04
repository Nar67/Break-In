#include "SpriteManager.h"
#include <iostream>

vector<SpriteSheet*> SpriteManager::sprites;

void SpriteManager::createSprites() {


	vector<char> types = { '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e','f', 'g', 'h', 'j', 'k', 'l', 'm',
							'n', 'o', 'p', 'q', 'x', 'y', 'z'};
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


