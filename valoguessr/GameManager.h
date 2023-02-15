#pragma once

#include <Windows.h>

#include <algorithm>

#include "Game.h"
#include "Config.h"
#include "UIManager.h"

class GM
{
public:
	// singleton methods
	GM();
	~GM();
	static GM& getInstance();

	// game slot: current and only pointer to game object
	Game* gameSlot;

	// config member
	Config config;

	// constantly tracks mouse x and y pos
	double mouseX, mouseY;

	// original points for imported game
	uint32_t importedPoints;

	// all available maps to choose from
	Map allMaps[3][6 * 8];

	// current pool of maps according to difficulty & map choice
	std::vector<Map> filteredMaps;

	// vector to not allow duplicate maps per game
	std::vector<uint8_t> usedMaps;

	// filters maps based on settings and puts in GM::getInstance().filterMaps
	void filterMaps();

	// returns the index of a map relative to the diffivulty based on its name
	size_t mapIDOf(unsigned difficulty, const char* mapPath);

	// returns true if current clipboard follows game import regex
	bool validateImportString();

	// creates a game instance in GM::getInstance().gameSlot
	void newGame();
	void newFreeplay();
	void newImport(std::string importString);

	// updates GM::getInstance().gameSlot to proceed to next round
	void nextRound();

	// deletes game instance in GM::getInstance().gameSlot. if save then write to game history (STANDARD AND IMPORT ONLY)
	void stopGame(bool save);

	// calculates stats based on game history and updates config
	void calcStats();

	// calculates achievements (excluding "Play Freeplay") based on game history and updates config
	void calcAchievements();
};