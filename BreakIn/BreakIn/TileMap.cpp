#include <iostream>
#include "TileMap.h"

using namespace std;


TileMap* TileMap::createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	TileMap* map = new TileMap(levelFile, minCoords, program);

	return map;
}


TileMap::TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	loadLevel(levelFile, program);
}

TileMap::~TileMap()
{
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
			if (map[j * mapSize.x + i] != NULL)
				map[j * mapSize.x + i]->free();
	}
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
			map[j * mapSize.x + i]->render();
	}
	glDisable(GL_TEXTURE_2D);
	
}

void TileMap::free()
{
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
			map[j * mapSize.x + i]->free();
	}
}

vector<Tile*> TileMap::getTiles() {
	return map;
}

void TileMap::changeRoom() {
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
			map[j * mapSize.x + i]->changeRoom();
	}
}

glm::ivec2 TileMap::getMapSize()
{
	return mapSize;
}

int TileMap::getBlockSize()
{
	return blockSize;
}

bool TileMap::loadLevel(const string& levelFile, ShaderProgram& program)
{
	ifstream file;
	char tile;

	blockSize = 28;

	file.open(levelFile.c_str());
	if (!file.is_open())
		return false;
	file >> mapSize.x >> mapSize.y;
	
	map.resize(mapSize.x * mapSize.y);

	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			file >> tile;
			map[j * mapSize.x + i] = new Tile(i, j, tile, program);
			map[j * mapSize.x + i]->init();
		}
#ifndef _WIN32
		file.get(tile);
#endif
	}

	file.close();

	return true;
}

void TileMap::printMap()
{
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			SpriteType type = map[j * mapSize.x + i]->getType();
			switch (type) {
			case SpriteType::KEY:
    		    cout << "-KEY";
				break;
    		case SpriteType::WALL:
    		    cout << "-WALL";
				break;
			case SpriteType::BLOCK:
				cout << "-BLOCK";
				break;
			default:
    		    cout << "-ELSE";
				break;
			}
		}
		cout << endl;
	}
	cout << endl;
}

