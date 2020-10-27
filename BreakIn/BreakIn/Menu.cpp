#include "Menu.h"
#include "Game.h"
#include <GL/glut.h>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Menu::Menu() {

}

Menu::~Menu() {

}

void Menu::init() {
	initShaders();
	choice = 0;

	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(CAMERA_WIDTH, CAMERA_HEIGHT) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	breakinImage.loadFromFile("images/breakin.png", TEXTURE_PIXEL_FORMAT_RGBA);
	breakinQuad = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	if (!text.init("fonts/ArcadepixPlus.ttf"))
		cout << "Could not load font!!!" << endl;
}

void Menu::update(int deltaTime) {
	currentTime += deltaTime;

	if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		Game::instance().specialKeyReleased(GLUT_KEY_DOWN);
		++choice;
		choice %= 3;
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		Game::instance().specialKeyReleased(GLUT_KEY_UP);
		choice += 2;
		choice %= 3;
	}

	if (Game::instance().getKey(32)) {
		Game::instance().changeMode(choice);
	}
}

void Menu::render() {
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	breakinQuad->render(breakinImage);

	if (choice == 0) {
		text.render("PLAY", glm::vec2(260, 350), 30, glm::vec4(0.5, 0.4, 1, 1));
		text.render("INSTRUCTIONS", glm::vec2(182, 400), 24, glm::vec4(1, 1, 1, 1));
		text.render("CREDITS", glm::vec2(232, 450), 24, glm::vec4(1, 1, 1, 1));
	}
	else if (choice == 1) {
		text.render("PLAY", glm::vec2(270, 350), 24, glm::vec4(1, 1, 1, 1));
		text.render("INSTRUCTIONS", glm::vec2(150, 400), 30, glm::vec4(0.5, 0.4, 1, 1));
		text.render("CREDITS", glm::vec2(232, 450), 24, glm::vec4(1, 1, 1, 1));
	}
	else {
		text.render("PLAY", glm::vec2(270, 350), 24, glm::vec4(1, 1, 1, 1));
		text.render("INSTRUCTIONS", glm::vec2(182, 400), 24, glm::vec4(1, 1, 1, 1));
		text.render("CREDITS", glm::vec2(216, 450), 30, glm::vec4(0.5, 0.4, 1, 1));
	}

}

void Menu::initShaders() {
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
