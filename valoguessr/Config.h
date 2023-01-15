#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <bitset>
#include <vector>
#include <fstream>
#include <algorithm>
#include <filesystem>

#include "Round.h"
#include "Game.h"

class Config
{
private:
	// default stats
	uint16_t d_playCount = 0;
	uint16_t d_Ls = 0;
	uint16_t d_Ws = 0;
	uint32_t d_pointRecord = 0;
	uint32_t d_lowestPoints = 0;
	uint64_t d_totalPoints = 0;

	// default achievements
	std::bitset<8> d_achievements{ 0b00000000 };

	// default settings
	uint8_t d_difficulty = 2;
	std::bitset<8> d_mapChoice{ 0b11111111 };

	// default game history
	std::vector<Game> d_games{ };

public:
	// stats
	uint16_t playCount;
	uint16_t Ls;
	uint16_t Ws;
	uint32_t pointRecord;
	uint32_t lowestPoints;
	uint64_t totalPoints;

	/*
	achievements
	* [0]: play endless
	* [1-3]: standard 5, 10, 15
	* [4-6]: score 10k, 15k, 20k
	* [7]: perfect score
	*/
	std::bitset<8> achievements;

	// settings
	uint8_t difficulty;
	// split -> ascent
	std::bitset<8> mapChoice;

	// game history
	std::vector<Game> games;

	// base constructor
	Config();

	// base destructor
	~Config();

	// writes data to config\config.valog
	void serialize();

	// reads data from config\config.valog and sets members to it's data
	void deserialize();

	// deletes config\config.valog, resetting config data
	void deleteFile();

	// sets stats, history, and achievements
	void resetSHA();

	// sets settings to default
	void resetSettings();
};

