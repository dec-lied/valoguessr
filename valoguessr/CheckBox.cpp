#include "CheckBox.h"

unsigned char* CheckBox::checkedData, * CheckBox::uncheckedData;
GLuint CheckBox::checkedTextureID, CheckBox::uncheckedTextureID;
int CheckBox::dataW, CheckBox::dataH, CheckBox::dataNR;

CheckBox::CheckBox(float x, float y, float width, float height)
	: UIElement(x, y, width, height)
	, checked(false)
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

CheckBox::~CheckBox()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);

#ifdef _DEBUG
	std::cout << "~CheckBox" << std::endl;
#endif
}

void CheckBox::init()
{
#ifdef _DEBUG
	CheckBox::checkedData = stbi_load(".\\images\\textures\\checkedBox.png", &CheckBox::dataW, &CheckBox::dataH, &CheckBox::dataNR, 0);
	CheckBox::uncheckedData = stbi_load(".\\images\\textures\\uncheckedBox.png", &CheckBox::dataW, &CheckBox::dataH, &CheckBox::dataNR, 0);
#else
	CheckBox::checkedData = stbi_load(".\\..\\images\\textures\\checkedBox.png", &CheckBox::dataW, &CheckBox::dataH, &CheckBox::dataNR, 0);
	CheckBox::uncheckedData = stbi_load(".\\..\\images\\textures\\uncheckedBox.png", &CheckBox::dataW, &CheckBox::dataH, &CheckBox::dataNR, 0);
#endif

	glGenTextures(1, &CheckBox::checkedTextureID);
	glGenTextures(1, &CheckBox::uncheckedTextureID);

	glBindTexture(GL_TEXTURE_2D, CheckBox::checkedTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (!CheckBox::checkedData)
	{
#ifdef _DEBUG
		std::cout << "failed to load image data for .\\images\\textures\\checkedBox.png." << std::endl;
#endif
	}
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CheckBox::dataW, CheckBox::dataH, 0, GL_RGBA, GL_UNSIGNED_BYTE, CheckBox::checkedData);


	glBindTexture(GL_TEXTURE_2D, CheckBox::uncheckedTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (!CheckBox::uncheckedData)
	{
#ifdef _DEBUG
		std::cout << "failed to load image data for .\\images\\textures\\uncheckedBox.png." << std::endl;
#endif
	}
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CheckBox::dataW, CheckBox::dataH, 0, GL_RGBA, GL_UNSIGNED_BYTE, CheckBox::uncheckedData);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void CheckBox::cleanup()
{
	glDeleteTextures(1, &CheckBox::checkedTextureID);
	glDeleteTextures(1, &CheckBox::uncheckedTextureID);

	stbi_image_free(CheckBox::checkedData);
	stbi_image_free(CheckBox::uncheckedData);
}

void CheckBox::flip()
{
	this->checked = !this->checked;
	
	if (this->checked)
	{
		glBindTexture(GL_TEXTURE_2D, CheckBox::uncheckedTextureID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, CheckBox::dataW, CheckBox::dataH, GL_RGBA, GL_UNSIGNED_BYTE, CheckBox::uncheckedData);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, CheckBox::checkedTextureID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, CheckBox::dataW, CheckBox::dataH, GL_RGBA, GL_UNSIGNED_BYTE, CheckBox::checkedData);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void CheckBox::setStatus(bool status)
{
	if (this->checked != status)
		this->flip();
}

void CheckBox::update()
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

void CheckBox::render()
{
	if (!this->visible)
		return;

	Image::imageShader->use();
	Image::imageShader->setFloat("opacity", 1.0f);
	Image::imageShader->setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(UIElement::projection));

	glBindVertexArray(this->VAO);
	if (this->checked)
		glBindTexture(GL_TEXTURE_2D, CheckBox::checkedTextureID);
	else
		glBindTexture(GL_TEXTURE_2D, CheckBox::uncheckedTextureID);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}
