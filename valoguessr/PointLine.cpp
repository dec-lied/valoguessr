#include "PointLine.h"

PointLine::PointLine(glm::vec2 point1, glm::vec2 point2, glm::vec4 color, float margin)
	: UIElement
	(
		(point1[0] + point2[0]) / 2.0f,
		(point1[1] + point2[1]) / 2.0f,
		0.00375f * margin,
		::fabs(point1[1] - point2[1])
	)
	, point1(point1)
	, point2(point2)
	, color(color)
	, margin(margin) 
{
	float height1s = 0.00375f * margin, rat = *UIElement::WINDOWHEIGHT / (float)*UIElement::WINDOWWIDTH, width1s = height1s * rat;
	GLfloat vertices[16]
	{
		(point1[0] - width1s) * *UIElement::WINDOWWIDTH, (point1[1] + height1s) * *UIElement::WINDOWHEIGHT, // tl // 0
		(point1[0] + width1s) * *UIElement::WINDOWWIDTH, (point1[1] + height1s) * *UIElement::WINDOWHEIGHT, // tr
		(point1[0] - width1s) * *UIElement::WINDOWWIDTH, (point1[1] - height1s) * *UIElement::WINDOWHEIGHT, // bl
		(point1[0] + width1s) * *UIElement::WINDOWWIDTH, (point1[1] - height1s) * *UIElement::WINDOWHEIGHT, // br

		(point2[0] - width1s) * *UIElement::WINDOWWIDTH, (point2[1] + height1s) * *UIElement::WINDOWHEIGHT, // tl // 4
		(point2[0] + width1s) * *UIElement::WINDOWWIDTH, (point2[1] + height1s) * *UIElement::WINDOWHEIGHT, // tr
		(point2[0] - width1s) * *UIElement::WINDOWWIDTH, (point2[1] - height1s) * *UIElement::WINDOWHEIGHT, // bl
		(point2[0] + width1s) * *UIElement::WINDOWWIDTH, (point2[1] - height1s) * *UIElement::WINDOWHEIGHT  // br
	};

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	if (point1[0] <= point2[0]) // point 1 is to the left
	{
		if (point1[1] <= point2[1]) // point 1 is under
		{
			GLuint indices[]
			{
				0, 5, 4, // 1tl, 2tr, 2tl
				0, 1, 5, // 1tl, 1tr, 2tr

				1, 7, 5, // 1tr, 2br, 2tr
				1, 3, 7, // 1tr, 1br, 2br

				0, 3, 1, // p1 square
				0, 2, 3, // p1 square

				4, 7, 5, // p2 square
				4, 6, 7 // p2 square
			};

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
		}
		else // point 1 is above
		{
			GLuint indices[]
			{
				0, 4, 5, // 1tl, 2tr, 2tl
				0, 5, 1, // 1tl, 1tr, 2tr

				0, 6, 4, // 1tl, 2tl, 2bl
				0, 2, 6, // 1tl, 1bl, 2bl

				0, 3, 1, // p1 square
				0, 2, 3, // p1 square

				4, 7, 5, // p2 square
				4, 6, 7 // p2 square
			};

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
		}
	}
	else // point 1 is to the right
	{
		if (point1[1] <= point2[1]) // point 1 is under
		{
			GLuint indices[]
			{
				4, 0, 1, // 1tl, 2tr, 2tl
				4, 1, 5, // 1tl, 1tr, 2tr

				4, 2, 0, // 1tl, 2tl, 2bl
				4, 6, 2, // 1tl, 1bl, 2bl

				4, 7, 5, // p1 square
				4, 6, 7, // p1 square

				0, 4, 1, // p2 square
				0, 2, 3 // p2 square
			};

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
		}
		else // point 1 is above
		{
			GLuint indices[]
			{
				4, 1, 0, // 1tl, 2tr, 2tl
				4, 5, 1, // 1tl, 1tr, 2tr

				5, 3, 1, // 1tr, 2br, 2tr
				5, 7, 3, // 1tr, 1br, 2br

				4, 7, 5, // p1 square
				4, 6, 7, // p1 square

				0, 3, 1, // p2 square
				0, 2, 3 // p2 square
			};

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
		}
	}

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0x00);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

PointLine::~PointLine()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);

#ifdef _DEBUG
	std::cout << "~PointLine" << std::endl;
#endif
}

void PointLine::update()
{
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	float height1s = 0.00375f * margin, rat = *UIElement::WINDOWHEIGHT / (float)*UIElement::WINDOWWIDTH, width1s = height1s * rat;
	GLfloat vertices[16]
	{
		(point1[0] - width1s) * *UIElement::WINDOWWIDTH, (point1[1] + height1s) * *UIElement::WINDOWHEIGHT, // tl // 0
		(point1[0] + width1s) * *UIElement::WINDOWWIDTH, (point1[1] + height1s) * *UIElement::WINDOWHEIGHT, // tr
		(point1[0] - width1s) * *UIElement::WINDOWWIDTH, (point1[1] - height1s) * *UIElement::WINDOWHEIGHT, // bl
		(point1[0] + width1s) * *UIElement::WINDOWWIDTH, (point1[1] - height1s) * *UIElement::WINDOWHEIGHT, // br

		(point2[0] - width1s) * *UIElement::WINDOWWIDTH, (point2[1] + height1s) * *UIElement::WINDOWHEIGHT, // tl // 4
		(point2[0] + width1s) * *UIElement::WINDOWWIDTH, (point2[1] + height1s) * *UIElement::WINDOWHEIGHT, // tr
		(point2[0] - width1s) * *UIElement::WINDOWWIDTH, (point2[1] - height1s) * *UIElement::WINDOWHEIGHT, // bl
		(point2[0] + width1s) * *UIElement::WINDOWWIDTH, (point2[1] - height1s) * *UIElement::WINDOWHEIGHT  // br
	};

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
}

void PointLine::render()
{
	if (!this->visible)
		return;

	Button::buttonShader->use();
	Button::buttonShader->setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(UIElement::projection));
	Button::buttonShader->set4fv("color", 1, glm::value_ptr(this->color));

	glBindVertexArray(this->VAO);

	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
}