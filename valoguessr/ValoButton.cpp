#include "ValoButton.h"
#include <array>

bool ValoButton::mouseInTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float mouseX, float mouseY)
{
	// sourced by https://www.geeksforgeeks.org/check-whether-a-given-point-lies-inside-a-triangle-or-not/
	auto area = [](float& x1, float& y1, float& x2, float& y2, float& x3, float& y3) -> float
	{
		return ::fabs(((x1 * (y2 - y3)) + (x2 * (y3 - y1)) + (x3 * (y1 - y2))) / 2.0f);
	};

	float valoTriangleA = area(x1, y1, x2, y2, x3, y3);
	float mouseTriangleA1 = area(x1, y1, x2, y2, mouseX, mouseY);
	float mouseTriangleA2 = area(x2, y2, x3, y3, mouseX, mouseY);
	float mouseTriangleA3 = area(x3, y3, x1, y1, mouseX, mouseY);

	// epsilon float equality check where epsilon = .0005f
	if (mouseTriangleA1 + mouseTriangleA2 + mouseTriangleA3 > valoTriangleA - 0.0005f && mouseTriangleA1 + mouseTriangleA2 + mouseTriangleA3 < valoTriangleA + 0.00005f)
		return true;
	else
		return false;
}


ValoButton::ValoButton(Text* text, float lMargin, float rMargin, float uMargin, float dMargin, float sidesWidth, glm::vec4 bgColor, glm::vec4 hoverColor, void(*onHover)(), void(*onClick)())
	: Button(text, lMargin, rMargin, uMargin, dMargin, bgColor, hoverColor, onHover, onClick)
	, sidesWidth(sidesWidth)
	, lTri
	({
		(((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) - (((this->width / 2.0f) + ((this->width / 2.0f) * this->lMargin)) * this->sidesWidth)) , // ll
		((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)), // t

		((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)), // l
		((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)), // b

		((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)), // l
		((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin))
	})
	, rTri
	({
		((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)), // r
		((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)), // t

		((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)), // r
		((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)), // b

		(((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) + (((this->width / 2.0f) + ((this->width / 2.0f) * this->rMargin)) * this->sidesWidth)), // rr
		((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin))  // t
	})
{
	GLfloat vertices[24]
	{
		((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) * *UIElement::WINDOWWIDTH, // l
			((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) * *UIElement::WINDOWHEIGHT, // t

		((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) * *UIElement::WINDOWWIDTH, // l
			((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)) * *UIElement::WINDOWHEIGHT, // b

		((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) * *UIElement::WINDOWWIDTH, // r
			((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) * *UIElement::WINDOWHEIGHT, // t


		((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) * *UIElement::WINDOWWIDTH, // l
			((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)) * *UIElement::WINDOWHEIGHT, // b

		((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) * *UIElement::WINDOWWIDTH, // r
			((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)) * *UIElement::WINDOWHEIGHT, // b

		((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) * *UIElement::WINDOWWIDTH, // r
			((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) * *UIElement::WINDOWHEIGHT, // t

		
		lTri.x1 * *UIElement::WINDOWWIDTH, lTri.y1 * *UIElement::WINDOWHEIGHT,
		lTri.x2 * *UIElement::WINDOWWIDTH, lTri.y2 * *UIElement::WINDOWHEIGHT,
		lTri.x3 * *UIElement::WINDOWWIDTH, lTri.y3 * *UIElement::WINDOWHEIGHT,

		rTri.x1 * *UIElement::WINDOWWIDTH, rTri.y1 * *UIElement::WINDOWHEIGHT,
		rTri.x2 * *UIElement::WINDOWWIDTH, rTri.y2 * *UIElement::WINDOWHEIGHT,
		rTri.x3 * *UIElement::WINDOWWIDTH, rTri.y3 * *UIElement::WINDOWHEIGHT,
	};

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0x00);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool ValoButton::isHovering(float mouseX, float mouseY)
{
	if (!this->visible)
		return false;

	mouseY = *UIElement::WINDOWHEIGHT - mouseY;

	if ((mouseX > ((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) * *UIElement::WINDOWWIDTH &&
		mouseX < ((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) * *UIElement::WINDOWWIDTH &&
		mouseY < ((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) * *UIElement::WINDOWHEIGHT &&
		mouseY > ((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)) * *UIElement::WINDOWHEIGHT)
		|| // left triangle
		ValoButton::mouseInTriangle(lTri.x1, lTri.y1, lTri.x2, lTri.y2, lTri.x3, lTri.y3, mouseX / *UIElement::WINDOWWIDTH, mouseY / *UIElement::WINDOWHEIGHT)
		|| // right triangle
		ValoButton::mouseInTriangle(rTri.x1, rTri.y1, rTri.x2, rTri.y2, rTri.x3, rTri.y3, mouseX / *UIElement::WINDOWWIDTH, mouseY / *UIElement::WINDOWHEIGHT)
		)
	{
		if (!this->hovering)
			this->onHover();

		this->hovering = true;
		return true;
	}
	else
	{
		this->hovering = false;
		return false;
	}
}

void ValoButton::update()
{
	this->text->update();

	this->width = text->width / *UIElement::WINDOWWIDTH;
	this->height = text->height / *UIElement::WINDOWHEIGHT;

	GLfloat vertices[24]
	{
		((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) * *UIElement::WINDOWWIDTH, // l
			((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) * *UIElement::WINDOWHEIGHT, // t

		((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) * *UIElement::WINDOWWIDTH, // l
			((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)) * *UIElement::WINDOWHEIGHT, // b

		((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) * *UIElement::WINDOWWIDTH, // r
			((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) * *UIElement::WINDOWHEIGHT, // t


		((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) * *UIElement::WINDOWWIDTH, // l
			((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)) * *UIElement::WINDOWHEIGHT, // b

		((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) * *UIElement::WINDOWWIDTH, // r
			((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)) * *UIElement::WINDOWHEIGHT, // b

		((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) * *UIElement::WINDOWWIDTH, // r
			((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) * *UIElement::WINDOWHEIGHT, // t


		lTri.x1 * *UIElement::WINDOWWIDTH, lTri.y1 * *UIElement::WINDOWHEIGHT,
		lTri.x2 * *UIElement::WINDOWWIDTH, lTri.y2 * *UIElement::WINDOWHEIGHT,
		lTri.x3 * *UIElement::WINDOWWIDTH, lTri.y3 * *UIElement::WINDOWHEIGHT,

		rTri.x1 * *UIElement::WINDOWWIDTH, rTri.y1 * *UIElement::WINDOWHEIGHT,
		rTri.x2 * *UIElement::WINDOWWIDTH, rTri.y2 * *UIElement::WINDOWHEIGHT,
		rTri.x3 * *UIElement::WINDOWWIDTH, rTri.y3 * *UIElement::WINDOWHEIGHT,
	};

	//GLfloat vertices[24]
	//{
	//	((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) * *UIElement::WINDOWWIDTH, // l
	//		((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) * *UIElement::WINDOWHEIGHT, // t

	//	((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) * *UIElement::WINDOWWIDTH, // l
	//		((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)) * *UIElement::WINDOWHEIGHT, // b

	//	((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) * *UIElement::WINDOWWIDTH, // r
	//		((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) * *UIElement::WINDOWHEIGHT, // t


	//	((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) * *UIElement::WINDOWWIDTH, // l
	//		((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)) * *UIElement::WINDOWHEIGHT, // b

	//	((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) * *UIElement::WINDOWWIDTH, // r
	//		((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)) * *UIElement::WINDOWHEIGHT, // b

	//	((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) * *UIElement::WINDOWWIDTH, // r
	//		((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) * *UIElement::WINDOWHEIGHT, // t


	//	(((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) - (((this->width / 2.0f) + ((this->width / 2.0f) * this->lMargin)) * this->sidesWidth)) * *UIElement::WINDOWWIDTH, // ll
	//		((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) * *UIElement::WINDOWHEIGHT, // t

	//	((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) * *UIElement::WINDOWWIDTH, // l
	//		((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)) * *UIElement::WINDOWHEIGHT, // b

	//	((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) * *UIElement::WINDOWWIDTH, // l
	//		((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) * *UIElement::WINDOWHEIGHT, // t


	//	((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) * *UIElement::WINDOWWIDTH, // r
	//		((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) * *UIElement::WINDOWHEIGHT, // t

	//	((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) * *UIElement::WINDOWWIDTH, // r
	//		((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)) * *UIElement::WINDOWHEIGHT, // b

	//	(((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) + (((this->width / 2.0f) + ((this->width / 2.0f) * this->rMargin)) * this->sidesWidth)) * *UIElement::WINDOWWIDTH, // rr
	//		((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) * *UIElement::WINDOWHEIGHT  // t
	//};

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
}

void ValoButton::render()
{
	if (!this->visible)
		return;

	Button::buttonShader->use();
	Button::buttonShader->setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(UIElement::projection));

	if (this->hovering)
		Button::buttonShader->set4fv("color", 1, glm::value_ptr(this->hoverColor));
	else
		Button::buttonShader->set4fv("color", 1, glm::value_ptr(this->bgColor));

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 12);

	this->text->render();
}