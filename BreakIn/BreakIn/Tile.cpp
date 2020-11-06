#include "Tile.h"
#include "SpriteManager.h"

const int blockSize = 28;

Tile::Tile(int x, int y, char const& tileType, ShaderProgram& shprog) {
	xPos = x;
	yPos = y;
	sprite = SpriteManager::getSprite(tileType);
	program = shprog;
	room = offset = 0;
	hits = setHits();
	nRoom = false;
	points = setPoints();
}

void Tile::init() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	sendVertices();
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void Tile::free() {
	glDeleteBuffers(1, &vbo);
}

void Tile::calculateVertices() {
	vertices.clear();
	
	posTile = glm::vec2(xPos * blockSize,yPos * blockSize / 2 + offset);

	if (nRoom) {
		if (offset < room*446)
			offset += 32;
		else nRoom = false;
	}

	glm::vec2 texCoordTile[2];
	texCoordTile[0] = sprite->getTexCoord0();
	texCoordTile[1] = sprite->getTexCoord1();
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

void Tile::sendVertices() {
	calculateVertices();
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
}

SpriteType Tile::getType() {
	return sprite->getType();
}

void Tile::render() {
	sendVertices();
	glBindVertexArray(vao);
	sprite->getTexture().use();
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Tile::nextRoom() {
	if (room < 2) {
		room++;
		nRoom = true;
	}
}

glm::vec2 Tile::getPosition()
{
	return posTile;
}

glm::vec2 Tile::getIndex() {
	glm::ivec2 pos = glm::vec2(xPos, yPos);
	return pos;
}

int Tile::getBlockSize()
{
	return blockSize;

}

int Tile::setHits() {
	return sprite->getHits();
}

int Tile::setPoints() {
	return sprite->getPoints();
}

int Tile::decreaseHits() {
	return --hits;
}

int Tile::getPoints() {
	return points;
}