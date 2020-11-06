#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "TexturedQuad.h"
#include "Text.h"
#include "Menu.h"
#include <string>
#include "Ball.h"

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	void nextRoom();
	void nextBank();
	void restart();

private:
	void initShaders();
	void initText();
	void initSprites();
	void loadLevel();
	void loadPlayer();
	void loadBall();
	void renderText();

private:
	TileMap* map;
	Player* player;
	Ball* ball;
	//TileMap* key;
	//vector<TileMap*> layers;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	TexturedQuad* texQuad[6];
	Texture texs[6];
	Text text;
	int currentLevel = 1, currentRoom = 1, currentLives = 4, money, points;
	const string levels = "levels/level0";
	int window_width, window_height;
};


#endif //

