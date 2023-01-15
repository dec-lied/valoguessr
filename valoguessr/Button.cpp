#include "Button.h"

std::unique_ptr<Shader> Button::buttonShader = nullptr;

Button::Button(float x, float y, float width, float height, glm::vec4 bgColor, glm::vec4 hoverColor, void (*onHover)(), void(*onClick)(), void(*offHover)(), bool changesScene)
	: UIElement(x, y, width, height)
	, bgColor(bgColor)
	, hoverColor(hoverColor)
	, onHover(onHover)
	, onClick(onClick)
	, offHover(offHover)
	, changesScene(changesScene)
	, borderColor(glm::vec4(0.0f))
	, borderWidth(0.0f)
	, lMargin(0.0f)
	, rMargin(0.0f)
	, uMargin(0.0f)
	, dMargin(0.0f)
{
	this->buttonType = ButtonType::BLANK;

	GLfloat vertices[12]
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
			((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) * *UIElement::WINDOWHEIGHT // t
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

Button::Button(float x, float y, float width, float height, float borderWidth, glm::vec4 bgColor, glm::vec4 hoverColor, glm::vec4 borderColor, void (*onHover)(), void(*onClick)(), void(*offHover)(), bool changesScene)
	: UIElement(x, y, width, height)
	, borderWidth(borderWidth)
	, bgColor(bgColor)
	, hoverColor(hoverColor)
	, borderColor(borderColor)
	, onHover(onHover)
	, onClick(onClick)
	, offHover(offHover)
	, changesScene(changesScene)
	, lMargin(0.0f)
	, rMargin(0.0f)
	, uMargin(0.0f)
	, dMargin(0.0f)
{
	this->buttonType = ButtonType::BLANK_BORDER;

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



		(((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) - ((this->borderWidth * (*UIElement::WINDOWHEIGHT / (float)*UIElement::WINDOWWIDTH)) * ((this->height / 2.0f)))) * *UIElement::WINDOWWIDTH, // l
			(((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) + (this->borderWidth * ((this->height / 2.0f)))) * *UIElement::WINDOWHEIGHT, // t		

		(((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) - ((this->borderWidth * (*UIElement::WINDOWHEIGHT / (float)*UIElement::WINDOWWIDTH)) * ((this->height / 2.0f)))) * *UIElement::WINDOWWIDTH, // l
			(((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)) - (this->borderWidth * ((this->height / 2.0f)))) * *UIElement::WINDOWHEIGHT, //  b  		

		(((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) + ((this->borderWidth * (*UIElement::WINDOWHEIGHT / (float)*UIElement::WINDOWWIDTH)) * ((this->height / 2.0f)))) * *UIElement::WINDOWWIDTH, // r
			(((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) + (this->borderWidth * ((this->height / 2.0f)))) * *UIElement::WINDOWHEIGHT, // t				


		(((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) - ((this->borderWidth * (*UIElement::WINDOWHEIGHT / (float)*UIElement::WINDOWWIDTH)) * ((this->height / 2.0f)))) * *UIElement::WINDOWWIDTH, // l
			(((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)) - (this->borderWidth * ((this->height / 2.0f)))) * *UIElement::WINDOWHEIGHT, //  b  		

		(((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) + ((this->borderWidth * (*UIElement::WINDOWHEIGHT / (float)*UIElement::WINDOWWIDTH)) * ((this->height / 2.0f)))) * *UIElement::WINDOWWIDTH, // r
			(((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)) - (this->borderWidth * ((this->height / 2.0f)))) * *UIElement::WINDOWHEIGHT, //  b  		

		(((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) + ((this->borderWidth * (*UIElement::WINDOWHEIGHT / (float)*UIElement::WINDOWWIDTH)) * ((this->height / 2.0f)))) * *UIElement::WINDOWWIDTH, // r
			(((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) + (this->borderWidth * ((this->height / 2.0f)))) * *UIElement::WINDOWHEIGHT // t		
	};

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0x00);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Button::Button(Text* text, float lMargin, float rMargin, float uMargin, float dMargin, glm::vec4 bgColor, glm::vec4 hoverColor, void (*onHover)(), void(*onClick)(), void(*offHover)(), bool changesScene)
	: UIElement
	(
		text->centerX,
		text->centerY,
		text->width / *UIElement::WINDOWWIDTH,
		text->height / *UIElement::WINDOWHEIGHT
	)
	, text(text)
	, lMargin(lMargin)
	, rMargin(rMargin)
	, uMargin(uMargin)
	, dMargin(dMargin)
	, bgColor(bgColor)
	, hoverColor(hoverColor)
	, onHover(onHover)
	, onClick(onClick)
	, offHover(offHover)
	, changesScene(changesScene)
	, borderColor(glm::vec4(0.0f))
	, borderWidth(0.0f)
{
	this->buttonType = ButtonType::TEXT;

	GLfloat vertices[12]
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
			((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) * *UIElement::WINDOWHEIGHT // t
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

Button::Button(Text* text, float lMargin, float rMargin, float uMargin, float dMargin, float borderWidth, glm::vec4 bgColor, glm::vec4 hoverColor, glm::vec4 borderColor, void (*onHover)(), void(*onClick)(), void(*offHover)(), bool changesScene)
	: UIElement
	(
		text->centerX,
		text->centerY,
		text->width / *UIElement::WINDOWWIDTH,
		text->height / *UIElement::WINDOWHEIGHT
	)
	, text(text)
	, lMargin(lMargin)
	, rMargin(rMargin)
	, uMargin(uMargin)
	, dMargin(dMargin)
	, borderWidth(borderWidth)
	, bgColor(bgColor)
	, hoverColor(hoverColor)
	, borderColor(borderColor)
	, onHover(onHover)
	, onClick(onClick)
	, offHover(offHover)
	, changesScene(changesScene)
{
	this->buttonType = ButtonType::TEXT_BORDER;

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



		(((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) - ((this->borderWidth * (*UIElement::WINDOWHEIGHT / (float)*UIElement::WINDOWWIDTH)) * ((this->height / 2.0f)))) * *UIElement::WINDOWWIDTH, // l
			(((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) + (this->borderWidth * ((this->height / 2.0f)))) * *UIElement::WINDOWHEIGHT, // t		

		(((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) - ((this->borderWidth * (*UIElement::WINDOWHEIGHT / (float)*UIElement::WINDOWWIDTH)) * ((this->height / 2.0f)))) * *UIElement::WINDOWWIDTH, // l
			(((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)) - (this->borderWidth * ((this->height / 2.0f)))) * *UIElement::WINDOWHEIGHT, //  b  		

		(((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) + ((this->borderWidth * (*UIElement::WINDOWHEIGHT / (float)*UIElement::WINDOWWIDTH)) * ((this->height / 2.0f)))) * *UIElement::WINDOWWIDTH, // r
			(((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) + (this->borderWidth * ((this->height / 2.0f)))) * *UIElement::WINDOWHEIGHT, // t				


		(((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) - ((this->borderWidth * (*UIElement::WINDOWHEIGHT / (float)*UIElement::WINDOWWIDTH)) * ((this->height / 2.0f)))) * *UIElement::WINDOWWIDTH, // l
			(((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)) - (this->borderWidth * ((this->height / 2.0f)))) * *UIElement::WINDOWHEIGHT, //  b  		

		(((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) + ((this->borderWidth * (*UIElement::WINDOWHEIGHT / (float)*UIElement::WINDOWWIDTH)) * ((this->height / 2.0f)))) * *UIElement::WINDOWWIDTH, // r
			(((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)) - (this->borderWidth * ((this->height / 2.0f)))) * *UIElement::WINDOWHEIGHT, //  b  		

		(((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) + ((this->borderWidth * (*UIElement::WINDOWHEIGHT / (float)*UIElement::WINDOWWIDTH)) * ((this->height / 2.0f)))) * *UIElement::WINDOWWIDTH, // r
			(((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) + (this->borderWidth * ((this->height / 2.0f)))) * *UIElement::WINDOWHEIGHT // t		
	};

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0x00);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Button::Button(Image* image, glm::vec4 bgColor, void (*onHover)(), void(*onClick)(), void(*offHover)(), bool changesScene)
	: UIElement
	(
		image->centerX,
		image->centerY,
		image->width,
		image->height
	)
	, image(image)
	, bgColor(bgColor)
	, onHover(onHover)
	, onClick(onClick)
	, offHover(offHover)
	, changesScene(changesScene)
	, hoverColor(glm::vec4(0.0f))
	, borderColor(glm::vec4(0.0f))
	, borderWidth(0.0f)
	, lMargin(0.0f)
	, rMargin(0.0f)
	, uMargin(0.0f)
	, dMargin(0.0f)
{
	this->buttonType = ButtonType::IMAGE;

	GLfloat vertices[12]
	{
		(this->centerX - (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY + (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,
		(this->centerX - (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY - (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,
		(this->centerX + (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY + (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,

		(this->centerX - (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY - (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,
		(this->centerX + (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY - (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,
		(this->centerX + (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY + (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,
	};

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0x00);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Button::~Button()
{
	if (this->text)
		delete this->text;

	if (this->image)
		delete this->image;

	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);

#ifdef _DEBUG
	std::cout << "~Button" << std::endl;
#endif
}

void Button::init()
{
#ifdef _DEBUG
	Button::buttonShader = std::unique_ptr<Shader>(new Shader(".\\shaders\\button.vert", ".\\shaders\\button.frag", true));
#else
	Button::buttonShader = std::unique_ptr<Shader>(new Shader(".\\..\\shaders\\button.vert", ".\\..\\shaders\\button.frag", true));
#endif
}

void Button::cleanup()
{
	Button::buttonShader->deleteShader();
}

bool Button::isHovering(float mouseX, float mouseY)
{
	if (!this->visible)
		return false;

    // mouseX is greater than left side of button and less than right side of button (inside button)
	if (mouseX > (((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) - (this->borderWidth * ((this->height / 2.0f)))) * *UIElement::WINDOWWIDTH &&
		mouseX < (((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) + (this->borderWidth * ((this->height / 2.0f)))) * *UIElement::WINDOWWIDTH &&
		(*UIElement::WINDOWHEIGHT - mouseY) > (((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)) - (this->borderWidth * ((this->height / 2.0f)))) * *UIElement::WINDOWHEIGHT &&
		(*UIElement::WINDOWHEIGHT - mouseY) < (((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) + (this->borderWidth * ((this->height / 2.0f)))) * *UIElement::WINDOWHEIGHT)
	{
		if (!this->hovering)
		{
			this->hovering = true;
			this->onHover();
		}
	}
	else
	{
		if (this->hovering)
		{
			this->hovering = false;
			this->offHover();
		}
	}

	return this->hovering;
}


void Button::update()
{
	if (this->text)
	{
		this->text->update();

		this->width = this->text->width / *UIElement::WINDOWWIDTH;
		this->height = this->text->height / *UIElement::WINDOWHEIGHT;
	}

	if (this->image)
	{
		this->image->update();

		this->centerX = this->image->centerX;
		this->centerY = this->image->centerY;
		this->width = this->image->width;
		this->height = this->image->height;
	}

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	if (this->buttonType == ButtonType::BLANK_BORDER || this->buttonType == ButtonType::TEXT_BORDER)
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



			(((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) - ((this->borderWidth * (*UIElement::WINDOWHEIGHT / (float)*UIElement::WINDOWWIDTH)) * ((this->height / 2.0f)))) * *UIElement::WINDOWWIDTH, // l
				(((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) + (this->borderWidth * ((this->height / 2.0f)))) * *UIElement::WINDOWHEIGHT, // t		

			(((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) - ((this->borderWidth * (*UIElement::WINDOWHEIGHT / (float)*UIElement::WINDOWWIDTH)) * ((this->height / 2.0f)))) * *UIElement::WINDOWWIDTH, // l
				(((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)) - (this->borderWidth * ((this->height / 2.0f)))) * *UIElement::WINDOWHEIGHT, //  b  		

			(((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) + ((this->borderWidth * (*UIElement::WINDOWHEIGHT / (float)*UIElement::WINDOWWIDTH)) * ((this->height / 2.0f)))) * *UIElement::WINDOWWIDTH, // r
				(((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) + (this->borderWidth * ((this->height / 2.0f)))) * *UIElement::WINDOWHEIGHT, // t				


			(((this->centerX - (this->width / 2.0f)) - ((this->width / 2.0f) * this->lMargin)) - ((this->borderWidth * (*UIElement::WINDOWHEIGHT / (float)*UIElement::WINDOWWIDTH)) * ((this->height / 2.0f)))) * *UIElement::WINDOWWIDTH, // l
				(((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)) - (this->borderWidth * ((this->height / 2.0f)))) * *UIElement::WINDOWHEIGHT, //  b  		

			(((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) + ((this->borderWidth * (*UIElement::WINDOWHEIGHT / (float)*UIElement::WINDOWWIDTH)) * ((this->height / 2.0f)))) * *UIElement::WINDOWWIDTH, // r
				(((this->centerY - (this->height / 2.0f)) - ((this->height / 2.0f) * this->dMargin)) - (this->borderWidth * ((this->height / 2.0f)))) * *UIElement::WINDOWHEIGHT, //  b  		

			(((this->centerX + (this->width / 2.0f)) + ((this->width / 2.0f) * this->rMargin)) + ((this->borderWidth * (*UIElement::WINDOWHEIGHT / (float)*UIElement::WINDOWWIDTH)) * ((this->height / 2.0f)))) * *UIElement::WINDOWWIDTH, // r
				(((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) + (this->borderWidth * ((this->height / 2.0f)))) * *UIElement::WINDOWHEIGHT // t		
		};

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	}
	else if (this->buttonType == ButtonType::IMAGE)
	{
		GLfloat vertices[12]
		{
			(this->centerX - (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY + (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,
			(this->centerX - (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY - (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,
			(this->centerX + (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY + (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,

			(this->centerX - (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY - (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,
			(this->centerX + (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY - (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,
			(this->centerX + (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY + (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,
		};

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	}
	else
	{
		GLfloat vertices[12]
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
				((this->centerY + (this->height / 2.0f)) + ((this->height / 2.0f) * this->uMargin)) * *UIElement::WINDOWHEIGHT // t
		};

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	}
}

void Button::render()
{
	if (!this->visible)
		return;

	Button::buttonShader->use();
	Button::buttonShader->setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(UIElement::projection));

	glBindVertexArray(this->VAO);

	if (this->image)
	{
		Button::buttonShader->set4fv("color", 1, glm::value_ptr(this->bgColor));
		glDrawArrays(GL_TRIANGLES, 0, 6);

		this->image->render();

		return;
	}

	if (this->buttonType == ButtonType::BLANK_BORDER || this->buttonType == ButtonType::TEXT_BORDER)
	{
		Button::buttonShader->set4fv("color", 1, glm::value_ptr(this->borderColor));
		glDrawArrays(GL_TRIANGLES, 6, 6);
	}

	if (this->hovering)
		Button::buttonShader->set4fv("color", 1, glm::value_ptr(this->hoverColor));
	else
		Button::buttonShader->set4fv("color", 1, glm::value_ptr(this->bgColor));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	if (this->text)
		this->text->render();
}
