#include "UIManager.h"

#include <iostream>

unsigned WINDOWWIDTH, WINDOWHEIGHT;
float ratioW = 1.0f, ratioH = 1.0f;

bool clicking = false, prevClick = false, focused = true, justIconified = false;

void window_focus_callback(GLFWwindow* window, int glfocused)
{
	if (glfocused)
		focused = true;
	else
		focused = false;
}

// updates window width & height when window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	if (justIconified)
	{
		justIconified = false;
		return;
	}

	ratioW *= (float)width / (float)WINDOWWIDTH;
	ratioH *= (float)height / (float)WINDOWHEIGHT;

	WINDOWWIDTH = width;
	WINDOWHEIGHT = height;

	UIElement::projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);

	Yui::getInstance().updateAll();

	glViewport(0, 0, width, height);
}

void window_iconify_callback(GLFWwindow* window, int iconified)
{
	justIconified = true;
}

void processInput(GLFWwindow* window);

int main()
{
	/*
		Setting up OpenGL (GLFW, GLAD)
	*/
	if (!glfwInit())
	{
#ifdef _DEBUG
		std::cout << "failed to initialize glfw" << std::endl;
#endif
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int monitorX, monitorY;
	glfwGetMonitorPos(glfwGetPrimaryMonitor(), &monitorX, &monitorY);

	const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	WINDOWWIDTH = (unsigned)(videoMode->width * 0.825f); WINDOWHEIGHT = (unsigned)(videoMode->height * 0.825f);
	ratioW = WINDOWWIDTH / 1584.0f;
	ratioH = WINDOWHEIGHT / 891.0f;

#ifdef _DEBUG
	std::cout << "window resolution: " << WINDOWWIDTH << "x" << WINDOWHEIGHT << std::endl;
#endif

	GLFWwindow* window = glfwCreateWindow(WINDOWWIDTH, WINDOWHEIGHT, "ValoGuessr", nullptr, nullptr);
	if (!window)
	{
#ifdef _DEBUG
		std::cout << "failed to initialize window" << std::endl;
#endif
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowFocusCallback(window, window_focus_callback);
	glfwSetWindowIconifyCallback(window, window_iconify_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSwapInterval(1); // vsync on
	glfwSetWindowSizeLimits(window, (int)(0.55f * WINDOWWIDTH), (int)(0.55f * WINDOWHEIGHT), GLFW_DONT_CARE, GLFW_DONT_CARE);
	glfwSetWindowPos(window, monitorX + (videoMode->width - WINDOWWIDTH) / 2, monitorY + (videoMode->height - WINDOWHEIGHT) / 2);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
#ifdef _DEBUG
		std::cout << "failed to initialize glad" << std::endl;
#endif
		return -1;
	}

	// necessary opengl flags
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// setting logo in new scope to deallocate GLFWimage
	{
		GLFWimage image;
#ifdef _DEBUG
		image.pixels = stbi_load(".\\images\\textures\\icon.png", &image.width, &image.height, 0, 4);
#else
		image.pixels = stbi_load(".\\..\\images\\textures\\icon.png", &image.width, &image.height, 0, 4);
#endif
		glfwSetWindowIcon(window, 1, &image);
		stbi_image_free(image.pixels);
	}

	/*
		Setting up UI elements
	*/
	stbi_set_flip_vertically_on_load(true);

	UIElement::WINDOWWIDTH = &WINDOWWIDTH;
	UIElement::WINDOWHEIGHT = &WINDOWHEIGHT;

	GM::getInstance().init();
#ifdef _DEBUG
	Yui::getInstance().init(".\\fonts\\valorant.ttf", &ratioW, &ratioH, 130);
#else
	Yui::getInstance().init(".\\..\\fonts\\valorant.ttf", &ratioW, &ratioH, 130);
#endif

	if (std::filesystem::exists(PATH_PREFIX.append("config\\config.valog").c_str()))
		Yui::getInstance().loadScene(0);
	else
		Yui::getInstance().loadScene(10);

	while (!glfwWindowShouldClose(window))
	{
		// background
		glClearColor(0.059f, 0.094f, 0.133f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (focused)
			processInput(window);

        Yui::getInstance().renderAll();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/*
		Cleanup
	*/
	GM::getInstance().cleanup();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || Yui::getInstance().toClose)
		glfwSetWindowShouldClose(window, true);

	glfwGetCursorPos(window, &GM::getInstance().mouseX, &GM::getInstance().mouseY);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (!prevClick && !clicking)
			clicking = true;
		else
			clicking = false;

		prevClick = true;
	}
	else
	{
		prevClick = false;
		clicking = false;
	}

	for (UIElement* element : Yui::getInstance().UIElements)
	{
		Button* bp = dynamic_cast<Button*>(element);
		if (bp && bp->isHovering((float)GM::getInstance().mouseX, (float)GM::getInstance().mouseY))
		{
			if (clicking && Yui::getInstance().playable)
			{
				bool toChange = bp->changesScene; // need to copy value because onClick may deallocate bp
				bp->onClick();

				if (toChange)
					break;
			}
		}
	}
}
