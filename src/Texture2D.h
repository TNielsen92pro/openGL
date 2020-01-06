#pragma once

#ifndef TEXTURE2D_H
#define TEXTURE2D_H

//#include <GL/glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <string>

class Texture2D {
private:
	GLuint gl_location_;

	int width_, height_;
	GLuint mipmaps_;
	GLuint gl_internal_format_, gl_format_, gl_type;

public:
	Texture2D(int width, int height, GLuint gl_internal_format, GLuint gl_format,
		GLuint gl_type, void* data);

	~Texture2D();

	// Binds the texture without making it active
	void bind();
	// Activates the texture and binds it
	void bind(GLuint texture_location);
	void unbind();

	void set_wrap_s(GLuint wrap);
	void set_wrap_t(GLuint wrap);
	void set_min_filter(GLuint filter);
	void set_mag_filter(GLuint filter);

	int get_width() { return width_; }
	int get_height() { return height_; }
	int get_num_mipmaps() { return mipmaps_; }

	GLuint get_location() { return gl_location_; };
};

#endif
