#include "UIManager.h"

// reflplan:
// minimap loading takes a while, could have used move semantics, etc
// could remake the export string to be able to hold coords for each guess

/* TODO
* LONG TERM:
* rest of maps																					Y
* resize all images D:																			Y
* make tutorial																					Y
* make stats screen more consise. get real feedback to make it look better (swap colors?)		Y
* gonna need to handle those fatal errors														Y
* 
* COMPLETE:
* add sound to everything. get new sounds too													Y
* resize tutorial images, too much storage														Y
* review every scene, optimize if possible, clean up											Y
* revise which buttons should have hover stuff yk												Y
* remake import screen																			Y
* map char array limit to highest size filename after i get all the maps						Y
* make all file navigation uniform																Y
* set all buttons that dont change the scene's final param to false								Y
* somehow work the exported game thing															Y
* make all the text consistent with the A flip thing											Y
* fix memory leak with marker (removeElement)													Y
* scale down markers, make them look better														Y
*/

std::vector<UIElement*> Yui::UIElements;

irrklang::ISoundEngine* Yui::soundEngine;
irrklang::ISoundSource* Yui::menuBGM;

unsigned Yui::scene = 0, Yui::historyPage = 1, Yui::exportCount = 0, Yui::tutorialPage = 0;

bool Yui::toClose = false, Yui::playable = true, Yui::minimapScaled = false;

Image* Yui::marker = nullptr;
float Yui::markerRelX, Yui::markerRelY;

void Yui::addElement(UIElement* element)
{
	Yui::UIElements.push_back(element);
}

void Yui::addElements(unsigned count, ...)
{
	va_list args;
	va_start(args, count);

	for (unsigned i = 0; i < count; i++)
		Yui::UIElements.push_back(va_arg(args, UIElement*));

	va_end(args);
}

void Yui::removeElement(UIElement* element)
{
	size_t index;
	bool contains = false;

	for (index = 0; index < Yui::UIElements.size(); index++)
	{
		if (Yui::UIElements[index] == element)
		{
			contains = true;
			break;
		}
	}

	if (contains)
	{
		delete Yui::UIElements[index];
		Yui::UIElements.erase(Yui::UIElements.begin() + index);
	}
	else
	{
#ifdef _DEBUG
		std::cout << "attempting to remove element that is not in UIElements" << std::endl;
#endif
	}
}

void Yui::purgeElements()
{
	for (size_t i = 0; i < Yui::UIElements.size(); i++)
		delete Yui::UIElements[i];

	Yui::UIElements.clear();

	Yui::marker = nullptr;
}

void Yui::updateAll()
{
	for (UIElement* element : Yui::UIElements)
		element->update();
}

void Yui::renderAll()
{
	for (UIElement* element : Yui::UIElements)
		element->render();
}

void Yui::init(const char* fontName, float* ratioW, float* ratioH, unsigned pixelHeight)
{
	UIElement::projection = glm::ortho(0.0f, (float)*UIElement::WINDOWWIDTH, 0.0f, (float)*UIElement::WINDOWHEIGHT);
	ScrollText::scrollSpeed = 10.0f;

	::srand((unsigned)::time(0));

	GM::init();
	Text::init(fontName, ratioW, ratioH, pixelHeight);
	Image::init();
	Button::init();
	CheckBox::init();

	Yui::soundEngine = irrklang::createIrrKlangDevice();
	Yui::soundEngine->setSoundVolume(1.0f);

	Yui::menuBGM = Yui::soundEngine->addSoundSourceFromFile(PATH_PREFIX.append("audio\\valomenu.mp3").c_str());

	Yui::UIElements.reserve(25);

	if (std::filesystem::exists(PATH_PREFIX.append("config\\config.valog").c_str()))
		Yui::loadScene(0);
	else
		Yui::loadScene(10);
}

void Yui::cleanup()
{
	Yui::purgeElements();

	Yui::soundEngine->stopAllSounds();
	Yui::soundEngine->drop();

	Text::cleanup();
	CheckBox::cleanup();

	Image::cleanup();
	Button::cleanup();
}

/*
		BEGINNING OF YUI::LOADSCENE
*/

void Yui::loadScene(unsigned sceneID)
{
	Yui::scene = sceneID;
	Yui::purgeElements();

	Yui::playable = true;

	switch (sceneID)
	{
	case 0: // main menu
	{
		if (!Yui::soundEngine->isCurrentlyPlaying(Yui::menuBGM))
			Yui::soundEngine->play2D(Yui::menuBGM, true);

		Yui::addElements
		(
			7,
			new Button
			(
				0.5f,
				0.8735f,
				1.0f,
				0.003f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				[]() {},
				[]() {},
				[]() {},
				false
			),
			new Button
			(
				new Text("AchieveMents", 0.12f, 0.9375f, 0.3f, 0.3f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
				0.1f,
				0.1f,
				1.25f,
				1.25f,
				glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
				glm::vec4(1.0f, 0.984f, 0.961f, 0.04f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclick.mp3").c_str());
					Yui::loadScene(1);
				}
			),
			new Button
			(
				new Text("StAts", 0.3f, 0.9375f, 0.3f, 0.3f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
				0.2f,
				0.2f,
				1.25f,
				1.25f,
				glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
				glm::vec4(1.0f, 0.984f, 0.961f, 0.04f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclick.mp3").c_str());
					Yui::loadScene(2);
				}
			),
			new ValoButton
			(
				new Text("PlAy", 0.5f, 0.9375f, 0.75f, 0.75f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
				0.1f,
				0.1f,
				0.65f,
				0.65f,
				0.5f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoplayclick.mp3").c_str());
					Yui::loadScene(5);
				}
			),
			new Button
			(
				new Text("Settings", 0.715f, 0.9375f, 0.3f, 0.3f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
				0.2f,
				0.2f,
				1.25f,
				1.25f,
				glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
				glm::vec4(1.0f, 0.984f, 0.961f, 0.04f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclick.mp3").c_str());
					Yui::loadScene(3);
				}
			),
			new Button
			(
				new Text("History", 0.885f, 0.9375f, 0.3f, 0.3f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
				0.2f,
				0.2f,
				1.25f,
				1.25f,
				glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
				glm::vec4(1.0f, 0.984f, 0.961f, 0.04f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclick.mp3").c_str());

					Yui::historyPage = 1;
					Yui::loadScene(4);
				}
			),
			new ScrollText
			(
				std::string("vAloguessr"),
				0.5f,
				0.4415f, 
				1.0f,
				1.0f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				{ {2.25f, "vAloguessr"} }
			)
		);

		((ScrollText*)Yui::UIElements[6])->inf = true;

		break;
	}

	case 1: // achievements
	{
		Yui::addElements
		(
			19,
			new Button
			(
				0.5f,
				0.8735f,
				1.0f,
				0.003f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				[]() {},
				[]() {},
				[]() {},
				false
			),
			new Button
			(
				new Text("BAck", 0.0675f, 0.9375f, 0.3f, 0.3f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
				0.2f,
				0.2f,
				1.25f,
				1.25f,
				glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
				glm::vec4(1.0f, 0.984f, 0.961f, 0.04f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclick.mp3").c_str());
					Yui::loadScene(0);
				}
			),
			new ValoButton
			(
				new Text("AchieveMents", 0.5f, 0.9375f, 0.75f, 0.75f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
				0.01f,
				0.01f,
				0.65f,
				0.65f,
				0.2f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
				[]() {},
				[]() {}
			),

			new CheckBox(	0.075f,		0.7375f,	0.1f * windowRatioHW,	0.1f),
			new Text("",	0.135f,		0.7375f,	0.325f,					0.35f, glm::vec4(0.741f, 0.224f, 0.267f, 1.0f), true),

			new CheckBox(	0.075f,		0.525f,		0.1f * windowRatioHW,	0.1f),
			new Text("",	0.135f,		0.525f,		0.325f,					0.35f, glm::vec4(0.741f, 0.224f, 0.267f, 1.0f), true),

			new CheckBox(	0.075f,		0.3125f,	0.1f * windowRatioHW,	0.1f),
			new Text("",	0.135f,		0.3125f,	0.325f,					0.35f, glm::vec4(0.741f, 0.224f, 0.267f, 1.0f), true),

			new CheckBox(	0.075f,		0.1f,		0.1f * windowRatioHW,	0.1f),
			new Text("",	0.135f,		0.1f,		0.325f,					0.35f, glm::vec4(0.741f, 0.224f, 0.267f, 1.0f), true),

			new CheckBox(	0.575f,		0.7375f,	0.1f * windowRatioHW,	0.1f),
			new Text("",	0.635f,		0.7375f,	0.325f,					0.35f, glm::vec4(0.741f, 0.224f, 0.267f, 1.0f), true),

			new CheckBox(	0.575f,		0.525f,		0.1f * windowRatioHW,	0.1f),
			new Text("",	0.635f,		0.525f,		0.325f,					0.35f, glm::vec4(0.741f, 0.224f, 0.267f, 1.0f), true),

			new CheckBox(	0.575f,		0.3125f,	0.1f * windowRatioHW,	0.1f),
			new Text("",	0.635f,		0.3125f,	0.325f,					0.35f, glm::vec4(0.741f, 0.224f, 0.267f, 1.0f), true),

			new CheckBox(	0.575f,		0.1f,		0.1f * windowRatioHW,	0.1f),
			new Text("",	0.635f,		0.1f,		0.325f,					0.35f, glm::vec4(0.741f, 0.224f, 0.267f, 1.0f), true)
		);

		ui2t(4)->changeText("PlAy FreeplAy");
		ui2t(6)->changeText("PlAy StAndArd 5 times");
		ui2t(8)->changeText("PlAy StAndArd 10 times");
		ui2t(10)->changeText("PlAy StAndArd 15 times");
		ui2t(12)->changeText("Score 10k+ Points");
		ui2t(14)->changeText("Score 15k+ Points");
		ui2t(16)->changeText("Score 20k+ Points");
		ui2t(18)->changeText("Get A Perfect Score");

		Yui::setAchievements();

		break;
	}

	case 2: // stats
	{
		Yui::addElements
		(
			16,
			new Button
			(
				0.5f,
				0.8735f,
				1.0f,
				0.003f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				[]() {},
				[]() {},
				[]() {},
				false
			),
			new Button
			(
				new Text("BAck", 0.0675f, 0.9375f, 0.3f, 0.3f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
				0.2f,
				0.2f,
				1.25f,
				1.25f,
				glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
				glm::vec4(1.0f, 0.984f, 0.961f, 0.04f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclick.mp3").c_str());
					Yui::loadScene(0);
				}
			),
			new ValoButton
			(
				new Text("StAts", 0.5f, 0.9375f, 0.75f, 0.75f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
				0.01f,
				0.01f,
				0.65f,
				0.65f,
				0.5f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
				[]() {},
				[]() {}
			),

			new Text("",		0.035f,		0.725f,		0.4f,		0.4f,		glm::vec4(1.0f, 0.984f, 0.961f, 1.0f),   true),
			new Text("",		0.335f,		0.725f,		0.4f,		0.4f,		glm::vec4(0.741f, 0.224f, 0.267f, 1.0f), true),

			new Text("",		0.545f,		0.725f,		0.4f,		0.4f,		glm::vec4(1.0f, 0.984f, 0.961f, 1.0f),   true),
			new Text("",		0.86f,		0.725f,		0.4f,		0.4f,		glm::vec4(0.741f, 0.224f, 0.267f, 1.0f), true),

			new Text("",		0.035f,		0.5f,		0.4f,		0.4f,		glm::vec4(1.0f, 0.984f, 0.961f, 1.0f),   true),
			new Text("",		0.335f,		0.5f,		0.4f,		0.4f,		glm::vec4(0.741f, 0.224f, 0.267f, 1.0f), true),

			new Text("",		0.545f,		0.5f,		0.4f,		0.4f,		glm::vec4(1.0f, 0.984f, 0.961f, 1.0f),   true),
			new Text("",		0.86f,		0.5f,		0.4f,		0.4f,		glm::vec4(0.741f, 0.224f, 0.267f, 1.0f), true),

			new Text("",		0.035f,		0.3f,		0.4f,		0.4f,		glm::vec4(1.0f, 0.984f, 0.961f, 1.0f),   true),
			new Text("",		0.335f,		0.3f,		0.4f,		0.4f,		glm::vec4(0.741f, 0.224f, 0.267f, 1.0f), true),

			new Text("",		0.545f,		0.3f,		0.4f,		0.4f,		glm::vec4(1.0f, 0.984f, 0.961f, 1.0f),   true),
			new Text("",		0.86f,		0.3f,		0.4f,		0.4f,		glm::vec4(0.741f, 0.224f, 0.267f, 1.0f), true),

			new Button
			(
				new Text("Reset All gAMe dAtA", 0.5f, 0.1f, 0.5f, 0.5f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
				0.1f,
				0.1f,
				1.25f,
				1.25f,
				0.225f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclicky.mp3").c_str());

					GM::config.resetSHA();

					Yui::setStats();
				},
				[]() {},
				false
			)
		);

		ui2t(3)->changeText("GAMes plAyed:");
		ui2t(5)->changeText("AverAge points:");
		ui2t(7)->changeText("Highest points:");
		ui2t(9)->changeText("Lowest points:");
		ui2t(11)->changeText("high scorers:");
		ui2t(13)->changeText("low scorers:");

		Yui::setStats();
		
		break;
	}

	case 3: // settings
	{
		Yui::addElements
		(
			19,
			new Button
			(
				0.5f,
				0.8735f,
				1.0f,
				0.003f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				[]() {},
				[]() {},
				[]() {},
				false
			),
			new Button
			(
				new Text("BAck", 0.0675f, 0.9375f, 0.3f, 0.3f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
				0.2f,
				0.2f,
				1.25f,
				1.25f,
				glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
				glm::vec4(1.0f, 0.984f, 0.961f, 0.04f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclick.mp3").c_str());
					Yui::loadScene(0);
				}
			),
			new ValoButton
			(
				new Text("Settings", 0.5f, 0.9375f, 0.75f, 0.75f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
				0.01f,
				0.01f,
				0.65f,
				0.65f,
				0.3f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
				[]() {},
				[]() {}
			),
			new Button
			(
				0.035f,
				0.825f,
				0.05f * .525f,
				0.05f,
				glm::vec4(0.941f, 0.351f, 0.402f, 1.0f),
				glm::vec4(0.941f, 0.351f, 0.402f, 1.0f),
				[]() {},
				[]() {},
				[]() {},
				false
			),
			new Text("Active", 0.095f, 0.825f, 0.25f, 0.25f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
			new Text("Difficulty:", 0.5f, 0.775f, 0.5f, 0.5f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
			new Button
			(
				new Text("EAsy", 0.175f, 0.65f, 0.35f, 0.35f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
				1.0f,
				1.0f,
				1.5f,
				1.5f,
				0.5f,
				glm::vec4(0.533f, 0.533f, 0.533f, 1.0f),
				glm::vec4(0.941f, 0.351f, 0.402f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[]() {},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());

					GM::config.difficulty = 1;
					Yui::setSettingsButtons();
				},
				[]()
				{
					Yui::setSettingsButtons();
				}
			),
			new Button
			(
				new Text("MediuM", 0.5f, 0.65f, 0.35f, 0.35f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
				1.0f,
				1.0f,
				1.5f,
				1.5f,
				0.5f,
				glm::vec4(0.941f, 0.351f, 0.402f, 1.0f),
				glm::vec4(0.533f, 0.533f, 0.533f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[](){},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());

					GM::config.difficulty = 2;
					Yui::setSettingsButtons();
				},
				[]()
				{
					Yui::setSettingsButtons();
				}
			),
			new Button
			(
				new Text("HArd", 0.825f, 0.65f, 0.35f, 0.35f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
				1.0f,
				1.0f,
				1.5f,
				1.5f,
				0.5f,
				glm::vec4(0.533f, 0.533f, 0.533f, 1.0f),
				glm::vec4(0.941f, 0.351f, 0.402f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[](){},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());

					GM::config.difficulty = 3;
					Yui::setSettingsButtons();
				},
				[]()
				{
					Yui::setSettingsButtons();
				}
			),
			new Text("MAps Allowed:", 0.5f, 0.475f, 0.5f, 0.5f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
			new Button
			(
				new Text("Ascent", 0.14375f, 0.35f, 0.35f, 0.35f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
				0.75f,
				0.75f,
				1.5f,
				1.5f,
				0.5f,
				glm::vec4(0.941f, 0.351f, 0.402f, 1.0f),
				glm::vec4(0.533f, 0.533f, 0.533f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[](){},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());

					if (GM::config.mapChoice[0])
					{
						if ((unsigned)GM::config.mapChoice.to_ulong() > 1)
							GM::config.mapChoice[0] = !GM::config.mapChoice[0];
					}
					else
						GM::config.mapChoice[0] = !GM::config.mapChoice[0];

					Yui::setSettingsButtons();
				},
				[]()
				{
					Yui::setSettingsButtons();
				}
			),
			new Button
			(
				new Text("Bind", 0.38125f, 0.35f, 0.35f, 0.35f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
				1.75f,
				1.75f,
				1.5f,
				1.5f,
				0.5f,
				glm::vec4(0.941f, 0.351f, 0.402f, 1.0f),
				glm::vec4(0.533f, 0.533f, 0.533f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[]() {},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());

					if (GM::config.mapChoice[1])
					{
						if ((unsigned)GM::config.mapChoice.to_ulong() > 2)
							GM::config.mapChoice[1] = !GM::config.mapChoice[1];
					}
					else
						GM::config.mapChoice[1] = !GM::config.mapChoice[1];

					Yui::setSettingsButtons();
				},
				[]()
				{
					Yui::setSettingsButtons();
				}
			),
			new Button
			(
				new Text("Breeze", 0.61875f, 0.35f, 0.35f, 0.35f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
				0.825f,
				0.825f,
				1.5f,
				1.5f, 
				0.5f,
				glm::vec4(0.941f, 0.351f, 0.402f, 1.0f),
				glm::vec4(0.533f, 0.533f, 0.533f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[]() {},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());

					if (GM::config.mapChoice[2])
					{
						if ((unsigned)GM::config.mapChoice.to_ulong() > 4)
							GM::config.mapChoice[2] = !GM::config.mapChoice[2];
					}
					else
						GM::config.mapChoice[2] = !GM::config.mapChoice[2];

					Yui::setSettingsButtons();
				},
				[]()
				{
					Yui::setSettingsButtons();
				}
			),
			new Button
			(
				new Text("FrActure", 0.85625f, 0.35f, 0.35f, 0.35f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
				0.25f,
				0.25f,
				1.5f,
				1.5f,
				0.5f,
				glm::vec4(0.941f, 0.351f, 0.402f, 1.0f),
				glm::vec4(0.533f, 0.533f, 0.533f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[]() {},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());

					if (GM::config.mapChoice[3])
					{
						if ((unsigned)GM::config.mapChoice.to_ulong() > 8)
							GM::config.mapChoice[3] = !GM::config.mapChoice[3];
					}
					else
						GM::config.mapChoice[3] = !GM::config.mapChoice[3];

					Yui::setSettingsButtons();
				},
				[]()
				{
					Yui::setSettingsButtons();
				}
			),
			new Button
			(
				new Text("HAven", 0.14375f, 0.225f, 0.35f, 0.35f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
				1.05f,
				1.05f,
				1.5f,
				1.5f,
				0.5f,
				glm::vec4(0.941f, 0.351f, 0.402f, 1.0f),
				glm::vec4(0.533f, 0.533f, 0.533f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[]() {},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());

					if (GM::config.mapChoice[4])
					{
						if ((unsigned)GM::config.mapChoice.to_ulong() > 16)
							GM::config.mapChoice[4] = !GM::config.mapChoice[4];
					}
					else
						GM::config.mapChoice[4] = !GM::config.mapChoice[4];

					Yui::setSettingsButtons();
				},
				[]()
				{
					Yui::setSettingsButtons();
				}
			),
			new Button
			(
				new Text("Icebox", 0.38125f, 0.225f, 0.35f, 0.35f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
				0.75f,
				0.75f,
				1.5f,
				1.5f,
				0.5f,
				glm::vec4(0.941f, 0.351f, 0.402f, 1.0f),
				glm::vec4(0.533f, 0.533f, 0.533f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[]() {},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());

					if (GM::config.mapChoice[5])
					{
						if ((unsigned)GM::config.mapChoice.to_ulong() > 32)
							GM::config.mapChoice[5] = !GM::config.mapChoice[5];
					}
					else
						GM::config.mapChoice[5] = !GM::config.mapChoice[5];

					Yui::setSettingsButtons();
				},
				[]()
				{
					Yui::setSettingsButtons();
				}
			),
			new Button
			(
				new Text("PeArl", 0.61875f, 0.225f, 0.35f, 0.35f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
				1.15f,
				1.15f,
				1.5f,
				1.5f,
				0.5f,
				glm::vec4(0.941f, 0.351f, 0.402f, 1.0f),
				glm::vec4(0.533f, 0.533f, 0.533f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[]() {},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());

					if (GM::config.mapChoice[6])
					{
						if ((unsigned)GM::config.mapChoice.to_ulong() > 64)
							GM::config.mapChoice[6] = !GM::config.mapChoice[6];
					}
					else
						GM::config.mapChoice[6] = !GM::config.mapChoice[6];

					Yui::setSettingsButtons();
				},
				[]()
				{
					Yui::setSettingsButtons();
				}
			),
			new Button
			(
				new Text("Split", 0.85625f, 0.225f, 0.35f, 0.35f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
				1.5f,
				1.5f,
				1.5f,
				1.5f,
				0.5f,
				glm::vec4(0.941f, 0.351f, 0.402f, 1.0f),
				glm::vec4(0.533f, 0.533f, 0.533f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[]() {},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());

					if (GM::config.mapChoice[7])
					{
						if ((unsigned)GM::config.mapChoice.to_ulong() > 128)
							GM::config.mapChoice[7] = !GM::config.mapChoice[7];
					}
					else
						GM::config.mapChoice[7] = !GM::config.mapChoice[7];

					Yui::setSettingsButtons();
				},
				[]()
				{
					Yui::setSettingsButtons();
				}
			),
			new Button
			(
				new Text("Reset to DefAult", 0.5f, 0.075f, 0.4f, 0.4f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
				0.25f,
				0.25f,
				1.25f,
				1.25f,
				0.4f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[]() {},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclicky.mp3").c_str());

					GM::config.resetSettings();

					Yui::setSettingsButtons();
				}
			)
		);

		Yui::setSettingsButtons();

		break;
	}

	case 4: // history
	{
		Yui::setHistory();

		break;
	}

	case 5: // play selection screen
	{
		Yui::addElements
		(
			11,
			new Button
			(
				0.5f,
				0.8735f,
				1.0f,
				0.003f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				[]() {},
				[]() {},
				[]() {},
				false
			),
			new Button
			(
				new Text("BAck", 0.0675f, 0.9375f, 0.3f, 0.3f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
				0.2f,
				0.2f,
				1.25f,
				1.25f,
				glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
				glm::vec4(1.0f, 0.984f, 0.961f, 0.04f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclick.mp3").c_str());
					Yui::loadScene(0);
				}
			),
			new ValoButton
			(
				new Text("plAy", 0.5f, 0.9375f, 0.75f, 0.75f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
				0.1f,
				0.1f,
				0.65f,
				0.65f,
				0.5f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
				[]() {},
				[]() {}
			),
			new Button
			(
				new Image(PATH_PREFIX.append("images\\splashes\\ascentSplash.png").c_str(), false, 0.25f, 0.654f, 0.5f, 0.436f, 0.325f),
				glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
				[]() {},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoplayclick.mp3").c_str());

					GM::newGame();

					if (GM::filteredMaps.size() >= 5)
						Yui::loadScene(6);
					else
					{
						GM::stopGame(false);
						Yui::loadScene(100);
					}
				}
			),
			new Text("STANDARD", 0.25f, 0.654f, 0.8f, 0.8f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
			new Button
			(
				new Image(PATH_PREFIX.append("images\\splashes\\havenSplash.png").c_str(), false, 0.75f, 0.654f, 0.5f, 0.436f, 0.325f),
				glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
				[]() {},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoplayclick.mp3").c_str());
					GM::config.achievements[0] = 0b1;

					GM::newFreeplay();

					if (GM::filteredMaps.size() >= 5)
						Yui::loadScene(6);
					else
					{
						GM::stopGame(false);
						Yui::loadScene(100);
					}
				}
			),
			new Text("FREEPLAY", 0.75f, 0.654f, 0.8f, 0.8f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
			new Button
			(
				new Image(PATH_PREFIX.append("images\\splashes\\breezeSplash.png").c_str(), false, 0.25f, 0.218f, 0.5f, 0.436f, 0.325f),
				glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
				[]() {},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclick.mp3").c_str());
					Yui::loadScene(10);
				}
			),
			new Text("TUTORIAL", 0.25f, 0.218f, 0.8f, 0.8f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
			new Button
			(
				new Image(PATH_PREFIX.append("images\\splashes\\pearlSplash.png").c_str(), false, 0.75f, 0.218f, 0.5f, 0.436f, 0.325f),
				glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
				[]() {},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclick.mp3").c_str());

					Yui::tutorialPage = 0;
					Yui::loadScene(11);
				}
			),
			new Text("IMPORT", 0.75f, 0.218f, 0.8f, 0.8f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f))
	);

		break;
	}

	case 6: // play scene
	{
		std::string diffPath = PATH_PREFIX.append("images\\maps\\").c_str();
		if (GM::config.difficulty == 1)
			diffPath += "easy\\";
		else if (GM::config.difficulty == 2)
			diffPath += "medium\\";
		else
			diffPath += "hard\\";

		std::string minimapPath = PATH_PREFIX.append("images\\minimaps\\").c_str();
		switch (GM::gameSlot->getCurrentRound().map.mapType)
		{
			case MapEnum::ASCENT:
				minimapPath += "ascentMap.png";
				break;

			case MapEnum::BIND:
				minimapPath += "bindMap.png";
				break;

			case MapEnum::BREEZE:
				minimapPath += "breezeMap.png";
				break;

			case MapEnum::FRACTURE:
				minimapPath += "fractureMap.png";
				break;

			case MapEnum::HAVEN:
				minimapPath += "havenMap.png";
				break;

			case MapEnum::ICEBOX:
				minimapPath += "iceboxMap.png";
				break;

			case MapEnum::PEARL:
				minimapPath += "pearlMap.png";
				break;

			case MapEnum::SPLIT:
				minimapPath += "splitMap.png";
				break;
			default:
#ifdef _DEBUG
				std::cout << "current round's map type unknown in play scene" << std::endl;
#endif
				break; 
		}

		Yui::addElements
		(
			4,
			new Image(diffPath + std::string(GM::gameSlot->getCurrentRound().map.mapPath), false, 0.5f, 0.5f, 1.0, 1.0f),
			new Button
			(
				new Text("PlAy", 0.5f, 0.9625f, 0.3f, 0.3f, glm::vec4(1.0f, 0.984f, 0.961f, 0.65f)),
				20.0f,
				20.0f,
				1.5f,
				1.5f,
				glm::vec4(0.741f, 0.224f, 0.267f, 0.35f),
				glm::vec4(0.741f, 0.224f, 0.267f, 0.35f),
				[]() {},
				[]() {},
				[]() {},
				false
			),
			new Button
			(
				new Image(minimapPath, true, 1.0f - (unscaledMinimapWidth / 2.0f), unscaledMinimapHeight / 2.0f, unscaledMinimapWidth, unscaledMinimapHeight),
				glm::vec4(1.0f, 0.984f, 0.961f, 0.1f),
				[]()
				{
					if (!Yui::minimapScaled)
					{
						ui2b(2)->centerX = 1.0f - (scaledMinimapWidth / 2.0f);
						ui2b(2)->centerY = scaledMinimapHeight / 2.0f;
						ui2b(2)->width = scaledMinimapWidth;
						ui2b(2)->height = scaledMinimapHeight;

						ui2b(2)->image->centerX = ui2b(2)->centerX;
						ui2b(2)->image->centerY = ui2b(2)->centerY;
						ui2b(2)->image->width = ui2b(2)->width;
						ui2b(2)->image->height = ui2b(2)->height;

						Yui::minimapScaled = true;

						ui2b(2)->update();
						Yui::updateMarker();
					}
				},
				[]()
				{
					Yui::setMarker();
					ui2b(3)->visible = true;
				},
				[]()
				{
					if (Yui::minimapScaled)
					{
						ui2b(2)->centerX = 1.0f - (unscaledMinimapWidth / 2.0f);
						ui2b(2)->centerY = unscaledMinimapHeight / 2.0f;
						ui2b(2)->width = unscaledMinimapWidth;
						ui2b(2)->height = unscaledMinimapHeight;

						ui2b(2)->image->centerX = ui2b(2)->centerX;
						ui2b(2)->image->centerY = ui2b(2)->centerY;
						ui2b(2)->image->width = ui2b(2)->width;
						ui2b(2)->image->height = ui2b(2)->height;

						Yui::minimapScaled = false;
						
						ui2b(2)->update();
						Yui::updateMarker();
					}
				},
				false
			),
			new Button
			(
				new Text("Guess", 0.5f, 0.1f, 0.4f, 0.4f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
				0.65f,
				0.65f,
				1.0f,
				1.0f,
				0.2125f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclick.mp3").c_str());

					GM::gameSlot->nextRound(glm::vec2(Yui::markerRelX, Yui::markerRelY));

					if (GM::gameSlot->gameType == GameType::STANDARD || GM::gameSlot->gameType == GameType::IMPORT)
						Yui::loadScene(7);
					else if (GM::gameSlot->gameType == GameType::FREEPLAY)
						Yui::loadScene(9);
				}
			)
		);

		ui2b(3)->visible = false;

		break;
	}

	case 7: // round result scene
	{
		std::string minimapPath = PATH_PREFIX.append("images\\minimaps\\").c_str();
		switch (GM::gameSlot->rounds[GM::gameSlot->roundIndex - 1].map.mapType)
		{
		case MapEnum::ASCENT:
			minimapPath += "ascentMap.png";
			break;

		case MapEnum::BIND:
			minimapPath += "bindMap.png";
			break;

		case MapEnum::BREEZE:
			minimapPath += "breezeMap.png";
			break;

		case MapEnum::FRACTURE:
			minimapPath += "fractureMap.png";
			break;

		case MapEnum::HAVEN:
			minimapPath += "havenMap.png";
			break;

		case MapEnum::ICEBOX:
			minimapPath += "iceboxMap.png";
			break;

		case MapEnum::PEARL:
			minimapPath += "pearlMap.png";
			break;

		case MapEnum::SPLIT:
			minimapPath += "splitMap.png";
			break;
		default:
#ifdef _DEBUG
			std::cout << "current round's map type unknown in results scene" << std::endl;
#endif
			break;
		}

		Yui::addElements
		(
			9,
			new Button
			(
				new Text("Round Results", 0.5f, 0.9625f, 0.3f, 0.3f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
				20.0f,
				20.0f,
				1.5f,
				1.5f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				[]() {},
				[]() {},
				[]() {},
				false
			),
			new Text
			(
				"You scored", 
				0.75f,
				0.65f,
				0.5f,
				0.5f,
				glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)
			),
			new Text
			(
				std::to_string(GM::gameSlot->rounds[GM::gameSlot->roundIndex - 1].points) + " / 5000", 
				0.75f,
				0.5f,
				0.5f,
				0.5f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f)
			),
			new Text
			(
				"points",
				0.75f,
				0.35f,
				0.5f,
				0.5f,
				glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)
			),
			new Button
			(
				new Text
				(
					(GM::gameSlot->roundIndex < 5) ? "Next round" : "Finish Game",
					0.75f,
					0.1f,
					0.4f,
					0.4f,
					glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)
				),
				0.65f,
				0.65f,
				1.0f,
				1.0f,
				0.2125f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclicky.mp3").c_str());

					if (Game::roundIndex < 5)
						Yui::loadScene(6);
					else
					{
						if (GM::gameSlot->gameType == GameType::STANDARD)
							Yui::loadScene(8);
						else
							Yui::loadScene(12);
					}
				}
			),
			new Button
			(
				new Image(minimapPath, true, 0.25f, 0.4625f, 0.5f, 0.925f),
				glm::vec4(1.0f, 0.984f, 0.961f, 0.025f),
				[]() {},
				[]() {},
				[]() {},
				false
			),
			new PointLine
			(
				glm::vec2
				(
					GM::gameSlot->rounds[Game::roundIndex - 1].guessCoords[0] * 0.5f,
					GM::gameSlot->rounds[Game::roundIndex - 1].guessCoords[1] * 0.925f
				),
				glm::vec2
				(
					GM::gameSlot->rounds[Game::roundIndex - 1].map.correctCoords[0] * 0.5f,
					GM::gameSlot->rounds[Game::roundIndex - 1].map.correctCoords[1] * 0.925f
				),
				glm::vec4(1.0f, 0.984f, 0.961f, 0.4f), 
				0.75f
			),
			new Image // correct marker
			(
				PATH_PREFIX.append("images\\textures\\valomarker.png").c_str(),
				true,
				GM::gameSlot->rounds[Game::roundIndex - 1].map.correctCoords[0] * 0.5f,
				GM::gameSlot->rounds[Game::roundIndex - 1].map.correctCoords[1] * 0.925f * 1.025f,
				0.034f * 0.525f,
				0.034f
			),
			new Image // guess marker
			(
				PATH_PREFIX.append("images\\textures\\marker.png").c_str(),
				true,
				GM::gameSlot->rounds[Game::roundIndex - 1].guessCoords[0] * 0.5f,
				GM::gameSlot->rounds[Game::roundIndex - 1].guessCoords[1] * 0.925f,
				0.015f * 0.525f,
				0.015f
			)
		);

		break;
	}

	case 8: // game results scene
	{
		for (size_t i = 0; i < GM::gameSlot->rounds.size(); i++)
		{
			std::string minimapPath = PATH_PREFIX.append("images\\minimaps\\").c_str();
			switch (GM::gameSlot->rounds[i].map.mapType)
			{
			case MapEnum::ASCENT:
				minimapPath += "ascentMap.png";
				break;

			case MapEnum::BIND:
				minimapPath += "bindMap.png";
				break;

			case MapEnum::BREEZE:
				minimapPath += "breezeMap.png";
				break;

			case MapEnum::FRACTURE:
				minimapPath += "fractureMap.png";
				break;

			case MapEnum::HAVEN:
				minimapPath += "havenMap.png";
				break;

			case MapEnum::ICEBOX:
				minimapPath += "iceboxMap.png";
				break;

			case MapEnum::PEARL:
				minimapPath += "pearlMap.png";
				break;

			case MapEnum::SPLIT:
				minimapPath += "splitMap.png";
				break;
			default:
#ifdef _DEBUG
				std::cout << "current round's map type unknown in results scene" << std::endl;
#endif
				break;
			}

			float realHeight = 0.925f;
			float nextX = 0.0f, nextY = 0.0f, nextW = 0.0f, nextH = 0.0f;
			switch (i)
			{
			case 0:
				nextX = 0.125f;
				nextY = realHeight * 0.75f;
				nextW = 0.25f;
				nextH = realHeight * 0.5f;
				break;

			case 1:
				nextX = 0.125f;
				nextY = realHeight * 0.25f;
				nextW = 0.25f;
				nextH = realHeight * 0.5f;
				break;

			case 2:
				nextX = 0.5f;
				nextY = realHeight * 0.5f;
				nextW = 0.5f;
				nextH = realHeight;
				break;

			case 3:
				nextX = 0.875f;
				nextY = realHeight * 0.75f;
				nextW = 0.25f;
				nextH = realHeight * 0.5f;
				break;

			case 4:
				nextX = 0.875f;
				nextY = realHeight * 0.25f;
				nextW = 0.25f;
				nextH = realHeight * 0.5f;
				break;

			default:
#ifdef _DEBUG
				std::cout << "unknown index in game result screen" << std::endl;
#endif
				break;
			}

			float roundRecapOpacity = 0.15f;
			Yui::addElements
			(
				4,
				new Button
				(
					new Image(minimapPath, true, nextX, nextY, nextW, nextH, roundRecapOpacity),
					glm::vec4(1.0f, 0.984f, 0.961f, 0.0f),
					[]() {},
					[]() {},
					[]() {},
					false
				),
				new PointLine
				(
					glm::vec2
					(
						(GM::gameSlot->rounds[i].map.correctCoords[0] * nextW) + (nextX - (nextW * 0.5f)),
						(GM::gameSlot->rounds[i].map.correctCoords[1] * nextH) + (nextY - (nextH * 0.5f))
					),						 
					glm::vec2				 
					(						 
						(GM::gameSlot->rounds[i].guessCoords[0] * nextW) + (nextX - (nextW * 0.5f)),
						(GM::gameSlot->rounds[i].guessCoords[1] * nextH) + (nextY - (nextH * 0.5f))
					),
					glm::vec4(1.0f, 0.984f, 0.961f, roundRecapOpacity * 0.2f),
					0.75f
				),
				new Image // correct marker
				(
					PATH_PREFIX.append("images\\textures\\valomarker.png").c_str(),
					true,
					(GM::gameSlot->rounds[i].map.correctCoords[0] * nextW) + (nextX - (nextW * 0.5f)),
					(GM::gameSlot->rounds[i].map.correctCoords[1] * nextH * 1.025f) + (nextY - (nextH * 0.5f)),
					0.015f * 0.525f,
					0.015f,
					roundRecapOpacity * 2.0f
				),
				new Image // guess marker
				(
					PATH_PREFIX.append("images\\textures\\marker.png").c_str(),
					true,
					(GM::gameSlot->rounds[i].guessCoords[0] * nextW) + (nextX - (nextW * 0.5f)),
					(GM::gameSlot->rounds[i].guessCoords[1] * nextH) + (nextY - (nextH * 0.5f)),
					0.015f * 0.525f,
					0.015f,
					roundRecapOpacity
				)
			);
		}

		Yui::addElements
		(
			6,
			new Button
			(
				new Text("GAMe Results", 0.5f, 0.9625f, 0.3f, 0.3f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
				20.0f,
				20.0f,
				1.5f,
				1.5f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				[]() {},
				[]() {},
				[]() {},
				false
			),
			new Text
			(
				"You scored",
				0.5f,
				0.725f,
				0.5f,
				0.5f,
				glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)
			),
			new Text
			(
				std::to_string(GM::gameSlot->sumOfRounds()) + " / 25000",
				0.5f,
				0.575f,
				0.5f,
				0.5f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f)
			),
			new Text
			(
				"totAl points",
				0.5f,
				0.425f,
				0.5f,
				0.5f,
				glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)
			),
			new Button
			(
				new Text("PlAy AgAin", 0.5f, 0.215f, 0.4f, 0.4f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
				0.25f,
				0.25f,
				1.0f,
				1.0f,
				0.25f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclicky.mp3").c_str());

					GM::stopGame(true);
					GM::newGame();

					if (GM::filteredMaps.size() >= 5)
						Yui::loadScene(6);
					else
					{
						GM::stopGame(false);
						Yui::loadScene(100);
					}
				}
			),
			new Button
			(
				new Text("MAin Menu", 0.5f, 0.085f, 0.4f, 0.4f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
				0.25f,
				0.25f,
				1.0f,
				1.0f,
				0.25f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclicky.mp3").c_str());

					GM::stopGame(true);

					Yui::loadScene(0);
				}
			)
		);

		break;
	}

	case 9: // freeplay round results
	{
		std::string minimapPath = PATH_PREFIX.append("images\\minimaps\\").c_str();
		switch (GM::gameSlot->rounds[GM::gameSlot->roundIndex - 1].map.mapType)
		{
		case MapEnum::ASCENT:
			minimapPath += "ascentMap.png";
			break;

		case MapEnum::BIND:
			minimapPath += "bindMap.png";
			break;

		case MapEnum::BREEZE:
			minimapPath += "breezeMap.png";
			break;

		case MapEnum::FRACTURE:
			minimapPath += "fractureMap.png";
			break;

		case MapEnum::HAVEN:
			minimapPath += "havenMap.png";
			break;

		case MapEnum::ICEBOX:
			minimapPath += "iceboxMap.png";
			break;

		case MapEnum::PEARL:
			minimapPath += "pearlMap.png";
			break;

		case MapEnum::SPLIT:
			minimapPath += "splitMap.png";
			break;
		default:
#ifdef _DEBUG
			std::cout << "current round's map type unknown in endless results scene" << std::endl;
#endif
			break;
		}

		Yui::addElements
		(
			11,
			new Button
			(
				new Text("FreeplAy Results", 0.5f, 0.9625f, 0.3f, 0.3f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
				20.0f,
				20.0f,
				1.5f,
				1.5f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				[]() {},
				[]() {},
				[]() {},
				false
			),
			new Text
			(
				std::string("Round ") + std::to_string(Game::roundIndex) + ":",
				0.75f,
				0.82f,
				0.5f,
				0.5f,
				glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)
			),
			new Text
			(
				std::to_string(GM::gameSlot->rounds[Game::roundIndex - 1].points) + " / 5000",
				0.75f,
				0.7f,
				0.5f,
				0.5f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f)
			),
			new Text
			(
				"TotAl Points:",
				0.75f,
				0.5f,
				0.5f,
				0.5f,
				glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)
			),
			new Text
			(
				std::to_string(GM::gameSlot->sumOfRounds()) + " / " + std::to_string(5000 * Game::roundIndex),
				0.75f,
				0.35f,
				0.5f,
				0.5f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f)
			),
			new Button
			(
				new Text
				(
					"PlAy AgAin",
					0.75f,
					0.2f,
					0.4f,
					0.4f,
					glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)
				),
				0.65f,
				0.65f,
				1.0f,
				1.0f,
				0.2125f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclicky.mp3").c_str());

					Yui::loadScene(6);
				}
			),
			new Button
			(
				new Text
				(
					"MAin Menu",
					0.75f,
					0.075f,
					0.4f,
					0.4f,
					glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)
				),
				0.65f,
				0.65f,
				1.0f,
				1.0f,
				0.2125f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclicky.mp3").c_str());

					GM::stopGame(false);

					Yui::loadScene(0);
				}
			),
			new Button
			(
				new Image(minimapPath, true, 0.25f, 0.4625f, 0.5f, 0.925f),
				glm::vec4(1.0f, 0.984f, 0.961f, 0.025f),
				[]() {},
				[]() {},
				[]() {},
				false
			),
			new PointLine
			(
				glm::vec2
				(
					GM::gameSlot->rounds[Game::roundIndex - 1].guessCoords[0] * 0.5f,
					GM::gameSlot->rounds[Game::roundIndex - 1].guessCoords[1] * 0.925f
				),
				glm::vec2
				(
					GM::gameSlot->rounds[Game::roundIndex - 1].map.correctCoords[0] * 0.5f,
					GM::gameSlot->rounds[Game::roundIndex - 1].map.correctCoords[1] * 0.925f
				),
				glm::vec4(1.0f, 0.984f, 0.961f, 0.4f), 
				0.75f
			),
			new Image // correct marker
			(
				PATH_PREFIX.append("images\\textures\\valomarker.png").c_str(),
				true,
				GM::gameSlot->rounds[Game::roundIndex - 1].map.correctCoords[0] * 0.5f,
				GM::gameSlot->rounds[Game::roundIndex - 1].map.correctCoords[1] * 0.925f * 1.025f,
				0.034f * 0.525f,
				0.034f
			),
			new Image // guess marker
			(
				PATH_PREFIX.append("images\\textures\\marker.png").c_str(),
				true,
				GM::gameSlot->rounds[Game::roundIndex - 1].guessCoords[0] * 0.5f,
				GM::gameSlot->rounds[Game::roundIndex - 1].guessCoords[1] * 0.925f,
				0.015f * 0.525f,
				0.015f
			)
		);

		if (Game::roundIndex >= GM::gameSlot->rounds.size())
		{
			ui2b(5)->visible = false;
			
			ui2b(6)->text->centerY = 0.1375f;
			ui2b(6)->centerY = 0.1375f;
			ui2b(6)->uMargin = 1.5f;
			ui2b(6)->dMargin = 1.5f;
			ui2b(6)->update();
		}

		break;
	}

	case 10: // tutorial screen
	{
		Yui::setTutorial();

		break;
	}

	case 11: // import screen
	{
		Yui::addElements
		(
			9,
			new Button
			(
				0.5f,
				0.8735f,
				1.0f,
				0.003f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				[]() {},
				[]() {},
				[]() {},
				false
			),
			new Button
			(
				new Text("BAck", 0.0675f, 0.9375f, 0.3f, 0.3f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
				0.2f,
				0.2f,
				1.25f,
				1.25f,
				glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
				glm::vec4(1.0f, 0.984f, 0.961f, 0.04f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclick.mp3").c_str());
					Yui::loadScene(0);
				}
			),
			new ValoButton
			(
				new Text("IMport GAMe", 0.5f, 0.9375f, 0.75f, 0.75f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
				0.01f,
				0.01f,
				0.65f,
				0.65f,
				0.3f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
				[]() {},
				[]() {}
			),
			new Text("", 0.5f, 0.6035f, 0.65f, 0.65f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f), false),
			new Button
			(
				0.5f,
				0.5535f,
				0.7f,
				0.006f,
				glm::vec4(1.0f, 0.984f, 0.961f, 1.0f),
				glm::vec4(1.0f, 0.984f, 0.961f, 1.0f),
				[]() {},
				[]() {},
				[]() {},
				false
			),
			new Button
			(
				new Text("CleAr", 0.25f, 0.43f, 0.45f, 0.45f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
				0.35f,
				0.35f,
				1.25f,
				1.25f,
				0.2125f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclick.mp3").c_str());

					ui2t(7)->visible = false;

					ui2t(3)->changeText("");
					ui2t(3)->update();

					ui2b(8)->visible = false;
				},
				[]() {},
				false
			),
			new Button
			(
				new Text("PAste", 0.75f, 0.43f, 0.45f, 0.45f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
				0.35f,
				0.35f,
				1.25f,
				1.25f,
				0.2125f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclick.mp3").c_str());

					ui2t(7)->visible = false;

					ui2t(3)->changeText(Yui::clipboardText());
					ui2t(3)->update();

					ui2b(8)->visible = true;
				},
				[]() {},
				false
			),
			new Text("InvAlid iMport forMAt", 0.5f, 0.76f, 0.5f, 0.5f, glm::vec4(0.741f, 0.224f, 0.267f, 1.0f)),
			new Button
			(
				new Text("PlAy", 0.5f, 0.135f, 0.625f, 0.625f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
				0.75f,
				0.75f,
				1.25f,
				1.25f,
				0.2125f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoplayclick.mp3").c_str());

					if (GM::validateImportString())
					{
						GM::newImport(ui2t(3)->text);

						Yui::loadScene(6);
					}
					else
						ui2t(7)->visible = true;
				}
			)
		);

		ui2t(7)->visible = false;
		ui2b(8)->visible = false;

		break;
	}

	case 12: // import game results
	{
		for (size_t i = 0; i < GM::gameSlot->rounds.size(); i++)
		{
			std::string minimapPath = PATH_PREFIX.append("images\\minimaps\\").c_str();
			switch (GM::gameSlot->rounds[i].map.mapType)
			{
			case MapEnum::ASCENT:
				minimapPath += "ascentMap.png";
				break;

			case MapEnum::BIND:
				minimapPath += "bindMap.png";
				break;

			case MapEnum::BREEZE:
				minimapPath += "breezeMap.png";
				break;

			case MapEnum::FRACTURE:
				minimapPath += "fractureMap.png";
				break;

			case MapEnum::HAVEN:
				minimapPath += "havenMap.png";
				break;

			case MapEnum::ICEBOX:
				minimapPath += "iceboxMap.png";
				break;

			case MapEnum::PEARL:
				minimapPath += "pearlMap.png";
				break;

			case MapEnum::SPLIT:
				minimapPath += "splitMap.png";
				break;
			default:
#ifdef _DEBUG
				std::cout << "current round's map type unknown in results scene" << std::endl;
#endif
				break;
			}

			float realHeight = 0.925f;
			float nextX = 0.0f, nextY = 0.0f, nextW = 0.0f, nextH = 0.0f;
			switch (i)
			{
			case 0:
				nextX = 0.125f;
				nextY = realHeight * 0.75f;
				nextW = 0.25f;
				nextH = realHeight * 0.5f;
				break;

			case 1:
				nextX = 0.125f;
				nextY = realHeight * 0.25f;
				nextW = 0.25f;
				nextH = realHeight * 0.5f;
				break;

			case 2:
				nextX = 0.5f;
				nextY = realHeight * 0.5f;
				nextW = 0.5f;
				nextH = realHeight;
				break;

			case 3:
				nextX = 0.875f;
				nextY = realHeight * 0.75f;
				nextW = 0.25f;
				nextH = realHeight * 0.5f;
				break;

			case 4:
				nextX = 0.875f;
				nextY = realHeight * 0.25f;
				nextW = 0.25f;
				nextH = realHeight * 0.5f;
				break;

			default:
#ifdef _DEBUG
				std::cout << "unknown index in game result screen" << std::endl;
#endif
				break;
			}

			float roundRecapOpacity = 0.15f;
			Yui::addElements
			(
				4,
				new Button
				(
					new Image(minimapPath, true, nextX, nextY, nextW, nextH, roundRecapOpacity),
					glm::vec4(1.0f, 0.984f, 0.961f, 0.0f),
					[]() {},
					[]() {},
					[]() {},
					false
				),
				new PointLine
				(
					glm::vec2
					(
						(GM::gameSlot->rounds[i].map.correctCoords[0] * nextW) + (nextX - (nextW * 0.5f)),
						(GM::gameSlot->rounds[i].map.correctCoords[1] * nextH) + (nextY - (nextH * 0.5f))
					),
					glm::vec2
					(
						(GM::gameSlot->rounds[i].guessCoords[0] * nextW) + (nextX - (nextW * 0.5f)),
						(GM::gameSlot->rounds[i].guessCoords[1] * nextH) + (nextY - (nextH * 0.5f))
					),
					glm::vec4(1.0f, 0.984f, 0.961f, roundRecapOpacity * 0.2f),
					0.75f
				),
				new Image // correct marker
				(
					PATH_PREFIX.append("images\\textures\\valomarker.png").c_str(),
					true,
					(GM::gameSlot->rounds[i].map.correctCoords[0] * nextW) + (nextX - (nextW * 0.5f)),
					(GM::gameSlot->rounds[i].map.correctCoords[1] * nextH * 1.025f) + (nextY - (nextH * 0.5f)),
					0.015f * 0.525f,
					0.015f,
					roundRecapOpacity * 2.0f
				),
				new Image // guess marker
				(
					PATH_PREFIX.append("images\\textures\\marker.png").c_str(),
					true,
					(GM::gameSlot->rounds[i].guessCoords[0] * nextW) + (nextX - (nextW * 0.5f)),
					(GM::gameSlot->rounds[i].guessCoords[1] * nextH) + (nextY - (nextH * 0.5f)),
					0.015f * 0.525f,
					0.015f,
					roundRecapOpacity
				)
			);
		}

		Yui::addElements
		(
			9,
			new Button
			(
				new Text("IMported GAMe Results", 0.5f, 0.9625f, 0.3f, 0.3f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
				20.0f,
				20.0f,
				1.5f,
				1.5f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				[]() {},
				[]() {},
				[]() {},
				false
			),
			new Text
			(
				"You scored",
				0.25f,
				0.76f,
				0.5f,
				0.5f,
				glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)
			),
			new Text
			(
				std::to_string(GM::gameSlot->sumOfRounds()) + " / 25000",
				0.25f,
				0.535f,
				0.5f,
				0.5f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f)
			),
			new Text
			(
				"totAl points",
				0.25f,
				0.325f,
				0.5f,
				0.5f,
				glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)
			),
			new Text
			(
				"The originAl",
				0.75f,
				0.805f,
				0.5f,
				0.5f,
				glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)
			),
			new Text
			(
				"gAMe scored",
				0.75f,
				0.715f,
				0.5f,
				0.5f,
				glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)
			),
			new Text
			(
				std::to_string(GM::importedPoints) + " / 25000",
				0.75f,
				0.535f,
				0.5f,
				0.5f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f)
			),
			new Text
			(
				"totAl points",
				0.75f,
				0.325f,
				0.5f,
				0.5f,
				glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)
			),
			new Button
			(
				new Text("MAin Menu", 0.5f, 0.1f, 0.4f, 0.4f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
				0.25f,
				0.185f,
				1.0f,
				1.0f,
				0.25f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclicky.mp3").c_str());

					GM::stopGame(true);

					Yui::loadScene(0);
				}
			)
		);

		break;
	}

	case 100:
	{
		Yui::addElements
		(
			6,
			new Button
			(
				0.5f,
				0.8735f,
				1.0f,
				0.003f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				[]() {},
				[]() {},
				[]() {},
				false
			),
			new ValoButton
			(
				new Text("OH NO", 0.5f, 0.9375f, 0.75f, 0.75f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
				0.01f,
				0.01f,
				0.65f,
				0.65f,
				0.5f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
				[]() {},
				[]() {}
			),
			new Text
			(
				"vAloGuessr hAs encountered A fAtAl error.",
				0.5f,
				0.75f,
				0.45f,
				0.45f,
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
			),
			new Text
			(
				"the fix is likely to delete config/config.vAlog",
				0.5f,
				0.5f,
				0.4375f,
				0.4375f,
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
			),
			new Text
			(
				"Press the button to quit the progrAM",
				0.5f,
				0.25f,
				0.475f,
				0.475f,
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
			),
			new Button
			(
				new Text
				(
					"Exit",
					0.5f,
					0.1f,
					0.6f,
					0.6f,
					glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
				),
				0.9f,
				0.9f,
				0.75f,
				0.75f,
				0.2f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::toClose = true;
				}
			)
		);

		break;
	}

	default:
		Yui::loadScene(0);

		break;
	}
}

/*
		END OF YUI::LOADSCENE
*/

void Yui::setStats()
{
	GM::calcStats();

	ui2t(4)->changeText(std::to_string(GM::config.playCount).c_str());
	ui2t(6)->changeText(std::to_string((GM::config.playCount) ? GM::config.totalPoints / GM::config.playCount : 0).c_str());
	ui2t(8)->changeText(std::to_string(GM::config.pointRecord).c_str());
	ui2t(10)->changeText(std::to_string(GM::config.lowestPoints).c_str());
	ui2t(12)->changeText(std::to_string(GM::config.Ls).c_str());
	ui2t(14)->changeText(std::to_string(GM::config.Ws).c_str());

	Yui::updateAll();
}

void Yui::setAchievements()
{
	GM::calcStats();
	GM::calcAchievements();

	for (size_t i = 0; i < 8; i++)
	{
		if (GM::config.achievements[i])
		{
			ui2cb((2 * i) + 3)->setStatus(true);
			ui2t(((2 * i) + 1) + 3)->textColor = glm::vec4(0.741f, 0.224f, 0.267f, 1.0f);
		}
		else 
		{
			ui2cb((2 * i) + 3)->setStatus(false);
			ui2t(((2 * i) + 1) + 3)->textColor = glm::vec4(1.0f, 0.984f, 0.961f, 0.925f);
		}
	}
}

void Yui::setSettingsButtons()
{
	ui2b(6)->bgColor = glm::vec4(0.533f, 0.533f, 0.533f, 1.0f);
	ui2b(6)->hoverColor = glm::vec4(0.941f, 0.351f, 0.402f, 1.0f);

	ui2b(7)->bgColor = glm::vec4(0.533f, 0.533f, 0.533f, 1.0f);
	ui2b(7)->hoverColor = glm::vec4(0.941f, 0.351f, 0.402f, 1.0f);

	ui2b(8)->bgColor = glm::vec4(0.533f, 0.533f, 0.533f, 1.0f);
	ui2b(8)->hoverColor = glm::vec4(0.941f, 0.351f, 0.402f, 1.0f);

	switch (GM::config.difficulty)
	{
	case 1:
		ui2b(6)->bgColor = glm::vec4(0.941f, 0.351f, 0.402f, 1.0f);
		ui2b(6)->hoverColor = glm::vec4(0.941f, 0.351f, 0.402f, 1.0f);

		break;

	case 2:
		ui2b(7)->bgColor = glm::vec4(0.941f, 0.351f, 0.402f, 1.0f);
		ui2b(7)->hoverColor = glm::vec4(0.941f, 0.351f, 0.402f, 1.0f);

		break;

	case 3:
		ui2b(8)->bgColor = glm::vec4(0.941f, 0.351f, 0.402f, 1.0f);
		ui2b(8)->hoverColor = glm::vec4(0.941f, 0.351f, 0.402f, 1.0f);

		break;
	}

	for (int i = 0; i < 8; i++)
	{
		if (GM::config.mapChoice[i])
		{
			if (ui2b(10 + i)->hovering) // artificial 
			{
				ui2b(10 + i)->bgColor = glm::vec4(0.941f, 0.351f, 0.402f, 1.0f);
				ui2b(10 + i)->hoverColor = glm::vec4(0.941f, 0.351f, 0.402f, 1.0f);
			}
			else // natural
			{
				ui2b(10 + i)->bgColor = glm::vec4(0.941f, 0.351f, 0.402f, 1.0f);
				ui2b(10 + i)->hoverColor = glm::vec4(0.533f, 0.533f, 0.533f, 1.0f);
			}
		}
		else
		{
			if (ui2b(10 + i)->hovering) // artificial 
			{
				ui2b(10 + i)->bgColor = glm::vec4(0.533f, 0.533f, 0.533f, 1.0f);
				ui2b(10 + i)->hoverColor = glm::vec4(0.533f, 0.533f, 0.533f, 1.0f);
			}
			else // natural
			{
				ui2b(10 + i)->bgColor = glm::vec4(0.533f, 0.533f, 0.533f, 1.0f);
				ui2b(10 + i)->hoverColor = glm::vec4(0.941f, 0.351f, 0.402f, 1.0f);
			}
		}
	}
}

void Yui::setTutorial()
{
	Yui::purgeElements();
	Yui::addElements
	(
		6,
		new Button
		(
			0.5f,
			0.8735f,
			1.0f,
			0.003f,
			glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
			glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
			[]() {},
			[]() {},
			[]() {},
			false
		),
		new Button
		(
			new Text("BAck", 0.0675f, 0.9375f, 0.3f, 0.3f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
			0.2f,
			0.2f,
			1.25f,
			1.25f,
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
			glm::vec4(1.0f, 0.984f, 0.961f, 0.04f),
			[]()
			{
				Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
			},
			[]()
			{
				Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclick.mp3").c_str());
				Yui::loadScene(0);
			}
		),
		new ValoButton
		(
			new Text("TutoriAl", 0.5f, 0.9375f, 0.75f, 0.75f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
			0.01f,
			0.01f,
			0.65f,
			0.65f,
			0.3f,
			glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
			glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
			[]() {},
			[]()
			{

			}
		),
		new Button
		(
			new Text("<", 0.125f, 0.5725f, 0.45f, 0.7f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
			2.5f,
			2.5f,
			1.0f,
			1.0f,
			0.25f,
			glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
			glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			[]()
			{
				if (Yui::tutorialPage > 0)
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				}
			},
			[]()
			{
				if (Yui::tutorialPage > 0)
				{
					Yui::tutorialPage--;
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclick.mp3").c_str());
					Yui::setTutorial();
				}
			}
		),
		new Button
		(
			new Text(">", 0.875f, 0.5725f, 0.45f, 0.7f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
			2.5f,
			2.5f,
			1.0f,
			1.0f,
			0.25f,
			glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
			glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			[]()
			{
				if (Yui::tutorialPage < 5) // total tutorial pages
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				}
			},
			[]()
			{
				if (Yui::tutorialPage < 5)
				{
					Yui::tutorialPage++;
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclick.mp3").c_str());
					Yui::setTutorial();
				}
			}
		),
		new Button
		(
			0.5f, 0.5725f, 1.0f, 0.6f,
			glm::vec4(1.0f, 1.0f, 1.0f, 0.0375f),
			glm::vec4(1.0f, 1.0f, 1.0f, 0.0375f),
			[]() {},
			[]() {},
			[]() {},
			false
		)
	);

	if (Yui::tutorialPage == 0)
	{
		ui2b(3)->bgColor = glm::vec4(0.533f, 0.533f, 0.533f, 1.0f);
		ui2b(3)->hoverColor = glm::vec4(0.533f, 0.533f, 0.533f, 1.0f);
	}
	else if (Yui::tutorialPage == 5)
	{
		ui2b(4)->bgColor = glm::vec4(0.533f, 0.533f, 0.533f, 1.0f);
		ui2b(4)->hoverColor = glm::vec4(0.533f, 0.533f, 0.533f, 1.0f);
	}

	switch (Yui::tutorialPage)
	{
	case 0:
		Yui::addElements
		(
			2,
			new Image(PATH_PREFIX.append("images\\tutorial\\home.png").c_str(), false, 0.5f, 0.5725f, 0.5f, 0.5f),
			new Text("This is the MAin Menu! You cAn go Anywhere froM here.", 0.5f, 0.13625f, 0.35f, 0.35f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
		);

		// .2725

		break;

	case 1:
		Yui::addElements
		(
			3,
			new Image(PATH_PREFIX.append("images\\tutorial\\play.png").c_str(), false, 0.5f, 0.5725f, 0.5f, 0.5f),
			new Text("when plAying, press on the MiniMAp to plAce your guess.", 0.5f, 0.204375f, 0.35f, 0.35f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
			new Text("After thAt, press the guess button to subMit it.", 0.5f, 0.068125f, 0.35f, 0.35f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
		);

		break;

	case 2:
		Yui::addElements
		(
			3,
			new Image(PATH_PREFIX.append("images\\tutorial\\results.png").c_str(), false, 0.5f, 0.5725f, 0.5f, 0.5f),
			new Text("here, you cAn see your points And how close you were.", 0.5f, 0.204375f, 0.35f, 0.35f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
			new Text("then you cAn Move on to the next round!", 0.5f, 0.068125f, 0.35f, 0.35f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
		);

		break;

	case 3:
		Yui::addElements
		(
			3,
			new Image(PATH_PREFIX.append("images\\tutorial\\settings.png").c_str(), false, 0.5f, 0.5725f, 0.5f, 0.5f),
			new Text("in settings, you cAn choose the difficulty And which MAps Are included.", 0.5f, 0.204375f, 0.25f, 0.3f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
			new Text("these settings sAve, so they will stAy After you close the progrAM!", 0.5f, 0.068125f, 0.3f, 0.3f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
		);

		break;

	case 4:
		Yui::addElements
		(
			3,
			new Image(PATH_PREFIX.append("images\\tutorial\\history.png").c_str(), false, 0.5f, 0.5725f, 0.5f, 0.5f),
			new Text("in history, you cAn see All of your previous stAndArd gAMes. grAy gAMes Are iMported gAMes", 0.5f, 0.204375f, 0.2125f, 0.3f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
			new Text("click the export button to copy the MApcode (more on the next slide)", 0.5f, 0.068125f, 0.275f, 0.3f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
		);

		break;

	case 5:
		Yui::addElements
		(
			3,
			new Image(PATH_PREFIX.append("images\\tutorial\\import.png").c_str(), false, 0.5f, 0.5725f, 0.5f, 0.5f),
			new Text("here, you cAN pAste your clipboArd (the MApcode) And plAy Any gAme AgAin!", 0.5f, 0.204375f, 0.25f, 0.3f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
			new Text("with this, you cAn send MApcodes to friends And see how they do!", 0.5f, 0.068125f, 0.3f, 0.3f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
		);

		break;

	default:

#ifdef _DEBUG
		std::cout << "invalid settings scene" << std::endl;
#endif

		break;
	}
}

void Yui::setHistory()
{
	// need purge and element add in this method due to page scrolling needing to purge element list each click, hence needing to re-add the standard elements as well
	Yui::purgeElements();
	Yui::addElements
	(
		6,
		new Button
		(
			0.5f,
			0.8735f,
			1.0f,
			0.003f,
			glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
			glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
			[]() {},
			[]() {},
			[]() {},
			false
		),
		new Button
		(
			new Text("BAck", 0.0675f, 0.9375f, 0.3f, 0.3f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
			0.2f,
			0.2f,
			1.25f,
			1.25f,
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
			glm::vec4(1.0f, 0.984f, 0.961f, 0.04f),
			[]()
			{
				Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
			},
			[]()
			{
				Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclick.mp3").c_str());
				Yui::loadScene(0);
			}
		),
		new ValoButton
		(
			new Text("History", 0.5f, 0.9375f, 0.75f, 0.75f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
			0.01f,
			0.01f,
			0.65f,
			0.65f,
			0.3f,
			glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
			glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
			[]() {},
			[]() {}
		),
		new Button
		(
			new Text("<", 0.1f, 0.08f, 0.5f, 0.5f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
			5.0f,
			5.0f,
			1.0f,
			1.0f,
			0.25f,
			glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
			glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			[]()
			{
				if (Yui::historyPage > 1)
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				}
			},
			[]()
			{
				if (Yui::historyPage > 1)
				{
					Yui::historyPage--;
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclick.mp3").c_str());
					Yui::setHistory();
				}
			}
		),
		new Button
		(
			new Text("PAge: X/30", 0.5f, 0.08f, 0.5f, 0.5f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
			0.6f,
			0.6f,
			1.0f,
			1.0f,
			0.25f,
			glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
			glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			[]() {},
			[]() {},
			[]() {},
			false
		),
		new Button
		(
			new Text(">", 0.9f, 0.08f, 0.5f, 0.5f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
			5.0f,
			5.0f,
			1.0f,
			1.0f,
			0.25f,
			glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
			glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			[]()
			{
				size_t pageCount = GM::config.games.size() / 4;
				if (GM::config.games.size() % 4 != 0)
					pageCount++;

				if (Yui::historyPage < pageCount)
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				}
			},
			[]()
			{
				size_t pageCount = GM::config.games.size() / 4;
				if (GM::config.games.size() % 4 != 0)
					pageCount++;

				if (!pageCount)
					pageCount = 1;

				if (Yui::historyPage < pageCount)
				{
					Yui::historyPage++;
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclick.mp3").c_str());
					Yui::setHistory();
				}
			}
		)
	);

	size_t pageCount = GM::config.games.size() / 4;
	if (GM::config.games.size() % 4 != 0)
		pageCount++;

	if (!pageCount)
		pageCount = 1;
	
	ui2b(4)->text->changeText(std::format("PAge: {}/{}", Yui::historyPage, pageCount));

	if (Yui::historyPage <= 1)
	{
		ui2b(3)->bgColor = glm::vec4(0.533f, 0.533f, 0.533f, 1.0f);
		ui2b(3)->hoverColor = glm::vec4(0.533f, 0.533f, 0.533f, 1.0f);
	}
	else
	{
		ui2b(3)->bgColor = glm::vec4(0.741f, 0.224f, 0.267f, 1.0f);
		ui2b(3)->hoverColor = glm::vec4(0.871f, 0.281f, 0.332f, 1.0f);
	}

	if (Yui::historyPage >= pageCount)
	{
		ui2b(5)->bgColor = glm::vec4(0.533f, 0.533f, 0.533f, 1.0f);
		ui2b(5)->hoverColor = glm::vec4(0.533f, 0.533f, 0.533f, 1.0f);
	}
	else
	{
		ui2b(5)->bgColor = glm::vec4(0.741f, 0.224f, 0.267f, 1.0f);
		ui2b(5)->hoverColor = glm::vec4(0.871f, 0.281f, 0.332f, 1.0f);
	}

	if (!GM::config.games.size())
	{
		Yui::addElements
		(
			2,
			new Text("No gAMes recorded", 0.5f, 0.61f, 0.5f, 0.5f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f)),
			new Text("PlAy A gAMe And coMe bAck", 0.51f, 0.4f, 0.5f, 0.5f, glm::vec4(1.0f, 0.984f, 0.961f, 1.0f))
		);
		
		return;
	}

	for (size_t i = 0; i < ((Yui::historyPage == pageCount) ? (((GM::config.games.size() - 1) % 4) + 1) : 4); i++)
	{
		Yui::addElements
		(
			2,
			new Text
			(
				"",
				0.035f, 
				0.765f - (i * 0.18f), 
				0.4f, 
				0.4f, 
				glm::vec4(1.0f, 0.984f, 0.961f, 1.0f),
				true
			),
			new Button
			(
				new Text("Export", 0.825f, 0.765f - (i * 0.18f), 0.4f, 0.4f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
				0.55f,
				0.55f,
				1.25f,
				1.25f,
				0.25f,
				glm::vec4(0.741f, 0.224f, 0.267f, 1.0f),
				glm::vec4(0.871f, 0.281f, 0.332f, 1.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valohover.mp3").c_str());
				},
				[]()
				{
					Yui::soundEngine->play2D(PATH_PREFIX.append("audio\\valoclicky.mp3").c_str());

					// determines which button has been pressed to export
					for (size_t i = 0; i < (Yui::UIElements.size() - 6) / 2; i++)
					{
						if (ui2b(7 + (i * 2))->hovering)
						{
							Yui::exportGame((unsigned)i);

							ui2b(7 + (i * 2))->text->changeText("Copied!");
							ui2b(7 + (i * 2))->update();
						}
					}
				},
				[]()
				{
					// resets all buttons to say "Export"
					for (size_t i = 0; i < (Yui::UIElements.size() - 6) / 2; i++)
					{
						ui2b(7 + (i * 2))->text->changeText("Export");
						ui2b(7 + (i * 2))->update();
					}
				},
				false
			)
		);

		if (GM::config.games[((Yui::historyPage - 1) * 4) + i].gameType == GameType::IMPORT)
			ui2t(6 + 2 * i)->textColor = glm::vec4(0.433f, 0.433f, 0.433f, 1.0f);

		tm lt;
		localtime_s(&lt, &GM::config.games[((Yui::historyPage - 1) * 4) + i].time);

		ui2t(6 + (2 * i))->changeText(std::format("{}/{}/{} {}:{}{}\tPoints: {}", 1 + lt.tm_mon, lt.tm_mday, 1900 + lt.tm_year, lt.tm_hour, (lt.tm_min < 10) ? "0" : "", lt.tm_min, GM::config.games[Yui::historyPage * i].sumOfRounds()));
	}

	Yui::updateAll();
}

void Yui::setMarker()
{
	if (Yui::marker)
		Yui::removeElement(Yui::marker);

	float markerAbsX = (float)(GM::mouseX / (float)*UIElement::WINDOWWIDTH);
	float markerAbsY = (float)(*UIElement::WINDOWHEIGHT - GM::mouseY) / (float)*UIElement::WINDOWHEIGHT;

	if (Yui::minimapScaled)
	{
		Yui::markerRelX = (markerAbsX - (1.0f - scaledMinimapWidth)) / scaledMinimapWidth;
		Yui::markerRelY = markerAbsY / scaledMinimapHeight;
	}
	else
	{
		Yui::markerRelX = (markerAbsX - (1.0f - unscaledMinimapWidth)) / unscaledMinimapWidth;
		Yui::markerRelY = markerAbsY / unscaledMinimapHeight;
	}

#ifdef _DEBUG
	std::cout << "relative marker: (" << markerRelX << "f, " << markerRelY << "f)" << std::endl;
#endif

	Yui::marker = new Image(PATH_PREFIX.append("images\\textures\\marker.png").c_str(), true, markerAbsX, markerAbsY, scaledMarkerWidth, scaledMarkerHeight);
	Yui::addElement(Yui::marker);
}

void Yui::updateMarker()
{
	if (!Yui::marker)
		return;

	if (Yui::minimapScaled)
	{
		Yui::marker->centerX = (1.0f - scaledMinimapWidth) + (Yui::markerRelX * scaledMinimapWidth);
		Yui::marker->centerY = Yui::markerRelY * scaledMinimapHeight;
		Yui::marker->width = scaledMarkerWidth;
		Yui::marker->height = scaledMarkerHeight;
	}
	else
	{
		Yui::marker->centerX = (1.0f - unscaledMinimapWidth) + (Yui::markerRelX * unscaledMinimapWidth);
		Yui::marker->centerY = Yui::markerRelY * unscaledMinimapHeight;
		Yui::marker->width = unscaledMarkerWidth;
		Yui::marker->height = unscaledMarkerHeight;
	}

	Yui::marker->update();
}

std::string Yui::clipboardText()
{
	OpenClipboard(0);

	HANDLE hMem = GetClipboardData(CF_TEXT);

	char* textPtr = (char*)GlobalLock(hMem);

	if (!textPtr)
	{
#ifdef _DEBUG
		std::cout << "failed to open clipboard to import game" << std::endl;
#endif
		return "";
	}

	std::string clipboardText(textPtr);
#ifdef _DEBUG
	std::cout << "clipboard text is " << clipboardText << std::endl;
#endif

	GlobalUnlock(hMem);
	CloseClipboard();

	return clipboardText;
}

void Yui::exportGame(unsigned gameIndex)
{
 	int correctIndex = (4 * (Yui::historyPage - 1)) + gameIndex;

#ifdef _DEBUG
	std::cout << "gameIndex is " << gameIndex << ", correct index is " << correctIndex << std::endl;
#endif

	std::string clipboardString = std::format
	(
		"{:d};{:x};{:x};{:x};{:x};{:x};{:x}",
		GM::config.games[correctIndex].difficulty,
		GM::mapIDOf(GM::config.games[correctIndex].difficulty, GM::config.games[correctIndex].rounds[0].map.mapPath),
		GM::mapIDOf(GM::config.games[correctIndex].difficulty, GM::config.games[correctIndex].rounds[1].map.mapPath),
		GM::mapIDOf(GM::config.games[correctIndex].difficulty, GM::config.games[correctIndex].rounds[2].map.mapPath),
		GM::mapIDOf(GM::config.games[correctIndex].difficulty, GM::config.games[correctIndex].rounds[3].map.mapPath),
		GM::mapIDOf(GM::config.games[correctIndex].difficulty, GM::config.games[correctIndex].rounds[4].map.mapPath),
		GM::config.games[correctIndex].sumOfRounds()
	);

	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, clipboardString.size() + 1);
	LPVOID locked;

	if (hMem != NULL)
		locked = GlobalLock(hMem);
	else
	{
#ifdef _DEBUG 
		std::cout << "hMem is NULL" << std::endl;
#endif
		return;
	}

	if (locked != NULL)
		::memcpy(locked, clipboardString.c_str(), clipboardString.size() + 1);
	else
	{
#ifdef _DEBUG 
		std::cout << "locked is NULL" << std::endl;
#endif
		return;
	}

	OpenClipboard(0);
	EmptyClipboard();

	SetClipboardData(CF_TEXT, hMem);

	GlobalUnlock(hMem);
	CloseClipboard();

#ifdef _DEBUG
	std::cout << clipboardString << " has been copied to the clipboard!" << std::endl;
#endif
}