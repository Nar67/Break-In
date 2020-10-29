#ifndef	_SHORT_CUTS_INCLUDE
#define _SHORT_CUTS_INCLUDE

#include "ShaderProgram.h"
#include "Texture.h"
#include "TexturedQuad.h"

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480

class ShortCuts
{
public:
	ShortCuts();
	~ShortCuts();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	ShaderProgram texProgram;
	glm::mat4 projection;
	Texture shortcutsImage;
	TexturedQuad* shortcutsQuad;

	float currentTime;
};

#endif

