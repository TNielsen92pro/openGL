#pragma once

#ifndef SETHANDLER_H
#define SETHANDLER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Texture2D.h"
#include "Shader.h"


class setHandler {



private:
	GLuint densityBuffer = 0;
	GLuint combinedBuffer = 0;
	Texture2D* densityField;
	Texture2D* combinedTexture;

	// Points
	GLuint ssbo = 0;
	GLuint ssboLength = 0;

	// Quad
	unsigned int quadVAO;

	int width;
	int height;

	// Pixel buffer to get max value
	GLfloat* pix_buffer = new GLfloat[800 * 600 * 3];

	// Shaders
	Shader* ourShader;
	Shader* computeShader;
	Shader* imageShader;
	void setPoints(std::vector<GLfloat>& data);
	void renderImage(GLfloat color[3]);
	void render();
public:
	setHandler(GLuint inputwidth, GLuint inputheight);
	void render(std::vector<GLfloat>& data, GLfloat color[3]);
	Texture2D* getTexture() { return combinedTexture; };

};








#endif