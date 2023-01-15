#pragma once

#include "Text.h"

#include <vector>

struct scrollUpdate
{
	float timeReq;
	std::string newText;
};

class ScrollText : public Text
{
private:
	// internal
	float anchorTime;
	unsigned charCount;
	std::string scrollText;
	const std::string rootText;

public:
	// static members
	static float scrollSpeed;

	// universal members
	std::vector<scrollUpdate> updateQueue;

	unsigned phase;
	bool inf = false;

	ScrollText(std::string text, float centerX, float centerY, float scaleX, float scaleY, glm::vec4 textColor, std::vector<scrollUpdate> updateQueue);
	~ScrollText();

	// universal methods
	// returns elapsed time since anchorTime
	float elapsed();

	// sets anchorTime to current time
	void anchor();

	// update queue methods
	// adds update to the updateQueue
	void pushUpdate(scrollUpdate update);

	// commits update and dequeues it from the updateQueue
	void dequeueUpdate();

	// inherited
	void update();
	void render();
};

