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
	if (alarm) {
		if (room == alarmRoom)
			sound.playAlarm();
		else
			sound.stopSound("sound/alarm.ogg");
	}
}

void TileMap::previousRoom() {
	room--;
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
			map[j * mapSize.x + i]->previousRoom();
	}
	if (alarm) {
		if (room == alarmRoom)
			sound.playAlarm();
		else
			sound.stopSound("sound/alarm.ogg");
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
			if (tile >= 'l' and tile <= 'o')
				moneyTiles++;
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
	if (tile->getType() == SpriteType::BLOCK)
	{
		char c = tile->getSprite()->getId();
		points += tile->getPoints();
		glm::ivec2 pos = tile->getIndex();
				
		char t;
		switch (c)
		{
		case 'd':
			t = 'r';
			break;
		case 'g':
			t = 's';
			break;
		case 'h':
			t = 't';
			break;
		case 't':
			t = 'u';
			break;
		default:
			t = 'a';
			break;
		}

		int offset = tile->getOffset();
		int room = tile->getRoom();
		map[pos.y * mapSize.x + pos.x]->free();
		map[pos.y * mapSize.x + pos.x] = new Tile(pos.x, pos.y, t, program);
		map[pos.y * mapSize.x + pos.x]->setOffset(offset);
		map[pos.y * mapSize.x + pos.x]->setRoom(room);
		map[pos.y * mapSize.x + pos.x]->init();
		
		
    }
	else if (tile->getType() == SpriteType::KEY) {
		glm::ivec2 pos = tile->getIndex();
		int offset = tile->getOffset();
		int room = tile->getRoom();
		map[pos.y * mapSize.x + pos.x]->free();
		map[pos.y * mapSize.x + pos.x] = new Tile(pos.x, pos.y, 'a', program);
		map[pos.y * mapSize.x + pos.x]->setOffset(offset);
		map[pos.y * mapSize.x + pos.x]->setRoom(room);
		map[pos.y * mapSize.x + pos.x]->init();

		int newpos = pos.y + 1;
		if (map[(pos.y - 1) * mapSize.x + pos.x]->getType() == SpriteType::KEY) newpos = pos.y - 1;
		offset = map[newpos * mapSize.x + pos.x]->getOffset();
		room = tile->getRoom();
		map[newpos * mapSize.x + pos.x]->free();
		map[newpos * mapSize.x + pos.x] = new Tile(pos.x, newpos, 'a', program);
		map[newpos * mapSize.x + pos.x]->setOffset(offset);
		map[newpos * mapSize.x + pos.x]->setRoom(room);
		map[newpos * mapSize.x + pos.x]->init();

		openPath();
	}
	else if (tile->getType() == SpriteType::MONEY) {
		money += tile->getPoints();
		glm::ivec2 pos = tile->getIndex();
		char c = tile->getSprite()->getId();
		int newpos = pos.y - 1;
		if (c == 'l' or c == 'n')
			newpos = pos.y + 1;

		int offset = map[pos.y * mapSize.x + pos.x]->getOffset();
		int room = map[pos.y * mapSize.x + pos.x]->getRoom();
		map[pos.y * mapSize.x + pos.x]->free();
		map[pos.y * mapSize.x + pos.x] = new Tile(pos.x, pos.y, 'a', program);
		map[pos.y * mapSize.x + pos.x]->setOffset(offset);
		map[pos.y * mapSize.x + pos.x]->setRoom(room);
		map[pos.y * mapSize.x + pos.x]->init();

		offset = map[newpos * mapSize.x + pos.x]->getOffset();
		room = map[newpos * mapSize.x + pos.x]->getRoom();
		map[newpos * mapSize.x + pos.x]->free();
		map[newpos * mapSize.x + pos.x] = new Tile(pos.x, newpos, 'a', program);
		map[newpos * mapSize.x + pos.x]->setOffset(offset);
		map[newpos * mapSize.x + pos.x]->setRoom(room);
		map[newpos * mapSize.x + pos.x]->init();
		moneyTiles -= 2;
	}
	else if (tile->getType() == SpriteType::CALCULATOR) {
		glm::ivec2 pos = tile->getIndex();
		int offset = map[pos.y * mapSize.x + pos.x]->getOffset();
		int room = map[pos.y * mapSize.x + pos.x]->getRoom();
		map[pos.y * mapSize.x + pos.x]->free();
		map[pos.y * mapSize.x + pos.x] = new Tile(pos.x, pos.y, 'a', program);
		map[pos.y * mapSize.x + pos.x]->setOffset(offset);
		map[pos.y * mapSize.x + pos.x]->setRoom(room);
		map[pos.y * mapSize.x + pos.x]->init();

		int newpos = pos.y + 1;
		if (map[(pos.y - 1) * mapSize.x + pos.x]->getType() == SpriteType::CALCULATOR) newpos = pos.y - 1;
		offset = map[newpos * mapSize.x + pos.x]->getOffset();
		room = map[newpos * mapSize.x + pos.x]->getRoom();
		map[newpos * mapSize.x + pos.x]->free();
		map[newpos * mapSize.x + pos.x] = new Tile(pos.x, newpos, 'a', program);
		map[newpos * mapSize.x + pos.x]->setOffset(offset);
		map[newpos * mapSize.x + pos.x]->setRoom(room);
		map[newpos * mapSize.x + pos.x]->init();
		calculator = true;
	}
}

bool TileMap::getCalculator() {
	return calculator;
}

void TileMap::setCalculator() {
	calculator = false;
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
	int j = 64 - room * 32;
	int offset, room;
	for (int i = 5; i < 11; ++i) {
		offset = map[j * mapSize.x + i]->getOffset();
		room = map[j * mapSize.x + i]->getRoom();
		map[j * mapSize.x + i]->free();
		map[j * mapSize.x + i] = new Tile(i, j, 'a', program);
		map[j * mapSize.x + i]->setOffset(offset);
		map[j * mapSize.x + i]->setRoom(room);
		map[j * mapSize.x + i]->init();
		offset = map[(j + 1) * mapSize.x + i]->getOffset();
		room = map[(j + 1) * mapSize.x + i]->getRoom();
		map[(j + 1) * mapSize.x + i]->free();
		map[(j + 1) * mapSize.x + i] = new Tile(i, (j + 1), tile, program);
		map[(j + 1) * mapSize.x + i]->setOffset(offset);
		map[(j + 1) * mapSize.x + i]->setRoom(room);
		map[(j + 1) * mapSize.x + i]->init();
	}
}

void TileMap::setRoom(int r) {
	room = r - 1;
}

void TileMap::setMoney(int money) {
	this->money = money;
}

int TileMap::getMoney() {
	return money;
}

int TileMap::getMoneyTiles() {
	return moneyTiles;
}

void TileMap::swapPoints() {
	money += 10;
	points -= 10;
}

void TileMap::setPoints(int points) {
	this->points = points;
}

int TileMap::getPoints() {
	return points;
}

void TileMap::deadInside() {
	lives--;
}

void TileMap::setLives(int lives) {
	this->lives = lives;
}

int TileMap::getLives() {
	return lives;
}

int TileMap::getCurrentRoom() {
	return room + 1;
}

void TileMap::setAlarm(bool a) {
	if (a && !alarm) {
		alarmRoom = room;
		sound.playAlarm();
	}
	alarm = a;
	
}

bool TileMap::getAlarm() {
	return alarm;
}

int TileMap::getAlarmRoom() {
	return alarmRoom +1;
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

