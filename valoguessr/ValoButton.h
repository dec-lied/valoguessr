#include "Button.h"

struct ValoTri
{
	float x1, y1, x2, y2, x3, y3;
};

class ValoButton : public Button
{
public:
	static bool mouseInTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float mouseX, float mouseY);

	float sidesWidth;
	ValoTri lTri, rTri;

	ValoButton(Text* text, float lMargin, float rMargin, float uMargin, float dMargin, float sidesWidth, glm::vec4 bgColor, glm::vec4 hoverColor, void (*onHover)(), void(*onClick)());

	bool isHovering(float mouseX, float mouseY);

	void update();
	void render();
};