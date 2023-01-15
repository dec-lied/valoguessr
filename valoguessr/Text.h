#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>

#include "Shader.h"

#include "UIElement.h"

struct FTChar
{
	GLuint TextureID;
	glm::ivec2   Size;
	glm::ivec2   Bearing;
	unsigned int Advance;
};

class Text : public UIElement
{
private:

public:
	// static members
	// freetype members
	static std::map<GLchar, FTChar> FTChars;
	static unsigned pixelHeight;
	static FT_Library ftlib;
	static FT_Face face;

	// rendering-related members
	static std::unique_ptr<Shader> textShader;

	// internal
	static GLuint VAO, VBO;

	// scaling-related members
	static float* ratioW, *ratioH;

	// static methods
	static void init(const char* fontName, float* ratioW, float* ratioH, unsigned pixelHeight);
	static void cleanup();

	// universal members
	std::string text;
	glm::vec4 textColor;
	float realX, realY, scaleX, scaleY;
	float origRealX, origRealY;
	bool lock = false;

	// universal members
	// changes internal text variable. DOES NOT UPDATE FOR VERTSALILITY
	void changeText(std::string text);

	Text(std::string text, float centerX, float centerY, float scaleX, float scaleY, glm::vec4 textColor, bool lock = false);
	virtual ~Text();

	// inherited
	virtual void update();
	virtual void render();
};

