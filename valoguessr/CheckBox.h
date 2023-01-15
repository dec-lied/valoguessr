#pragma once

#include "Image.h"

class CheckBox : public UIElement
{
protected:
	GLuint VAO, VBO;
	static GLuint checkedTextureID, uncheckedTextureID;
	static int dataW, dataH, dataNR;

	// making static to reduce calls to stbi_load (reducing lag)
	static unsigned char* checkedData;
	static unsigned char* uncheckedData;

public:
	// static members
	static void init();
	static void cleanup();

	// universal members
	bool checked;

	CheckBox(float x, float y, float width, float height);
	~CheckBox();

	// universal methods
	void flip();
	void setStatus(bool status);

	// inherited
	void update();
	void render();
};