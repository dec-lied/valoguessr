#include "Round.h"

Round::Round(Map map, glm::vec2 guessCoords)
	: guessCoords(guessCoords)
	, points(0)
	, map(map)
{

}

Round::Round(Map map, glm::vec2 guessCoords, uint32_t points)
	: map(map)
	, guessCoords(guessCoords)
	, points(points)
{

}

void Round::calculatePoints()
{
	// pythagoras thereoum: sqrt(a^2 + b^2)
	float a = this->guessCoords[0] - this->map.correctCoords[0];
	float b = this->guessCoords[1] - this->map.correctCoords[1];

	float distance = ::sqrtf(::powf(a, 2) + ::powf(b, 2));
	// (0,0) - (1,1) -> 0 - sqrt(2)

	// point assignment
	if (distance <= 0.0085f)	// close enough
		this->points = 5000;
	else if (distance >= 0.5f)	// too far
		this->points = 0;
	else						// in the middle
		this->points = 5000 - (uint32_t)((distance - 0.0085f) * 7500); // sqrt .5
}