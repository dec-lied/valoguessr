#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

#ifdef _DEBUG
#include <iostream>
#endif

// enum assigning real valorant map to map object
enum MapEnum
{
	ASCENT		=	1,
	BIND		=	2,
	BREEZE		=	3,
	FRACTURE	=	4,
	HAVEN		=	5,
	ICEBOX		=	6,
	PEARL		=	7,
	SPLIT		=	8
};

struct Map
{
	// universal members
	// char array for buffer in serialization + deserialization
	char mapPath[21];

	// enum describing which map this object is
	MapEnum mapType;

	// correct coordinates (assigned by me)
	glm::vec2 correctCoords;

	// constructor ONLY for deserialization
	Map();

	// constructor given filepath and correct coordinates
	Map(std::string mapPath, glm::vec2 correctCoords);
};