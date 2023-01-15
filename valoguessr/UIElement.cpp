#include "UIElement.h"

unsigned *UIElement::WINDOWWIDTH, *UIElement::WINDOWHEIGHT;
glm::mat4 UIElement::projection;

UIElement::UIElement(float centerX, float centerY, float width, float height)
	: centerX(centerX)
	, centerY(centerY)
	, width(width)
	, height(height)
{

}