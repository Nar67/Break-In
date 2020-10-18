#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"

Scene::Scene()
{
	
	info.open("levels/info01.txt");
	int size;
	info >> size;
	layers = vector<TileMap*> (size);
	for (int i = 0; i < layers.size(); ++i) {
		layers[i] = NULL;
	}
	/*map = NULL;
	key = NULL;*/
}

Scene::~Scene()
{
	for (int i = 0; i < layers.size(); ++i) {
		if (layers[i] != NULL) {
			delete layers[i];
		}
	}
	/*if (map != NULL)
		delete map;
	if (key != NULL)
		delete key;*/
}


void Scene::init()
{
	initShaders();
	for (int i = 0; i < layers.size(); ++i) {
		string tilemap;
		info >> tilemap;
		layers[i] = TileMap::createTileMap(tilemap, glm::vec2(32, 16), texProgram);
	}
	info.close();
	//map = TileMap::createTileMap("levels/level01.txt", glm::vec2(32, 16), texProgram);
	//key = TileMap::createTileMap("levels/klvl01.txt", glm::vec2(32, 16), texProgram);
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	for (int i = 0; i < layers.size(); ++i) {
		layers[i]->render();
	}
	/*map->render();
	key->render();*/
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



