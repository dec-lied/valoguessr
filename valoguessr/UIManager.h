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

#define ui2b(ind) ((Button*)Yui::getInstance().UIElements[ind])
#define ui2t(ind) ((Text*)Yui::getInstance().UIElements[ind])
#define ui2i(ind) ((Image*)Yui::getInstance().UIElements[ind])
#define ui2cb(ind) ((CheckBox*)Yui::getInstance().UIElements[ind])
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
	// singleton methods
	Yui();
	~Yui();
	void init(const char* fontName, float* ratioW, float* ratioH, unsigned pixelHeight);
	static Yui& getInstance();

	// UIElements vector
	std::vector<UIElement*> UIElements;

	// adds an element to Yui::getInstance().UIElements
	void addElement(UIElement* element);

	// adds numerous elements to Yui::getInstance().UIElements given a count of elements and the elements
	void addElements(unsigned count, ...);

	// removes & deallocates an element from Yui::getInstance().UIElements given address of element
	void removeElement(UIElement* element);

	// deallocates and clears all elements in Yui::UIelements
	void purgeElements();

	// sound
	irrklang::ISoundEngine* soundEngine;
	irrklang::ISoundSource* menuBGM;

	// animation-related members
	bool toClose;
	bool playable;
	unsigned scene;

	void loadScene(unsigned sceneID);

	// scene specific members and methods
	bool minimapScaled;
	unsigned exportCount;
	unsigned historyPage;
	unsigned tutorialPage;

	// minimap marker members
	Image* marker;
	float markerRelX, markerRelY;

	// scene specific setter methods
	void setStats();
	void setAchievements();
	void setSettingsButtons();
	void setHistory();
	void setTutorial();
	void setMarker();
	void updateMarker();
	
	// game import / export methosd
	std::string clipboardText();
	void exportGame(unsigned gameIndex);

	// QoL calling update and render derived methods 
	void updateAll();
	void renderAll();
};
