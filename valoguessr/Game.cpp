#include "Game.h"

unsigned Game::roundIndex = 0;

Game::Game(const std::vector<Map>& maps, uint8_t difficulty, GameType gameType)
	: time(::time(0))
	, difficulty(difficulty)
	, gameType(gameType)
{
	this->rounds.reserve(maps.size());

	for (size_t i = 0; i < maps.size(); i++)
		this->rounds.push_back(Round(maps[i], glm::vec2(0.0f, 0.0f)));

	Game::roundIndex = 0;
}

Game::Game(std::vector<Round> rounds, uint8_t difficulty, time_t time, GameType gameType)
	: time(time)
	, rounds(rounds)
	, difficulty(difficulty)
	, gameType(gameType)
{
	Game::roundIndex = 0;
}

void Game::nextRound(glm::vec2 guessCoords)
{
	this->rounds[Game::roundIndex].guessCoords = guessCoords;
	this->rounds[Game::roundIndex].calculatePoints();

#ifdef _DEBUG
	std::cout << "correct coords: (" << this->rounds[roundIndex].map.correctCoords[0] << ", " << this->rounds[roundIndex].map.correctCoords[1] << ")" << std::endl;
	std::cout << "guessed coords: (" << this->rounds[roundIndex].guessCoords[0] << ", " << this->rounds[roundIndex].guessCoords[1] << ")" << std::endl;
	std::cout << "pts: " << rounds[roundIndex].points << std::endl;
#endif

	Game::roundIndex++;
}

uint32_t Game::sumOfRounds()
{
	uint32_t sum = 0;

	for (const Round& round : this->rounds)
		sum += round.points;

	return sum;
}

Round& Game::getCurrentRound()
{
	return this->rounds[Game::roundIndex];
}
