#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <map>
#include <string>
#include <fstream>

#ifdef _DEBUG
#include <iostream>
#endif

class Shader
{
private:
	// internal
	GLuint shaderID;

public:
	// universal members
	std::map<const char*, GLint> uniforms;

	Shader(const char* vertexPath, const char* fragPath, bool verbose);
	~Shader();

	// universal methods
	// binds shader
	void use();

	// unbinds shader
	void unbind();

	// deletes shader
	void deleteShader();

	// uniform methods
	GLint getUniformLocation(const char* uniformName);
	void setBool(const char* uniformName, GLboolean value);
	void setInt(const char* uniformName, GLint value);
	void setFloat(const char* uniformName, GLfloat value);
	void set3fv(const char* uniformName, GLsizei count, const GLfloat* value);
	void set4fv(const char* uniformName, GLsizei count, const GLfloat* value);
	void setMatrix3fv(const char* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value);
	void setMatrix4fv(const char* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value);
};