#pragma once

#include <string>
#include <stb/stb_image.h>

#include "Shader.h"
#include "Texture.h"

#include "UIElement.h"

class Image : public UIElement
{
protected:
	// internal
	GLuint VAO, VBO;
	Texture texture;

public:
	// static members & methods
	static std::unique_ptr<Shader> imageShader;
	static void init();
	static void cleanup();

	// universal members
	std::string imgPath;
	float opacity;

	Image(std::string imgPath, bool alpha, float centerX, float centerY, float width, float height, float opacity = 1.0f);
	~Image();

	// universal methods
	// given image file path and alpha, changes texture
	void changeTexture(std::string imgPath, bool alpha);

	// inherited
	virtual void update();
	virtual void render();
};

