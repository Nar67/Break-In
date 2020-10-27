#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include "ShaderProgram.h"
#include "Text.h"
#include "Texture.h"
#include "TexturedQuad.h"
#include <glm/glm.hpp>

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480

class Menu
{

public:
	Menu();
	~Menu();

	void init();
	void update(int deltaTime);
	void render();

private: 
	void initShaders();
private:
	ShaderProgram texProgram;
	glm::mat4 projection;
	Text text;
	Texture breakinImage;
	TexturedQuad* breakinQuad;

	float currentTime;
	int choice;

};

#endif