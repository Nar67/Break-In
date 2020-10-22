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
	//prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
			if (map[j * mapSize.x + i] != NULL)
				map[j * mapSize.x + i]->free();
	}
	/*if (map != NULL)
		delete map;*/
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
			
	/*
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	;*/
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
	//map = new vector<Tile*>[mapSize.x * mapSize.y];

	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			file >> tile;
			map[j * mapSize.x + i] = new Tile(i, j, tile, program);
			map[j * mapSize.x + i]->init();
			//map[j * mapSize.x + i] = tile;
		}
#ifndef _WIN32
		fin.get(tile);
#endif
	}

	file.close();

	return true;
}

void TileMap::prepareArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			tile = 'a';
			if (tile != 'a') {
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize / 2);

				switch (tile) {
				case 'b': //brick top
					texCoordTile[0] = glm::vec2(float(1) / tilesheetSize.x, float(21) / tilesheetSize.y);
					break;
				case 'c': // brick bottom
					texCoordTile[0] = glm::vec2(float(1) / tilesheetSize.x, float(22) / tilesheetSize.y);
					break;
				case 'd': //vermell
					texCoordTile[0] = glm::vec2(float(0) / tilesheetSize.x, float(1) / tilesheetSize.y);
					//texCoordTile[0] += halfTexel;
					break;
				case 'e'://blau
					texCoordTile[0] = glm::vec2(float(2) / tilesheetSize.x, float(1) / tilesheetSize.y);
					//texCoordTile[0] += halfTexel;
					break;
				case 'f'://verd
					texCoordTile[0] = glm::vec2(float(3) / tilesheetSize.x, float(0) / tilesheetSize.y);
					//texCoordTile[0] += halfTexel;
					break;
				case 'g'://blanc
					texCoordTile[0] = glm::vec2(float(1) / tilesheetSize.x, float(1) / tilesheetSize.y);
					break;
				case 'j'://key top
					texCoordTile[0] = glm::vec2(float(0) / tilesheetSize.x, float(0) / tilesheetSize.y);
					break;
				case 'k'://key bottom
					texCoordTile[0] = glm::vec2(float(0) / tilesheetSize.x, float(1) / tilesheetSize.y);
					break;
				case 'o'://platform left
					texCoordTile[0] = glm::vec2(float(0) / tilesheetSize.x, float(0) / tilesheetSize.y);
					break;
				case 'p'://platform right
					texCoordTile[0] = glm::vec2(float(1) / tilesheetSize.x, float(0) / tilesheetSize.y);
					break;
				case 'l'://ball 
					texCoordTile[0] = glm::vec2(float(0) / tilesheetSize.x, float(0) / tilesheetSize.y);
					break;
				default:
					break;
				}
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize / 2);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize / 2);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize / 2);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}

		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	/*auto shaderM = ServiceLocator::getShaderManager();
	posLocation = shaderM->bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = shaderM->bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
	*/
}