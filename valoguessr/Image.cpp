#include "Image.h"

std::unique_ptr<Shader> Image::imageShader;

Image::Image(std::string imgPath, bool alpha, float centerX, float centerY, float width, float height, float opacity)
	: imgPath(imgPath)
	, texture(imgPath.c_str(), alpha)
	, UIElement(centerX, centerY, width, height)
	, opacity(opacity)
{
	GLfloat vertices[]
	{
		(this->centerX - (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY + (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,		0.0f, 1.0f, // tl
		(this->centerX - (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY - (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,		0.0f, 0.0f, // bl
		(this->centerX + (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY + (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,		1.0f, 1.0f, // tr
																		
		(this->centerX - (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY - (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,		0.0f, 0.0f, // bl
		(this->centerX + (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY - (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,		1.0f, 0.0f, // br
		(this->centerX + (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY + (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,		1.0f, 1.0f  // tr
	};

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0x00);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Image::~Image()
{
	this->texture.deleteTexture();

	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);

#ifdef _DEBUG
	std::cout << "~Image" << std::endl;
#endif
}

void Image::init()
{
#ifdef _DEBUG
	Image::imageShader = std::unique_ptr<Shader>(new Shader(".\\shaders\\image.vert", ".\\shaders\\image.frag", true));
#else
	Image::imageShader = std::unique_ptr<Shader>(new Shader(".\\..\\shaders\\image.vert", ".\\..\\shaders\\image.frag", true));
#endif
}

void Image::cleanup()
{
	Image::imageShader->deleteShader();
}

void Image::changeTexture(std::string imgPath, bool alpha)
{
	this->imgPath = imgPath;

	this->texture.replaceTexture(this->imgPath.c_str(), alpha);
}

void Image::update()
{
	GLfloat vertices[]
	{
		(this->centerX - (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY + (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,		0.0f, 1.0f, // tl
		(this->centerX - (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY - (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,		0.0f, 0.0f, // bl
		(this->centerX + (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY + (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,		1.0f, 1.0f, // tr
																		
		(this->centerX - (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY - (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,		0.0f, 0.0f, // bl
		(this->centerX + (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY - (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,		1.0f, 0.0f, // br
		(this->centerX + (this->width / 2.0f)) * *UIElement::WINDOWWIDTH, (this->centerY + (this->height / 2.0f)) * *UIElement::WINDOWHEIGHT,		1.0f, 1.0f  // tr
	};

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
}

void Image::render()
{
	if (!this->visible)
		return;

	Image::imageShader->use();
	Image::imageShader->setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(UIElement::projection));
	Image::imageShader->setFloat("opacity", this->opacity);

	glBindVertexArray(this->VAO);
	this->texture.bindTexture();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}