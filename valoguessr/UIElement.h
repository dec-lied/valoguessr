#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef _DEBUG
#include <iostream>
#endif

class UIElement
{
public:
	// static members
	static unsigned* WINDOWWIDTH, * WINDOWHEIGHT;
	static glm::mat4 projection;

	// universal members
	float centerX, centerY, width, height;
	bool visible = true;

	UIElement(float centerX, float centerY, float width, float height);
	virtual ~UIElement() {}

	// pure virtual functions
	virtual void render() = 0;
	virtual void update() = 0;
};

