#pragma once

#include "Button.h"

class PointLine : public UIElement
{
protected:
	GLuint VAO, VBO, EBO;

public:
	glm::vec2 point1, point2;
	glm::vec4 color;
	float margin;

	PointLine(glm::vec2 point1, glm::vec2 point2, glm::vec4 color, float margin);
	~PointLine();

	void update();
	void render();
};

