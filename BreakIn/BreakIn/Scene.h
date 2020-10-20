#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Direction.h"

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
	void move(Direction const& direction);

private:
	void initShaders();

private:
	//TileMap* map;
	//TileMap* key;
	vector<TileMap*> layers;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	ifstream info;

};


#endif // _SCENE_INCLUDE

