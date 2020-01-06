#include "Texture2D.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <iostream>



Texture2D::Texture2D(int width, int height, GLuint gl_internal_format, GLuint gl_format,
	GLuint gl_type, void* data) {
	width_ = width;
	height_ = height;

	glGenTextures(1, &gl_location_);
	glBindTexture(GL_TEXTURE_2D, gl_location_);

	glTexImage2D(GL_TEXTURE_2D, 0, gl_internal_format, width,
		height, 0, gl_format, gl_type, data);

	unbind();
}

Texture2D::~Texture2D() {
	glDeleteTextures(1, &gl_location_);
}

void Texture2D::bind() {
	glBindTexture(GL_TEXTURE_2D, gl_location_);
}

void Texture2D::bind(GLuint texture_location) {
	glActiveTexture(GL_TEXTURE0 + texture_location);
	bind();
}

void Texture2D::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::set_wrap_s(GLuint wrap) {
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	unbind();
}

void Texture2D::set_wrap_t(GLuint wrap) {
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	unbind();
}
void Texture2D::set_min_filter(GLuint filter) {
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	unbind();
}

void Texture2D::set_mag_filter(GLuint filter) {
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	unbind();
}


