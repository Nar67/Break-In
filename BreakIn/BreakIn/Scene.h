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

enum class SceneState {GAME, GAMEOVER, WIN, NEXTBANK};

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
	void initTextQuads();
	void initSprites();
	void loadLevel();
	void loadPlayer();
	void loadBall();
	void renderGame();
	void renderCalculator();
	void renderGameOver();
	void renderWin();

private:
	TileMap* map;
	Player* player;
	Ball* ball;

	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	TexturedQuad* texQuad[9];
	Texture texs[9];
	Text text;
	int currentLevel = 1, currentRoom = 1, currentLives = 4, money = 0, points = 0;
	bool swapedPoints = false;
	const string levels = "levels/level0";
	int window_width, window_height;
	SceneState state;
	SoundManager sound;
};


#endif //

