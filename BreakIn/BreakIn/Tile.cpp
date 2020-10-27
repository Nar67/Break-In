#include "Tile.h"
#include "SpriteManager.h"

Tile::Tile(int x, int y, char const& tileType, ShaderProgram& shprog) {
	xPos = x;
	yPos = y;
	sprite = SpriteManager::getSprite(tileType);
	program = shprog;
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
	
	int blockSize = 28;
	glm::vec2 posTile = glm::vec2(xPos * blockSize,yPos * blockSize / 2 - blockSize+2);

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

