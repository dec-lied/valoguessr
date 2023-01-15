#include "Text.h"

std::unique_ptr<Shader> Text::textShader = nullptr;
std::map<GLchar, FTChar> Text::FTChars;
GLuint Text::VAO, Text::VBO;

FT_Library Text::ftlib;
FT_Face Text::face;

unsigned Text::pixelHeight;

float* Text::ratioW, *Text::ratioH;

Text::Text(std::string text, float centerX, float centerY, float scaleX, float scaleY, glm::vec4 textColor, bool lock)
	: text(text)
	, realX(0.0f)
	, realY(0.0f)
	, origRealX(0.0f)
	, origRealY(0.0f)
	, UIElement(centerX, centerY, 0.0f, 0.0f)
	, scaleX(scaleX)
	, scaleY(scaleY)
	, textColor(textColor)
	, lock(lock)
{
	for (std::string::const_iterator c = text.begin(); c != text.end(); c++)
	{
		FTChar ch = Text::FTChars[*c];

		if (!(c == text.end() - 1))
			this->width += (ch.Advance >> 6) * (this->scaleX * *Text::ratioW);
		else
		{
			this->width += ch.Bearing.x * (this->scaleX * *Text::ratioW);
			this->width += ch.Size.x * (this->scaleX * *Text::ratioW);
		}
	}

	this->height = Text::FTChars['T'].Size.y * (this->scaleY * *Text::ratioH);

	this->realX = (centerX * *UIElement::WINDOWWIDTH) - (this->width / 2.0f);
	this->realY = (centerY * *UIElement::WINDOWHEIGHT) - (this->height / 2.0f);

	this->origRealX = realX / *UIElement::WINDOWWIDTH;
	this->origRealY = realY / *UIElement::WINDOWHEIGHT;
}

Text::~Text()
{
#ifdef _DEBUG
	std::cout << "~Text" << std::endl;
#endif
}

void Text::init(const char* fontName, float* ratioW, float* ratioH, unsigned pixelHeight)
{
	// freetype setup
	if (FT_Init_FreeType(&Text::ftlib))
	{
#ifdef _DEBUG
		std::cout << "failed to initialize freetype" << std::endl;
#endif
		return;
	}

	if (FT_New_Face(Text::ftlib, fontName, 0, &face))
	{
#ifdef _DEBUG
		std::cout << "failed to load " << fontName << std::endl;
#endif
		return;
	}

	Text::ratioW = ratioW;
	Text::ratioH = ratioH;

#ifdef _DEBUG
	Text::textShader = std::unique_ptr<Shader>(new Shader(".\\shaders\\text.vert", ".\\shaders\\text.frag", true));
#else
	Text::textShader = std::unique_ptr<Shader>(new Shader(".\\..\\shaders\\text.vert", ".\\..\\shaders\\text.frag", true));
#endif

	Text::pixelHeight = pixelHeight;
	FT_Set_Pixel_Sizes(Text::face, 0, Text::pixelHeight);

	for (unsigned char c = 0; c < 128; c++)
	{
		if (FT_Load_Char(Text::face, c, FT_LOAD_RENDER))
		{
#ifdef _DEBUG
			std::cout << "failed to load glyph: " << (char)c << std::endl;
#endif
			continue;
		}

		GLuint texture;
		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			Text::face->glyph->bitmap.width,
			Text::face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			Text::face->glyph->bitmap.buffer
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		FTChar character
		{
			texture,
			glm::ivec2(Text::face->glyph->bitmap.width, Text::face->glyph->bitmap.rows),
			glm::ivec2(Text::face->glyph->bitmap_left, Text::face->glyph->bitmap_top),
			static_cast<unsigned int>(Text::face->glyph->advance.x)
		};

		Text::FTChars.insert(std::pair<char, FTChar>(c, character));
	}

	FT_Done_Face(Text::face);
	FT_Done_FreeType(Text::ftlib);

	glGenVertexArrays(1, &Text::VAO);
	glGenBuffers(1, &Text::VBO);

	glBindVertexArray(Text::VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Text::VBO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0x00);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 2));
	glEnableVertexAttribArray(1);

	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Text::cleanup()
{
	for (unsigned char c = 0; c < 128; c++)
		glDeleteTextures(1, &Text::FTChars[c].TextureID);

	glDeleteVertexArrays(1, &Text::VAO);
	glDeleteBuffers(1, &Text::VBO);

	Text::textShader->deleteShader();
}

void Text::changeText(std::string text)
{
	this->text = text;
}

void Text::update()
{
	this->width = 0.0f;
	for (std::string::const_iterator c = this->text.begin(); c != this->text.end(); c++)
	{
		FTChar ch = Text::FTChars[*c];

		if (!(c == this->text.end() - 1))
			this->width += (ch.Advance >> 6) * (this->scaleX * *Text::ratioW);
		else
		{
			this->width += ch.Bearing.x * (this->scaleX * *Text::ratioW);
			this->width += ch.Size.x * (this->scaleX * *Text::ratioW);
		}
	}

	this->height = Text::FTChars['T'].Size.y * (this->scaleY * *Text::ratioH);
}

void Text::render()
{
	if (!this->visible)
		return;

	if (this->lock)
	{
		this->realX = origRealX * *UIElement::WINDOWWIDTH;
		this->realY = origRealY * *UIElement::WINDOWHEIGHT;
	}
	else
	{
		this->realX = (this->centerX * *UIElement::WINDOWWIDTH) - (this->width / 2.0f);
		this->realY = (this->centerY * *UIElement::WINDOWHEIGHT) - (this->height / 2.0f);
	}

	glBindVertexArray(Text::VAO);

	Text::textShader->use();
	Text::textShader->set4fv("textColor", 1, glm::value_ptr(this->textColor));
	Text::textShader->setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(UIElement::projection));

	glActiveTexture(GL_TEXTURE0);

	for (std::string::const_iterator c = this->text.begin(); c != this->text.end(); c++)
	{
		FTChar ch = Text::FTChars[*c];

		float xpos = this->realX + (ch.Bearing.x * scaleX * *Text::ratioW);
		float ypos = this->realY - ((ch.Size.y - ch.Bearing.y) * (scaleY * *Text::ratioH));

		float w = ch.Size.x * (scaleX * *Text::ratioW);
		float h = ch.Size.y * (scaleY * *Text::ratioH);

		GLfloat vertices[6][4]
		{
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		this->realX += (ch.Advance >> 6) * (scaleX * *Text::ratioW);

		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, Text::VBO);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
