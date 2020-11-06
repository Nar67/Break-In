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
	this->program = program;
	this->levelFile = levelFile;
	loadLevel();
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

void TileMap::nextRoom() {
	room++;
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
			map[j * mapSize.x + i]->nextRoom();
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


bool TileMap::loadLevel()
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

void TileMap::removeTile(Tile* tile)
{
	if(tile->getType() == SpriteType::BLOCK)
    {
		int hits = tile->decreaseHits();
		if (hits == 0) {
			glm::ivec2 pos = tile->getIndex();
			map[pos.y * mapSize.x + pos.x]->free();
			map[pos.y * mapSize.x + pos.x] = new Tile(pos.x, pos.y, 'a', program);
			map[pos.y * mapSize.x + pos.x]->init();
		}
    }

	if (tile->getType() == SpriteType::KEY) {
		glm::ivec2 pos = tile->getIndex();
		map[pos.y * mapSize.x + pos.x]->free();
		map[pos.y * mapSize.x + pos.x] = new Tile(pos.x, pos.y, 'a', program);
		map[pos.y * mapSize.x + pos.x]->init();
		int newpos = pos.y + 1;
		if (map[pos.y+1 * mapSize.x + pos.x]->getType() == SpriteType::KEY) newpos = pos.y - 1;
		map[newpos * mapSize.x + pos.x]->free();
		map[newpos * mapSize.x + pos.x] = new Tile(pos.x, newpos,'a', program);
		map[newpos * mapSize.x + pos.x]->init();
		
		openPath();
	}
}

void TileMap::openPath() {
	char c = levelFile[levelFile.length() - 5];
	char tile;
	switch (c)
	{
	case '2':
		tile = 'y';
		break;
	case '3':
		tile = 'z';
		break;
	default:
		tile = 'x';
		break;
	}
	int j = 64 - room*32;
	for (int i = 5; i < 11; ++i) {
		map[j * mapSize.x + i]->free();
		map[j * mapSize.x + i] = new Tile(i, j, 'a', program);
		map[j * mapSize.x + i]->init();
		map[(j+1) * mapSize.x + i]->free();
		map[(j+1) * mapSize.x + i] = new Tile(i, (j+1), tile, program);
		map[(j+1) * mapSize.x + i]->init();
	}
}

void TileMap::setRoom(int r) {
	room = r-1;
}

void TileMap::printMap()
{
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			switch (map[j * mapSize.x + i]->getType()) {
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

