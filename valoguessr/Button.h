#pragma once

#include <irrKlang/irrKlang.h>

#include "Shader.h"

#include "Text.h"
#include "Image.h"

enum ButtonType
{
	BLANK, BLANK_BORDER, TEXT, TEXT_BORDER, IMAGE
};

class Button : public UIElement
{
private:
	// internal, do not inherit
	ButtonType buttonType;

	// border button members
	float borderWidth;
	glm::vec4 borderColor;

protected:
	// internal
	GLuint VAO, VBO;

public:
	// static members & methods
	static std::unique_ptr<Shader> buttonShader;
	static void init();
	static void cleanup();

	// text and image based button members
	float lMargin, rMargin, uMargin, dMargin;

	// text based button members
	Text* text = nullptr;

	// image based button members
	Image* image = nullptr;

	// universal members
	void(*onClick)();
	void(*onHover)();
	void(*offHover)();
	bool changesScene;
	bool hovering = false;
	bool clickable = true;
	glm::vec4 bgColor, hoverColor;

	Button(float x, float y, float width, float height, glm::vec4 bgColor, glm::vec4 hoverColor, void (*onHover)(), void(*onClick)(), void(*offHover)() = []() {}, bool changesScene = true);
	Button(float x, float y, float width, float height, float borderWidth, glm::vec4 bgColor, glm::vec4 hoverColor, glm::vec4 borderColor, void (*onHover)(), void(*onClick)(), void(*offHover)() = []() {}, bool changesScene = true);
	Button(Text* text, float lMargin, float rMargin, float uMargin, float dMargin, glm::vec4 bgColor, glm::vec4 hoverColor, void (*onHover)(), void(*onClick)(), void(*offHover)() = []() {}, bool changesScene = true);
	Button(Text* text, float lMargin, float rMargin, float uMargin, float dMargin, float borderWidth, glm::vec4 bgColor, glm::vec4 hoverColor, glm::vec4 borderColor, void (*onHover)(), void(*onClick)(), void(*offHover)() = []() {}, bool changesScene = true);
	Button(Image* image, glm::vec4 bgColor, void (*onHover)(), void(*onClick)(), void(*offHover)() = []() {}, bool changesScene = true);
	virtual ~Button();

	// universal methods
	// based on mouse coordinates, returns whether mouse is hovering over this instance
	virtual bool isHovering(float mouseX, float mouseY);

	// inherited
	virtual void update();
	virtual void render();
};

