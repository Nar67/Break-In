#include <iostream>
#include <string>
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

#define INIT_VIGILANT_X_TILES 1
#define INIT_VIGILANT_Y_TILES 17

#define MAP_OFFSET_Y 894

Scene::Scene()
{
	map = NULL;
	player = NULL;
	ball = NULL;
	vigilant = NULL;
}

Scene::~Scene()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (ball != NULL)
		delete ball;
	if (vigilant != NULL)
		delete vigilant;
	for (int i = 0; i < 7; i++)
		if (texQuad[i] != NULL)
			delete texQuad[i];
}


void Scene::init()
{
	initShaders();
	initTextQuads();
	initSprites();
	loadLevel();
	loadPlayer();
	loadBall();
	loadVigilant();
	state = SceneState::GAME;

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
	if (vigilant != NULL)
		vigilant->update(deltaTime);

	money = map->getMoney();
	points = map->getPoints();
	currentLives = map->getLives();
	currentRoom = map->getCurrentRoom();
	alarm = map->getAlarm();
	
	if (state == SceneState::GAME) {

		if (map->getMoneyTiles() == 0 and swapedPoints) {
			if (currentLevel < 3) {
				currentLevel++;
				currentRoom = 1;
				loadLevel();
				loadPlayer();
				loadBall();
				loadVigilant();
			}
			else {
				state = SceneState::WIN;
				ball->setStop(true);
				if (vigilant != NULL)
					vigilant->setStop(true);
				if (sound.isCurrentlyPlaying("sound/gameover.ogg"))
					sound.stopSound("sound/gameover.ogg");
				if (sound.isCurrentlyPlaying("sound/alarm.ogg"))
					sound.stopSound("sound/alarm.ogg");
				if (sound.isCurrentlyPlaying("sound/background.ogg"))
					sound.stopBackground();
				sound.playWin();
			}

		}
		if (alarm) {
			if (currentRoom == map->getAlarmRoom()) {
				vigilant->setStop(false);
				renderVigilant = true;
			}
			else {
				renderVigilant = false;
				vigilant->setStop(true);
			}

		}
		if (currentLives == 0) {
			state = SceneState::GAMEOVER;
			ball->setStop(true);
			if (vigilant != NULL)
				vigilant->setStop(true);
			if (sound.isCurrentlyPlaying("sound/gameover.ogg"))
				sound.stopSound("sound/gameover.ogg");
			if (sound.isCurrentlyPlaying("sound/alarm.ogg"))
				sound.stopSound("sound/alarm.ogg");
			if (sound.isCurrentlyPlaying("sound/background.ogg"))
				sound.stopBackground();
			sound.playDeath();
		}
	}

}

void Scene::render()
{

	if (state == SceneState::GAME) {
		renderGame();
		if (map->getCalculator())
			renderCalculator();
	}
	else if (state == SceneState::GAMEOVER) {
		renderGameOver();
	}
	else if (state == SceneState::WIN) {
		renderWin();
	}
		
}

void Scene::restart() {
	currentRoom = 1;
	currentLevel = 1;
	currentLives = 4;
	loadLevel();
	loadPlayer();
	loadBall();
}

void Scene::nextRoom() {
	if (currentRoom < 3) {
		ball->nextRoom();
		map->openPath();
		map->nextRoom();
	}
		
}

void Scene::nextBank() {
	if (currentLevel < 3) {
		currentLevel++;
		currentRoom = 1;
		loadLevel();
		map->setAlarm(false);
		renderVigilant = false;
		vigilant->setStop(true);
		if (sound.isCurrentlyPlaying("sound/alarm.ogg"))
			sound.stopSound("sound/alarm.ogg");
		loadBall();
		player->setTileMap(map);
		player->setInitPos();
		loadVigilant();
	}
}

void Scene::loadLevel() {
	if (map != NULL)
		map->free();
	string file = levels + to_string(currentLevel) + ".txt";
	map = TileMap::createTileMap(file, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map->setRoom(currentRoom);
	map->setMoney(money);
	map->setPoints(points);
	map->setLives(currentLives);
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
	if (ball != NULL)
		ball = NULL;
	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	ball->setPosition(glm::vec2(INIT_BALL_X_TILES * 28, INIT_BALL_Y_TILES * 28/2 + MAP_OFFSET_Y));
	ball->setTileMap(map);
	ball->setPlayer(player);
}

void Scene::loadVigilant()
{
	vigilant = new Vigilant();
	vigilant->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	vigilant->setPosition(glm::vec2(INIT_VIGILANT_X_TILES * 28, INIT_VIGILANT_Y_TILES * 28/2 + MAP_OFFSET_Y));
	vigilant->setTileMap(map);
	vigilant->setPlayer(player);
}

void Scene::initTextQuads() {
	glm::vec2 geom[2] = { glm::vec2 (0.f, 0.f), glm::vec2(128.f, 128.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texQuad[1] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texQuad[2] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texQuad[3] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texQuad[4] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texQuad[5] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texQuad[6] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texQuad[7] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	texQuad[8] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	// Load textures
	texs[0].loadFromFile("images/money.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[1].loadFromFile("images/points.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[2].loadFromFile("images/lives.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[3].loadFromFile("images/bank.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[4].loadFromFile("images/batmode.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[5].loadFromFile("images/room.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[6].loadFromFile("images/calculator.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[7].loadFromFile("images/gameOver.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[8].loadFromFile("images/win.png", TEXTURE_PIXEL_FORMAT_RGBA);
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

void Scene::renderGame() {
	glm::mat4 modelview = glm::mat4(1.0f);

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	map->render();
	player->render();
	ball->render();
	if (renderVigilant)
		vigilant->render();

	// Money
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(550.f, 909.f, 0.f));
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


	window_width = glutGet(GLUT_WINDOW_WIDTH);
	window_height = glutGet(GLUT_WINDOW_HEIGHT);

	
	string zeros;

	if (money < 10)
		zeros = "000000";
	else if (money < 100)
		zeros = "00000";
	else if (money < 1000)
		zeros = "0000";
	else if (money < 10000)
		zeros = "000";
	else if (money < 100000)
		zeros = "00";
	else if (money < 1000000)
		zeros = "0";
	else
		zeros = "";

	// Money
	text.render(zeros + to_string(money), glm::vec2(window_width - 25 * 7 * window_width / 640, 60 * window_height / 480),
		25 * window_width / 640, glm::vec4(1, 1, 1, 1));

	
	if (points < 10)
		zeros = "000000";
	else if (points < 100)
		zeros = "00000";
	else if (points < 1000)
		zeros = "0000";
	else if (points < 10000)
		zeros = "000";
	else if (points < 100000)
		zeros = "00";
	else if (points < 1000000)
		zeros = "0";
	else
		zeros = "";

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

void Scene::renderCalculator() {
	glm::mat4 modelview = glm::mat4(1.0f);
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(224.f, 1134.f, 0.f));
	modelview = glm::scale(modelview, glm::vec3(2.625f, 1.6f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[6]->render(texs[6]);

	window_width = glutGet(GLUT_WINDOW_WIDTH);
	window_height = glutGet(GLUT_WINDOW_HEIGHT);

	string zeros;

	if (points < 10)
		zeros = "000000";
	else if (points < 100)
		zeros = "00000";
	else if (points < 1000)
		zeros = "0000";
	else if (points < 10000)
		zeros = "000";
	else if (points < 100000)
		zeros = "00";
	else if (points < 1000000)
		zeros = "0";
	else 
		zeros = "";

	text.render(zeros + to_string(points), glm::vec2(window_width - 480 * window_width / 640, 262 * window_height / 480),
		20 * window_width / 640, glm::vec4(1, 1, 1, 1));

	if (money < 10)
		zeros = "000000";
	else if (money < 100)
		zeros = "00000";
	else if (money < 1000)
		zeros = "0000";
	else if (money < 10000)
		zeros = "000";
	else if (money < 100000)
		zeros = "00";
	else if (money < 1000000)
		zeros = "0";
	else
		zeros = "";

	text.render(zeros + to_string(money), glm::vec2(window_width - 480 * window_width / 640, 315 * window_height / 480),
		20 * window_width / 640, glm::vec4(1, 1, 1, 1));

	if (points > 0)
		map->swapPoints();
	else {
		swapedPoints = true;
		map->setCalculator();
		ball->setStop(false);
		sound.stopSound("sound/calculator.ogg");
	}

}

void Scene::renderWin() {
	glm::mat4 modelview = glm::mat4(1.0f);
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(320.f, 1130.f, 0.f));
	modelview = glm::scale(modelview, glm::vec3(5.f, 4.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[8]->render(texs[8]);
}

void Scene::renderGameOver() {
	glm::mat4 modelview = glm::mat4(1.0f);
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(320.f, 1130.f, 0.f));
	modelview = glm::scale(modelview, glm::vec3(4.f, 3.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[7]->render(texs[7]);

}
