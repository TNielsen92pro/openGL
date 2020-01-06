#include "setHandler.h"
#include <algorithm>
#include <iostream>
#include <vector>

setHandler::setHandler(GLuint inputwidth, GLuint inputheight) {
	width = inputwidth;
	height = inputheight;
	glGenFramebuffers(1, &densityBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, densityBuffer);
	densityField = new Texture2D(
		width, 
		height, 
		GL_RGB32F, 
		GL_RGB, 
		GL_FLOAT, 
		nullptr
	);
	std::cout << densityField << std::endl;
	densityField->set_min_filter(GL_NEAREST);
	densityField->set_mag_filter(GL_NEAREST);
	densityField->bind();
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, 
		GL_COLOR_ATTACHMENT0, 
		GL_TEXTURE_2D, 
		densityField->get_location(), 
		0
	);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Density buffer done goofed" << std::endl;
		exit(1);
	} 

	glGenFramebuffers(1, &combinedBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, combinedBuffer);
	combinedTexture = new Texture2D(
		width, 
		height, 
		GL_RGBA32F, 
		GL_RGBA, 
		GL_FLOAT, 
		nullptr
	);
	combinedTexture->set_min_filter(GL_NEAREST);
	combinedTexture->set_mag_filter(GL_NEAREST);
	combinedTexture->bind();
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, 
		GL_COLOR_ATTACHMENT0, 
		GL_TEXTURE_2D, 
		combinedTexture->get_location(), 
		0
	);
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Combined buffer done goofed" << std::endl;
		exit(1);
	}


	// quad rectangle
	float rectangleVertices[] = {
	 1.0f,  1.0f, 0.0f,  // top right
	 1.0f, -1.0f, 0.0f,  // bottom right
	-1.0f, -1.0f, 0.0f,  // bottom left
	-1.0f,  1.0f, 0.0f   // top left 
	};

	unsigned int rectangleIndices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW);
	glBindVertexArray(0);

	ourShader = new Shader("shader_v.vs", "shader_f.fs");
	computeShader = new Shader("shader_c.fs");
	imageShader = new Shader("imageDisplay.vs", "imageDisplay.fs");
	
}

void setHandler::setPoints(std::vector<GLfloat>& data) {
	if (ssbo != 0) { 
		glDeleteBuffers(1, &ssbo);
	}
	ssboLength = data.size();
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(
		GL_SHADER_STORAGE_BUFFER, 
		sizeof(GLfloat) * data.size(), 
		data.data(), 
		GL_STATIC_COPY
	);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void setHandler::renderImage(GLfloat color[3]) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, densityBuffer);
	glDisable(GL_BLEND);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ourShader->use();
	ourShader->setInt("ssbolength", ssboLength);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);

	glBindVertexArray(quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glViewport(0, 0, width, height);

	densityField->bind();
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, pix_buffer);
	float max_density = std::numeric_limits<float>::min();
	for (int x = 0; x < 800; x++) {
		for (int y = 0; y < 600; y++) {
			int idx = y * (800 * 3) + x;
			max_density = std::max(max_density, pix_buffer[idx]);
		}
	}
	// Draw to render texture
	glBindFramebuffer(GL_FRAMEBUFFER, combinedBuffer);
	
	glEnable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
	imageShader->use();
	imageShader->setFloat("max_density", max_density);
	imageShader->setVec3("color", color[0], color[1], color[2]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, densityField->get_location());

	glBindVertexArray(quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
}  
void setHandler::render(std::vector<GLfloat>& dots, GLfloat color[3]) {
	setPoints(dots);
	renderImage(color);
}
