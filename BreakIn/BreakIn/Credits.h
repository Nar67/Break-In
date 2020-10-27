#ifndef _CREDITS_INCLUDE
#define _CREDITS_INCLUDE

#include "ShaderProgram.h"
#include "Texture.h"
#include "TexturedQuad.h"
#include <glm/glm.hpp>


#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480


class Credits
{
public:
	Credits();
	~Credits();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	ShaderProgram texProgram;
	glm::mat4 projection;
	Texture creditsImage;
	TexturedQuad* creditsQuad;

	float currentTime;
};

#endif // !_CREDITS_INCLUDE

