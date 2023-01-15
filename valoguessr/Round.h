#pragma once

#include <ctime>
#include <cstdint>

#include "Map.h"

struct Round
{
	glm::vec2 guessCoords;
	uint32_t points;
	Map map;

	// constructs round object given the user's guess coordinates
	Round(Map map, glm::vec2 guessCoords);

	// constructor for deserialization
	Round(Map map, glm::vec2 guessCoords, uint32_t points);

	// calculates point value with current member values and assigns it to this->points
	void calculatePoints();
};
