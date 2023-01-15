#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#ifdef _DEBUG
#include <iostream>
#endif

class Texture
{
private:
	// internal
	GLuint textureID;

public:
	Texture(const char* imagePath, bool alpha);
	~Texture();

	// universal methods
	// binds texture
	void bindTexture();

	// unbinds texture
	void unbindTexture();

	// deletes texture
	void deleteTexture();

	// changes image data of this texture
	void replaceTexture(const char* imagePath, bool alpha);
};

