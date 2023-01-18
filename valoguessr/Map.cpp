#include "Map.h"

Map::Map()
	: mapPath()
	, mapType(ASCENT)
	, correctCoords(glm::vec2(0.0f, 0.0f))
{

}

Map::Map(std::string mapPath, glm::vec2 correctCoords)
	: correctCoords(correctCoords)
{
	strncpy_s(this->mapPath, mapPath.c_str(), strlen(mapPath.c_str()) + 1);

	// assigns what map type this object is based on filename
	switch (mapPath.at(0))
	{
	case 'a': // ascent
		this->mapType = MapEnum::ASCENT;
		break;

	case 'b': // bind - breeze
		if (mapPath.at(1) == 'i') // bind
			this->mapType = MapEnum::BIND;
		else if (mapPath.at(1) == 'r') // breeze
			this->mapType = MapEnum::BREEZE;
		else
		{
#ifdef _DEBUG
			std::cout << "unknown map format " << mapPath << std::endl;
#endif
			this->mapType = MapEnum::BIND;
		}

		break;

	case 'f': // fracture
		this->mapType = MapEnum::FRACTURE;
		break;

	case 'h': // haven
		this->mapType = MapEnum::HAVEN;
		break;

	case 'i': // icebox
		this->mapType = MapEnum::ICEBOX;
		break;

	case 'p': // pearl
		this->mapType = MapEnum::PEARL;
		break;

	case 's': // split
		this->mapType = MapEnum::SPLIT;
		break;

	default:
#ifdef _DEBUG
		std::cout << "unknown map format " << mapPath << std::endl;
#endif
		this->mapType = MapEnum::ASCENT;
		break;
	}
}