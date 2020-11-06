#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Scene.h"
#include "SpriteManager.h"
#include "Game.h"

#define SCREEN_X 34
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 6.5
#define INIT_PLAYER_Y_TILES 25

#define INIT_BALL_X_TILES 8
#define INIT_BALL_Y_TILES 25

#define MAP_OFFSET_Y 894

Scene::Scene()
{
	map = NULL;
	player = NULL;
	ball = NULL;
}

Scene::~Scene()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (ball != NULL)
		delete ball;
	for (int i = 0; i < 2; i++)
		if (texQuad[i] != NULL)
			delete texQuad[i];
}


void Scene::init()
{
	initShaders();
	initText();
	initSprites();
	loadLevel();
	loadPlayer();
	loadBall();

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	projection = glm::translate(projection, glm::vec3(0.f, -894.f, 0.f));
	currentTime = 0.0f;

	//Select font
	if (!text.init("fonts/ArcadepixPlus.ttf"))
		cout << "Could not load font!!!" << endl;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	ball->update(deltaTime);
}

void Scene::render()
{
	glm::mat4 modelview = glm::mat4(1.0f);
	
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();
	ball->render();

	// Money
	modelview = glm::translate(modelview, glm::vec3(550.f, 909.f, 0.f));
	modelview = glm::scale(modelview, glm::vec3(1.f, 0.25f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[0]->render(texs[0]);

	// Points
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(550.f, 989.f, 0.f));
	modelview = glm::scale(modelview, glm::vec3(1.f, 0.3f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[1]->render(texs[1]);

	// Lives
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(550.f, 1069.f, 0.f));
	modelview = glm::scale(modelview, glm::vec3(1.f, 0.2f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[2]->render(texs[2]);

	// Bank
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(550.f, 1149.f, 0.f));
	modelview = glm::scale(modelview, glm::vec3(1.f, 0.2f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[3]->render(texs[3]);

	// Batmode
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(550.f, 1229.f, 0.f));
	modelview = glm::scale(modelview, glm::vec3(1.f, 0.3f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[4]->render(texs[4]);

	// Room
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(550.f, 1294.f, 0.f));
	modelview = glm::scale(modelview, glm::vec3(1.f, 0.2f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[5]->render(texs[5]);

	renderText();
}

void Scene::restart() {
	currentRoom = 1;
	currentLevel = 1;
	currentLives = 4;
	loadLevel();
	loadPlayer();

}

void Scene::nextRoom() {
	if (currentRoom < 3) {
		ball->nextRoom();
		map->nextRoom();
		currentRoom++;
	}
		
}

void Scene::nextBank() {
	if (currentLevel < 3) {
		currentLevel++;
		currentRoom = 1;
		loadLevel();
	}
}

void Scene::loadLevel() {
	if (map != NULL)
		map->free();
	string file = levels + to_string(currentLevel) + ".txt";
	map = TileMap::createTileMap(file, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map->setRoom(currentRoom);
	if (ball != NULL) {
		ball->setTileMap(map);
		ball->firstRoom();
	}
}

void Scene::loadPlayer() {
	if (player != NULL)
		player = NULL;
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * 28, INIT_PLAYER_Y_TILES * 28 / 2 + MAP_OFFSET_Y));
	player->setTileMap(map);
}

void Scene::loadBall()
{
	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	ball->setPosition(glm::vec2(INIT_BALL_X_TILES * 28, INIT_BALL_Y_TILES * 28/2 + MAP_OFFSET_Y));
	ball->setTileMap(map);
	ball->setPlayer(player);
}

void Scene::initText() {
	glm::vec2 geom[2] = { glm::vec2 (0.f, 0.f), glm::vec2(128.f, 128.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texQuad[1] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texQuad[2] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texQuad[3] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texQuad[4] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texQuad[5] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	// Load textures
	texs[0].loadFromFile("images/money.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[1].loadFromFile("images/points.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[2].loadFromFile("images/lives.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[3].loadFromFile("images/bank.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[4].loadFromFile("images/batmode.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[5].loadFromFile("images/room.png", TEXTURE_PIXEL_FORMAT_RGBA);
}

void Scene::initSprites() {
	SpriteManager::createSprites();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::renderText() {
	window_width = glutGet(GLUT_WINDOW_WIDTH);
	window_height = glutGet(GLUT_WINDOW_HEIGHT);

	money = map->getMoney();
	string zeros;

	if (money >= 100000)
		zeros = "0";
	else if (money >= 10000)
		zeros = "00";
	else if (money >= 1000)
		zeros = "000";
	else if (money >= 100)
		zeros = "0000";
	else
		zeros = "000000";

	// Money
	text.render(zeros + to_string(money), glm::vec2(window_width - 25 * 7 * window_width / 640, 60 * window_height / 480),
		25 * window_width / 640, glm::vec4(1, 1, 1, 1));

	points = map->getPoints();

	if (points >= 100000)
		zeros = "0";
	else if (points >= 10000)
		zeros = "00";
	else if (points >= 1000)
		zeros = "000";
	else if (points >= 100)
		zeros = "0000";
	else
		zeros = "000000";

	// Points
	text.render(zeros + to_string(points), glm::vec2(window_width - 25 * 7 * window_width / 640, 145 * window_height / 480),
		25 * window_width / 640, glm::vec4(1, 1, 1, 1));
	// Lives
	text.render("0" + to_string(currentLives), glm::vec2(window_width - 25 * 3 * window_width / 640,
		220 * window_height / 480), 25 * window_width / 640, glm::vec4(1, 1, 1, 1));
	// Bank
	text.render("0" + to_string(currentLevel), glm::vec2(window_width - 25 * 3 * window_width / 640,
		300 * window_height / 480), 25 * window_width / 640, glm::vec4(1, 1, 1, 1));
	// Room
	text.render("0" + to_string(currentRoom), glm::vec2(window_width - 25 * 3 * window_width / 640,
		445 * window_height / 480), 25 * window_width / 640, glm::vec4(1, 1, 1, 1));
}


