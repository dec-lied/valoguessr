#pragma once

#include <Windows.h>

#include <regex>
#include <vector>
#include <sstream>
#include <cstdarg>

#include "Config.h"
#include "Shader.h"
#include "CheckBox.h"
#include "PointLine.h"
#include "ValoButton.h"
#include "ScrollText.h"
#include "GameManager.h"

#define ui2b(ind) ((Button*)Yui::UIElements[ind])
#define ui2t(ind) ((Text*)Yui::UIElements[ind])
#define ui2i(ind) ((Image*)Yui::UIElements[ind])
#define ui2cb(ind) ((CheckBox*)Yui::UIElements[ind])
#define windowRatioHW (*UIElement::WINDOWHEIGHT / (float)*UIElement::WINDOWWIDTH)
#define scaledMinimapHeight 0.75f
#define scaledMinimapWidth (scaledMinimapHeight * 0.525f)
#define unscaledMinimapHeight 0.4f
#define unscaledMinimapWidth (unscaledMinimapHeight * 0.525f)
#define scaledMarkerHeight 0.0065f
#define scaledMarkerWidth (0.0065f * 0.525f)
#define unscaledMarkerHeight 0.00575f
#define unscaledMarkerWidth (0.00575f * 0.525f)

#ifdef _DEBUG
#define PATH_PREFIX std::string(".\\")
#else
#define PATH_PREFIX std::string(".\\..\\")
#endif

class Yui
{
public:
	// UIElements vector
	static std::vector<UIElement*> UIElements;

	// adds an element to Yui::UIElements
	static void addElement(UIElement* element);

	// adds numerous elements to Yui::UIElements given a count of elements and the elements
	static void addElements(unsigned count, ...);

	// removes & deallocates an element from Yui::UIElements given address of element
	static void removeElement(UIElement* element);

	// deallocates and clears all elements in Yui::UIelements
	static void purgeElements();

	// general methods
	static void init(const char* fontName, float* ratioW, float* ratioH, unsigned pixelHeight);
	static void cleanup();

	// sound
	static irrklang::ISoundEngine* soundEngine;
	static irrklang::ISoundSource* menuBGM;

	// animation-related members
	static bool toClose;
	static bool playable;
	static unsigned scene;

	static void loadScene(unsigned sceneID);

	// scene specific members and methods
	static bool minimapScaled;
	static unsigned exportCount;
	static unsigned historyPage;
	static unsigned tutorialPage;

	static Image* marker;
	static float markerRelX, markerRelY;

	static void setStats();
	static void setAchievements();
	static void setSettingsButtons();
	static void setHistory();
	static void setTutorial();
	static void setMarker();
	static void updateMarker();
	
	static std::string clipboardText();

	static void exportGame(unsigned gameIndex);

	// QoL calling update and render derived methods 
	static void updateAll();
	static void renderAll();
};
