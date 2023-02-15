#pragma once

#include <vector>

#include "Round.h"

// enum to describe the type of game: STANDARD, FREEPLAY, & IMPORT
enum GameType
{
	STANDARD	=	1,
	FREEPLAY	=	2,
	IMPORT		=	3
};

class Game
{
public:
	// static members
	// to keep track of index for gameSlot's current round
	static unsigned roundIndex;

	// universal members
	// time of creation of game (typedef long long)
	time_t time;
	
	// game type enum
	GameType gameType;

	// need difficulty of game at time of creation for deserialization + importing
	uint8_t difficulty;

	// should only every be size 5 but sadly has to be a vector. do NOT want a default round constructor so cant use array.
	std::vector<Round> rounds;

	// constructor for general new game
	Game(const std::vector<Map>& maps, uint8_t difficulty, GameType gameType);

	// constructor for deserialization
	Game(std::vector<Round> rounds, uint8_t difficulty, time_t time, GameType gameType);
	
	// fills out round information and appends filler round
	void nextRound(glm::vec2 guessCoords);

	// returns the sum of all 5 rounds' points
	uint32_t sumOfRounds();

	// equivalent to GM::getInstance().gameSlot->rounds[GM::getInstance().roundIndex]
	Round& getCurrentRound();
};

