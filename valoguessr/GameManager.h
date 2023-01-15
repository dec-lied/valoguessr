#pragma once

#include <Windows.h>

#include <algorithm>

#include "Game.h"
#include "Config.h"
#include "UIManager.h"

class GM
{
public:
	// game slot: current and only pointer to game object
	static Game* gameSlot;

	// config member
	static Config config;

	// constantly tracks mouse x and y pos
	static double mouseX, mouseY;

	// original points for imported game
	static uint32_t importedPoints;

	// all available maps to choose from
	static Map allMaps[3][6 * 8];

	// current pool of maps according to difficulty & map choice
	static std::vector<Map> filteredMaps;

	// vector to not allow duplicate maps per game
	static std::vector<uint8_t> usedMaps;

	// filters maps based on settings and puts in GM::filterMaps
	static void filterMaps();

	// returns the index of a map relative to the diffivulty based on its name
	static size_t mapIDOf(unsigned difficulty, const char* mapPath);

	// returns true if current clipboard follows game import regex
	static bool validateImportString();

	// creates a game instance in GM::gameSlot
	static void newGame();
	static void newFreeplay();
	static void newImport(std::string importString);

	// updates GM::gameSlot to proceed to next round
	static void nextRound();

	// deletes game instance in GM::gameSlot. if save then write to game history (STANDARD AND IMPORT ONLY)
	static void stopGame(bool save);

	// calculates stats based on game history and updates config
	static void calcStats();

	// calculates achievements (excluding "Play Freeplay") based on game history and updates config
	static void calcAchievements();

	// reserves memory in vectors for performance
	static void init();
	static void cleanup();
};

