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
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string& levelFile, ShaderProgram& program)
{
	ifstream file;
	char tile;

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
		fin.get(tile);
#endif
	}

	file.close();

	return true;
}
