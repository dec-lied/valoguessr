#include "GameManager.h"

Game* GM::gameSlot = nullptr;

Config GM::config;
double GM::mouseX, GM::mouseY;
uint32_t GM::importedPoints = 0;

std::vector<Map> GM::filteredMaps;
std::vector<uint8_t> GM::usedMaps;

Map GM::allMaps[3][6 * 8]
{
	// easy
	{
		{ "aLever.png",				glm::vec2(0.275732f,	0.678095f)	},
		{ "aLobby.png",				glm::vec2(0.761818f,	0.369425f)	},
		{ "aPizza.png",				glm::vec2(0.523636f,	0.712116f)	},
		{ "aRafters.png",			glm::vec2(0.924657f,	0.741281f)	},
		{ "aTree.png",				glm::vec2(0.713209f,	0.726698f)	},
		{ "aMidGate.png",			glm::vec2(0.485453f,	0.581887f)	},

		{ "biAShort.png",			glm::vec2(0.618672f,	0.631164f)	},
		{ "biBShort.png",			glm::vec2(0.372593f,	0.512821f)	},
		{ "biElbow.png",			glm::vec2(0.141542f,	0.723208f)	},
		{ "biHeaven.png",			glm::vec2(0.787734f,	0.747628f)	},
		{ "biHookah.png",			glm::vec2(0.233587f,	0.516577f)	},
		{ "biUHaul.png",			glm::vec2(0.590495f,	0.687518f)	},

		{ "brATTS.png",				glm::vec2(0.80834f,		0.443754f)	},
		{ "brBPeek.png",			glm::vec2(0.0434258f,	0.618147f)	},
		{ "brHalls.png",			glm::vec2(0.623586f,	0.635414f)	},
		{ "brMid.png",				glm::vec2(0.423293f,	0.685487f)	},
		{ "brOn.png",				glm::vec2(0.393939,		0.555987f)	},
		{ "brTitties.png",			glm::vec2(0.706466f,	0.585341f)	},

		{ "fA.png",					glm::vec2(0.727186f,	0.485194f)	},
		{ "fBHeaven.png",			glm::vec2(0.0486058f,	0.545627f)	},
		{ "fBLong.png",				glm::vec2(0.309333f,	0.21756f)	},
		{ "fBLongFT.png",			glm::vec2(0.162566f,	0.246914f)	},
		{ "fBSite.png",				glm::vec2(0.355953f,	0.530087f)	},
		{ "fCTSpawn.png",			glm::vec2(0.552793f,	0.890961f)	},

		{ "hAShort.png",			glm::vec2(0.601457f,	0.548341f)	},
		{ "hCLong.png",				glm::vec2(0.134199f,	0.27417f)	},
		{ "hGarage.png",			glm::vec2(0.360956f,	0.586821f)	},
		{ "hHeaven.png",			glm::vec2(0.895211f,	0.702261f)	},
		{ "hMidWindow.png",			glm::vec2(0.51041f,		0.323874f)	},
		{ "hOutsideGarage.png",		glm::vec2(0.331753f,	0.415264f)	},

		{ "iADElevation.png",		glm::vec2(0.888339f,	0.662177f)	},
		{ "iAHeaven.png",			glm::vec2(0.757782f,	0.687831f)	},
		{ "iATElevation.png",		glm::vec2(0.661582f,	0.527497f)	},
		{ "iBHeaven.png",			glm::vec2(0.190888f,	0.575597f)	},
		{ "iMidElev.png",			glm::vec2(0.754346f,	0.562771f)	},
		{ "iOnTube.png",			glm::vec2(0.474335f,	0.45214f)	},

		{ "pABackSite.png",			glm::vec2(0.776678f,	0.747154f)	},
		{ "pArtCorner.png",			glm::vec2(0.659864f,	0.689434f)	},
		{ "pAShort.png",			glm::vec2(0.924414f,	0.448934f)	},
		{ "pBHeaven.png",			glm::vec2(0.238988f,	0.628507f)	},
		{ "pBLongCrevice.png",		glm::vec2(0.0259738f,	0.42809f)	},
		{ "pCTMid.png",				glm::vec2(0.575689f,	0.299824f)	},

		{ "sAHeaven.png",			glm::vec2(0.754346f,	0.723104f)	},
		{ "sAMid.png",				glm::vec2(0.496667f,	0.577201f)	},
		{ "sAT.png",				glm::vec2(0.87116f,		0.463364f)	},
		{ "sBHeaven.png",			glm::vec2(0.237271f,	0.602854f)	},
		{ "sGarage.png",			glm::vec2(0.0775097f,	0.471381f)	},
		{ "sRopes.png",				glm::vec2(0.570535f,	0.644541f)	}
	},

	// medium
	{
		{ "aBoatHouse.png",			glm::vec2(0.088589f,	0.73399f)	},
		{ "aCat.png",				glm::vec2(0.644306f,	0.531814f)	},
		{ "aCT2Mid.png",			glm::vec2(0.190193f,	0.464474f)	},
		{ "aMarket.png",			glm::vec2(0.421566f,	0.716567f)	},
		{ "aMidLink.png",			glm::vec2(0.454373f,	0.44548f)	},
		{ "aMidTop.png",			glm::vec2(0.540706f,	0.36778f)	},

		{ "biBags.png",				glm::vec2(0.48718f,		0.468951f)	},
		{ "biBox.png",				glm::vec2(0.750164f,	0.334367f)	},
		{ "biCrates.png",			glm::vec2(0.62055f,		0.546633f)	},
		{ "biElevator.png",			glm::vec2(0.197896f,	0.711938f)	},
		{ "biFountain.png",			glm::vec2(0.169503f,	0.381212f)	},
		{ "biGarage.png",			glm::vec2(0.32751f,		0.394477f)	},

		{ "brCTSpawn.png",			glm::vec2(0.578693f,	0.989381f)	},
		{ "brGutter.png",			glm::vec2(0.117672f,	0.39886f)	},
		{ "brHallsEntrance.png",	glm::vec2(0.65812f,		0.353967f)	},
		{ "brJail.png",				glm::vec2(0.393939f,	0.795994f)	},
		{ "brJukebox.png",			glm::vec2(0.602866f,	0.536994f)	},
		{ "brTWall.png",			glm::vec2(0.385306f,	0.302167f)	},

		{ "fABox.png",				glm::vec2(0.176379f,	0.488647f)	},
		{ "fBUnder.png",			glm::vec2(0.898127f,	0.545627f)	},
		{ "fDish.png",				glm::vec2(0.756064f,	0.694244f)	},
		{ "fDoorHeaven.png",		glm::vec2(0.623216f,	0.363636f)	},
		{ "fGar.png",				glm::vec2(0.910213f,	0.27972f)	},
		{ "fSpawnBox.png",			glm::vec2(0.632378f,	0.524291f)	},

		{ "hAConn.png",				glm::vec2(0.676126f,	0.688365f)	},
		{ "hAHell.png",				glm::vec2(0.887767f,	0.751216f)	},
		{ "hALong.png",				glm::vec2(0.868527f,	0.556678f)	},
		{ "hDoors.png",				glm::vec2(0.446849f,	0.107744f)	},
		{ "hDragon.png",			glm::vec2(0.110149f,	0.264871f)	},
		{ "hTargets.png",			glm::vec2(0.326599f,	0.624018f)	},

		{ "iFridge.png",			glm::vec2(0.472503f,	0.706322f)	},
		{ "iMidGrill.png",			glm::vec2(0.546256f,	0.612046f)	},
		{ "iMidStuff.png",			glm::vec2(0.589546f,	0.67789f)	},
		{ "iTSpawnElevation.png",	glm::vec2(0.424403f,	0.427984f)	},
		{ "iTSpawnWindow.png",		glm::vec2(0.597563f,	0.0299289f)	},
		{ "iUpTube.png",			glm::vec2(0.475709f,	0.430976f)	},

		{ "pAHall.png",				glm::vec2(0.65368f,		0.843996f)	},
		{ "pBLongFlowers.png",		glm::vec2(0.127786f,	0.465395f)	},
		{ "pBMidToArt.png",			glm::vec2(0.567099f,	0.465395f)	},
		{ "pBoxOpen.png",			glm::vec2(0.72102f,		0.873924f)	},
		{ "pGateEntrance.png",		glm::vec2(0.504569f,	0.469884f)	},
		{ "pMidOth.png",			glm::vec2(0.472503f,	0.381594f)	},

		{ "sAPillar.png",			glm::vec2(0.767516f,	0.537224f)	},
		{ "sBulletin.png",			glm::vec2(0.382716f,	0.680883f)	},
		{ "sFrog.png",				glm::vec2(0.432419f,	0.312757f)	},
		{ "sGaragePillar.png",		glm::vec2(0.0460155f,	0.375608f)	},
		{ "sSewer.png",				glm::vec2(0.647266f,	0.327722f)	},
		{ "sTSpawnCubby.png",		glm::vec2(0.517396f,	0.185559f)	}
	},

	// hard
	{
		{ "aDog.png",				glm::vec2(0.756957f,	0.352412f)	},
		{ "aFish.png",				glm::vec2(0.217402f,	0.605177f)	},
		{ "aGate.png",				glm::vec2(0.387532f,	0.503099f)	},
		{ "aLobster.png",			glm::vec2(0.231985f,	0.640203f)	},
		{ "aTSpawn.png",			glm::vec2(0.37781f,		0.0267347f)	},
		{ "aTrash.png",				glm::vec2(0.40948f,		0.495554f)	},

		{ "biBanner.png",			glm::vec2(0.115946f,	0.376414f)	},
		{ "biCacti.png",			glm::vec2(0.464733f,	0.343607f)	},
		{ "biPot.png",				glm::vec2(0.271346f,	0.609514f)	},
		{ "biTables.png",			glm::vec2(0.563153f,	0.46102f)	},
		{ "biTele.png",				glm::vec2(0.905033f,	0.36778f)	},
		{ "biView.png",				glm::vec2(0.59078f,		0.0328067f)	},

		{ "brArchesPlank.png",		glm::vec2(0.276526f,	0.923768f)	},
		{ "brGem.png",				glm::vec2(0.810067f,	0.224467f)	},
		{ "brJailDoor.png",			glm::vec2(0.102132f,	0.35224f)	},
		{ "brLab.png",				glm::vec2(0.778986f,	0.355694f)	},
		{ "brShackWindow.png",		glm::vec2(0.563153f,	0.250367f)	},
		{ "brWitchDoor.png",		glm::vec2(0.354226f,	0.720021f)	},

		{ "fArcadeWall.png",		glm::vec2(0.379509,		0.728769)	},
		{ "fBUnderWall.png",		glm::vec2(0.964727,		0.456416)	},
		{ "fEnergy.png",			glm::vec2(0.509626f,	0.466201f)	},
		{ "fDanger.png",			glm::vec2(0.124579f,	0.366629f)	},
		{ "fPosters.png",			glm::vec2(0.388759f,	0.442027f)	},
		{ "fUnderWall.png",			glm::vec2(0.343866f,	0.526634f)	},

		{ "hBells.png",				glm::vec2(0.342633f,	0.199027f)	},
		{ "hGreenBoxes.png",		glm::vec2(0.506173,		0.615039)	},
		{ "hMonitors.png",			glm::vec2(0.466089f,	0.320239f)	},
		{ "hSpawnCrate.png",		glm::vec2(0.595959f,	0.959222f)	},
		{ "hWallArt.png",			glm::vec2(0.446849f,	0.499813f)	},
		{ "hWallSmile.png",			glm::vec2(0.656886f,	0.511784f)	},

		{ "iBulletin.png",			glm::vec2(0.488536f,	0.70333f)	},
		{ "iCageSnowman.png",		glm::vec2(0.111752f,	0.782641f)	},
		{ "iCrouchSpot.png",		glm::vec2(0.289722f,	0.40853f)	},
		{ "iPoster.png",			glm::vec2(0.219176f,	0.565657f)	},
		{ "iSwagSnowman.png",		glm::vec2(0.0989257f,	0.697344f)	},
		{ "iTwoDoors.png",			glm::vec2(0.385923f,	0.763187f)	},

		{ "pDoor.png",				glm::vec2(0.223986f,	0.483352f)	},
		{ "pNotes.png",				glm::vec2(0.302549f,	0.236438f)	},
		{ "pOnze.png",				glm::vec2(0.408369f,	0.378601f)	},
		{ "pSeaAnimals.png",		glm::vec2(0.578323f,	0.650954f)	},  
		{ "pWallWindow.png",		glm::vec2(0.9038f,		0.69884f)	},
		{ "pWater.png",				glm::vec2(0.595959f,	0.764684f)	},

		{ "sKRoom.png",				glm::vec2(0.953503f,	0.736251f)	},
		{ "sMasks.png",				glm::vec2(0.361873f,	0.309764f)	},
		{ "sMonitors.png",			glm::vec2(0.85089f,		0.861953f)	},
		{ "sNeonStore.png",			glm::vec2(0.494949f,	0.860456f)	},
		{ "sPlug.png",				glm::vec2(0.296136f,	0.466891f)	},
		{ "sScuttle.png",			glm::vec2(0.289722f,	0.363636f)	}
	}
};

void GM::filterMaps()
{
	GM::filteredMaps.clear();

	for (const Map& map : GM::allMaps[GM::config.difficulty - 1])
	{
		switch (map.mapType)
		{
		case MapEnum::ASCENT: // ascent
			if (GM::config.mapChoice[0])
				GM::filteredMaps.push_back(map);
			break;

		case MapEnum::BIND: // bind
			if (GM::config.mapChoice[1])
				GM::filteredMaps.push_back(map);
			break;

		case MapEnum::BREEZE:
			if (GM::config.mapChoice[2])
				GM::filteredMaps.push_back(map);
			break;

		case MapEnum::FRACTURE: // fracture
			if (GM::config.mapChoice[3])
				GM::filteredMaps.push_back(map);
			break;

		case MapEnum::HAVEN: // haven
			if (GM::config.mapChoice[4])
				GM::filteredMaps.push_back(map);
			break;

		case MapEnum::ICEBOX: // icebox
			if (GM::config.mapChoice[5])
				GM::filteredMaps.push_back(map);
			break;

		case MapEnum::PEARL: // pearl
			if (GM::config.mapChoice[6])
				GM::filteredMaps.push_back(map);
			break;

		case MapEnum::SPLIT: // split
			if (GM::config.mapChoice[7])
				GM::filteredMaps.push_back(map);
			break;

		default:
#ifdef _DEBUG
			std::cout << "unknown map format in filterMaps" << map.mapPath << std::endl;
#endif
			break;
		}
	}
	
#ifdef _DEBUG
	std::cout << "filtered maps: " << std::endl;

	for (Map& map : GM::filteredMaps)
		std::cout << map.mapPath << std::endl;

	std::cout << std::endl;
#endif

}

size_t GM::mapIDOf(unsigned difficulty, const char* mapPath)
{
	for (size_t i = 0; i < sizeof(GM::allMaps[0]) / sizeof(GM::allMaps[0][0]); i++)
	{
		if (strlen(mapPath) != strlen(GM::allMaps[difficulty - 1][i].mapPath))
			continue; 

		bool match = true;

		// reverse iteration because the last letter has more weight, more variance, quicker sorting
		for (size_t strInd = strlen(mapPath) - 1; strInd > 0; strInd--)
		{
			if ((char)mapPath[strInd] != (char)GM::allMaps[difficulty - 1][i].mapPath[strInd])
			{
				match = false;

				break;
			}
		}

		if (match)
			return i;
		else
			continue;
	}

#ifdef _DEBUG
	std::cout << "could not find map ID of " << mapPath << " with difficulty " << difficulty << std::endl;
#endif

	return 0;
}

void GM::newGame()
{
	GM::usedMaps.clear();
	GM::filterMaps();

	if (GM::filteredMaps.size() < 5)
	{
#ifdef _DEBUG
		std::cout << "there are not 5 valid filtered maps. not good." << std::endl;
#endif

		return Yui::loadScene(100);
	}

	std::vector<Map> reservedMaps;
	reservedMaps.reserve(5);

	for (uint8_t i = 0; i < 5; i++)
	{
		unsigned mapIndex;
		do
		{
			mapIndex = rand() % GM::filteredMaps.size();
		} while (std::count(GM::usedMaps.begin(), GM::usedMaps.end(), mapIndex));

		GM::usedMaps.push_back(mapIndex);
		reservedMaps.push_back(GM::filteredMaps[mapIndex]);
	}

#ifdef _DEBUG
	std::cout << "reserved maps for current game: " << std::endl;

	for (int i = 0; i < reservedMaps.size(); i++)
		std::cout << reservedMaps[i].mapPath << std::endl;

	std::cout << std::endl;
#endif

	GM::gameSlot = new Game(reservedMaps, GM::config.difficulty, GameType::STANDARD);
}

void GM::newFreeplay()
{
	GM::usedMaps.clear();
	GM::filterMaps();

	if (GM::filteredMaps.size() < 1)
	{
#ifdef _DEBUG
		std::cout << "there are not any filtered maps for freeplay. not good." << std::endl;
#endif

		return Yui::loadScene(100);
	}

	std::vector<Map> randomizedMaps;
	randomizedMaps.reserve(GM::usedMaps.size());

	for (uint8_t i = 0; i < GM::filteredMaps.size(); i++)
	{
		unsigned mapIndex;
		do
		{
			mapIndex = rand() % GM::filteredMaps.size();
		} while (std::count(GM::usedMaps.begin(), GM::usedMaps.end(), mapIndex));

		GM::usedMaps.push_back(mapIndex);
		randomizedMaps.push_back(GM::filteredMaps[mapIndex]);
	}

#ifdef _DEBUG
	std::cout << "reserved maps for current game: " << std::endl;

	for (int i = 0; i < randomizedMaps.size(); i++)
		std::cout << randomizedMaps[i].mapPath << std::endl;

	std::cout << std::endl;
#endif

	GM::gameSlot = new Game(randomizedMaps, GM::config.difficulty, GameType::FREEPLAY);
}

void GM::newImport(std::string importString)
{
	uint32_t points = 0;
	unsigned difficulty = 0;
	std::vector<Map> maps;
	maps.reserve(5);

	size_t prev = 0;
	for (size_t curr = 0; curr < importString.size(); curr++)
	{
		if (importString[curr] == ';')
		{
			if (!prev) // first slice: difficulty
				difficulty = std::stoul(importString.substr(prev, curr - prev), nullptr, 16);
			else // middle slices: map IDs
				maps.push_back({ GM::allMaps[difficulty - 1][std::stoul(importString.substr(prev, curr - prev), nullptr, 16)] });

			prev = curr + 1;
		}
		else if (curr == importString.size() - 1) // last slice: points
			points = std::stoul(importString.substr(prev, importString.size()), nullptr, 16);
	}

	GM::importedPoints = points;
	GM::config.difficulty = difficulty;
	GM::gameSlot = new Game(maps, difficulty, GameType::IMPORT);
}

bool GM::validateImportString()
{
	std::string clipboardText = Yui::clipboardText();

	if (clipboardText.size() > 21)
	{
#ifdef _DEBUG
		std::cout << "invalid format: string too long" << std::endl;
#endif
		
		return false;
	}

	std::regex gameFMTrgx("[1-3];(([0-9a-f]){1,2};){5}([0-9a-f]){1,4}", std::regex_constants::ECMAScript);

	if (!std::regex_match(clipboardText, gameFMTrgx))
	{
#ifdef _DEBUG
		std::cout << "invalid format: does not match regex" << std::endl;
#endif

		return false;
	}

	return true;
}

void GM::stopGame(bool save)
{
	if (save)
		GM::config.games.push_back(*GM::gameSlot);

	delete GM::gameSlot;
	GM::gameSlot = nullptr;
}

void GM::nextRound()
{
	GM::gameSlot->nextRound(glm::vec2(Yui::markerRelX, Yui::markerRelY));
}

void GM::calcStats()
{
	size_t numOfStandards = GM::config.games.size();
	for (size_t i = 0; i < GM::config.games.size(); i++)
		if (GM::config.games[i].gameType == GameType::IMPORT)
			numOfStandards--;

	if (!numOfStandards)
		return;

	uint32_t low = GM::config.games[0].sumOfRounds();
	uint32_t max = low, total = low;

	uint8_t Ws = 0, Ls = 0;

	if (low >= 17500)
		Ws++;
	else
		Ls++;

	if (GM::config.games.size() > 1)
	{
		for (size_t i = 1; i < GM::config.games.size(); i++)
		{
			if (GM::config.games[i].gameType == GameType::IMPORT)
				continue;

			total += GM::config.games[i].sumOfRounds();

			if (GM::config.games[i].sumOfRounds() > max)
				max = GM::config.games[i].sumOfRounds();
			else if (GM::config.games[i].sumOfRounds() < low)
				low = GM::config.games[i].sumOfRounds();

			if (GM::config.games[i].sumOfRounds() >= 20000)
				Ws++;
			else
				Ls++;
		}
	}

	GM::config.playCount = (uint16_t)numOfStandards;

	GM::config.pointRecord = max;
	GM::config.lowestPoints = low;
	GM::config.totalPoints = total;

	GM::config.Ws = Ws;
	GM::config.Ls = Ls;
}

void GM::calcAchievements()
{
	if (GM::config.playCount >= 5)
		GM::config.achievements[1] = 0b1;
	else
		GM::config.achievements[1] = 0b0;

	if (GM::config.playCount >= 10)
		GM::config.achievements[2] = 0b1;
	else
		GM::config.achievements[2] = 0b0;

	if (GM::config.playCount >= 15)
		GM::config.achievements[3] = 0b1;
	else
		GM::config.achievements[3] = 0b0;

	if (GM::config.pointRecord > 10000)
		GM::config.achievements[4] = 0b1;
	else
		GM::config.achievements[4] = 0b0;

	if (GM::config.pointRecord > 15000)
		GM::config.achievements[5] = 0b1;
	else
		GM::config.achievements[5] = 0b0;

	if (GM::config.pointRecord > 20000)
		GM::config.achievements[6] = 0b1;
	else
		GM::config.achievements[6] = 0b0;

	if (GM::config.pointRecord == 25000)
		GM::config.achievements[7] = 0b1;
	else
		GM::config.achievements[7] = 0b0;
}

void GM::init()
{
	GM::filteredMaps.reserve(48);
	GM::usedMaps.reserve(15);
}

void GM::cleanup()
{
	if (GM::gameSlot)
		delete GM::gameSlot;
}