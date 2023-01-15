#include "ScrollText.h"

float ScrollText::scrollSpeed;

ScrollText::ScrollText(std::string text, float centerX, float centerY, float scaleX, float scaleY, glm::vec4 textColor, std::vector<scrollUpdate> updateQueue)
	: Text(text, centerX, centerY, scaleX, scaleY, textColor)
	, scrollText("")
	, rootText(text)
	, anchorTime((float)glfwGetTime())
	, charCount(0)
	, updateQueue(updateQueue)
	, phase(0)
{

}

ScrollText::~ScrollText()
{
	this->updateQueue.clear();

#ifdef _DEBUG
	std::cout << "~ScrollText" << std::endl;
#endif
}

float ScrollText::elapsed()
{
	return (float)glfwGetTime() - this->anchorTime;
}

void ScrollText::anchor()
{
	this->charCount = 0;
	this->scrollText = "";
	this->anchorTime = (float)glfwGetTime();
}

void ScrollText::pushUpdate(scrollUpdate update)
{
	this->updateQueue.push_back(update);
}

void ScrollText::dequeueUpdate()
{
	if (this->updateQueue.size() > 0 && this->elapsed() >= this->updateQueue[0].timeReq + (strlen(this->text.c_str()) / ScrollText::scrollSpeed))
	{
#ifdef _DEBUG
		std::cout << "changing text from \"" << this->text << "\" to \"" << this->updateQueue[0].newText << "\"" << std::endl;
#endif
		this->phase++;
		this->text = this->updateQueue[0].newText;

		if (!this->inf)
			this->updateQueue.erase(this->updateQueue.begin());

		this->anchor();
	}
#ifdef _DEBUG
	else
		if (this->updateQueue.size() > 0 && ::fmod(this->elapsed(), 1.0f) < 0.0125f)
			std::cout << this->text << ":\t" << this->elapsed() << "/" << this->updateQueue[0].timeReq + (strlen(this->text.c_str()) / ScrollText::scrollSpeed) << std::endl;
#endif
}

void ScrollText::update()
{
	this->width = 0.0f;
	for (std::string::const_iterator c = this->rootText.begin(); c != this->rootText.end(); c++)
	{
		FTChar ch = Text::FTChars[*c];

		if (!(c == this->rootText.end() - 1))
			this->width += (ch.Advance >> 6) * (this->scaleX * *Text::ratioW);
		else
		{
			this->width += ch.Bearing.x * (this->scaleX * *Text::ratioW);
			this->width += ch.Size.x * (this->scaleX * *Text::ratioW);
		}
	}

	this->height = Text::FTChars['T'].Size.y * (this->scaleY * *Text::ratioH);
}

void ScrollText::render()
{
	if (!this->visible)
		return;

	this->dequeueUpdate();

	if (this->charCount < this->text.size())
	{
		this->charCount = (unsigned)(this->elapsed() * ScrollText::scrollSpeed);
		this->scrollText = this->text.substr(0, this->charCount);
	}
	else if (this->scrollText.size() != this->text.size())
		this->scrollText = this->text;

	glBindVertexArray(Text::VAO);

	Text::textShader->use();
	Text::textShader->setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(UIElement::projection));
	Text::textShader->set4fv("textColor", 1, glm::value_ptr(this->textColor));

	glActiveTexture(GL_TEXTURE0);

	this->realX = (this->centerX * *UIElement::WINDOWWIDTH) - (this->width / 2.0f);
	this->realY = (this->centerY * *UIElement::WINDOWHEIGHT) - (this->height / 2.0f);

	for (std::string::const_iterator c = this->scrollText.begin(); c != this->scrollText.end(); c++)
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

		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, Text::VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		this->realX += (ch.Advance >> 6) * (scaleX * *Text::ratioW);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}