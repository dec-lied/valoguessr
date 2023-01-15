#include "Config.h"

Config::Config()
	: playCount(d_playCount)
	, Ws(d_Ws)
	, Ls(d_Ls)
	, pointRecord(d_pointRecord)
	, lowestPoints(d_lowestPoints)
	, totalPoints(d_totalPoints)
	, achievements(d_achievements)
	, difficulty(d_difficulty)
	, mapChoice(d_mapChoice)
	, games(d_games)
{
	this->deserialize();
}

Config::~Config()
{
	this->serialize();
}

void Config::serialize()
{
#ifdef _DEBUG
	if (!std::filesystem::exists(".\\config\\"))
		std::filesystem::create_directory(".\\config\\");
#else
	if (!std::filesystem::exists(".\\..\\config\\"))
		std::filesystem::create_directory(".\\..\\config\\");
#endif

#ifdef _DEBUG
	std::ofstream ofstream(".\\config\\config.valog", std::ios::out | std::ios::trunc | std::ios::binary);
#else
	std::ofstream ofstream(".\\..\\config\\config.valog", std::ios::out | std::ios::trunc | std::ios::binary);
#endif

	uint8_t mapChoiceByte = 0, achievementByte = 0;

	for (int i = 0; i < 8; i++)
	{
		mapChoiceByte |= ((bool)(this->mapChoice[i])) << i;
		achievementByte |= ((bool)(this->achievements[i])) << i;
	}

	ofstream.write((char*)&(this->difficulty), sizeof(this->difficulty)); // current difficulty
	ofstream.write((char*)&mapChoiceByte, sizeof(mapChoiceByte)); // map choice bitset
	ofstream.write((char*)&achievementByte, sizeof(achievementByte)); // achievements bitset

	for (size_t i = 0; i < this->games.size(); i++)
	{
		ofstream.write((char*)&this->games[i], sizeof(time_t) + sizeof(GameType) + sizeof(uint8_t)); // time and difficulty of game
		for (size_t j = 0; j < 5; j++)
			ofstream.write((char*)&this->games[i].rounds[j], sizeof(Round)); // 5 rounds
	}

	ofstream.close();
}

void Config::deserialize()
{
#ifdef _DEBUG
	std::cout << std::endl << "DESERIALIZATION INFO: " << std::endl << std::endl;
#endif

	if (!std::filesystem::exists(".\\..\\config\\"))
		std::filesystem::create_directory(".\\..\\config\\");

#ifdef _DEBUG
	std::ifstream ifstream(".\\config\\config.valog", std::ios::ate | std::ios::binary);
#else
	std::ifstream ifstream(".\\..\\config\\config.valog", std::ios::ate | std::ios::binary);
#endif

	this->resetSettings();

	// std::ios::ate starts ifstream at end, so this is the final byte #
	size_t endByte = ifstream.tellg();
	ifstream.seekg(std::ios::beg);

	// deserializes file info into pointer
	char* deserializedByteStream = (char*)::malloc(endByte);
	ifstream.read((char*)deserializedByteStream, endByte);
	ifstream.close();

	if (endByte <= 0)
		return;

	if (!deserializedByteStream)
	{
#ifdef _DEBUG
		std::cout << "failed to read settings into deserialized byte stream" << std::endl;
#endif

		return;
	}

	// assigning per user information
	if (*(uint8_t*)(deserializedByteStream) < 1 || *(uint8_t*)(deserializedByteStream) > 3)
	{
#ifdef _DEBUG
		std::filesystem::remove(".\\config\\config.valog");
#else
		std::filesystem::remove(".\\..\\config\\config.valog");
#endif
		return;
	}

	this->difficulty = *(uint8_t*)(deserializedByteStream); // current difficulty

	if (endByte <= 1)
		return;

	this->mapChoice = *(uint8_t*)(deserializedByteStream + 1); // map choice bitset

	if (endByte <= 2)
		return;

	this->achievements = *(uint8_t*)(deserializedByteStream + 2); // achievements bitset

	if (endByte <= 3)
		return;

	size_t amountOfGames = (endByte - 3) / (sizeof(time_t) + sizeof(GameType) + sizeof(uint8_t) + (sizeof(Round) * 5));

#ifdef _DEBUG
	std::cout << endByte - 3 << " remaining bytes" << std::endl;
	std::cout << endByte - 3 << " / " << (sizeof(time_t) + sizeof(GameType) + sizeof(uint8_t) + (sizeof(Round) * 5)) << " = " << amountOfGames << std::endl << std::endl;
#endif

	// assigning per game information
	for (size_t i = 0; i < amountOfGames; i++)

	{
		// base pointer to current game
		char* lStream = (deserializedByteStream + 3) + (i * (sizeof(time_t) + sizeof(GameType) + sizeof(uint8_t) + (5 * sizeof(Round))));

		// time of game
		time_t gameTime = *(time_t*)lStream;
		lStream += sizeof(time_t);

		// type of game
		GameType gameType = *(GameType*)lStream;
		lStream += sizeof(GameType);

		// difficulty of game
		uint8_t gameDiff = *(uint8_t*)lStream;
		lStream++;

		// rounds for deserialization
		std::vector<Round> deserializedRounds;
		deserializedRounds.reserve(5);

		// per round information
		for (size_t j = 0; j < 5; j++)
		{
			// base pointer to current round
			char* lRound = lStream + (sizeof(Round) * j);

			// round's guess coords
			glm::vec2 guessCoords = *(glm::vec2*)lRound;
			lRound += sizeof(glm::vec2);

			// round's points
			uint32_t points = *(uint32_t*)lRound;
			lRound += sizeof(uint32_t);

			// round's map
			Map map = *(Map*)lRound;
			lRound += sizeof(Map);

			// appends round to deserialized round vector
			deserializedRounds.push_back({map, guessCoords, points });

#ifdef _DEBUG
			std::cout << "game " << i + 1 << " round " << j + 1 << " data:" << std::endl;
			std::cout << "mapPath: " << map.mapPath << std::endl;
			std::cout << "mapType: " << map.mapType << std::endl;
			std::cout << "correct coords: (" << map.correctCoords[0] << ", " << map.correctCoords[1] << ")" << std::endl;
			std::cout << "guess coords: (" << guessCoords[0] << ", " << guessCoords[1] << ")" << std::endl;
			std::cout << "points: " << points << std::endl;
			std::cout << std::endl << std::endl;
#endif
		}

		// finally appends game to main config's games
		this->games.push_back({ deserializedRounds, gameDiff, gameTime, gameType });
	}

	// frees file data
	::free(deserializedByteStream);
}

void Config::deleteFile()
{
#ifdef _DEBUG
	std::filesystem::remove(".\\config\\config.valog");
#else
	std::filesystem::remove(".\\..\\config\\config.valog");
#endif
}

void Config::resetSHA()
{
	this->playCount = this->d_playCount;
	this->lowestPoints = this->d_lowestPoints;
	this->pointRecord = this->d_pointRecord;
	this->totalPoints = this->d_totalPoints;
	this->Ws = this->d_Ws;
	this->Ls = this->d_Ls;

	this->achievements = this->d_achievements;

	this->games.clear();
}

void Config::resetSettings()
{
	this->difficulty = this->d_difficulty;
	this->mapChoice = this->d_mapChoice;
}