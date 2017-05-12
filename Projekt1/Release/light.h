#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include "constants.h"

const static int NR_SHADOW = 4;
static GLuint depthMapFBO[NR_SHADOW];
static GLuint depthCubemap[NR_SHADOW];

class Light
{

public:
	const static int NR_LIGHTS = 32;
	glm::vec3 position;
	glm::vec3 color;
	string name;
	GLfloat linear;
	GLfloat quadratic;
	float distance_to_player;
	char visible = 1;
	static const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;


	Light(std::string const & name, glm::vec3 position, glm::vec3 color, GLfloat linear = 0.7, GLfloat quadratic = 1.8)
	{
		this->name = name;
		this->position = position;
		this->color = color;
		this->linear = linear;
		this->quadratic = quadratic;
	}

	static bool sort_distance(const Light * s1, const Light * s2)
	{
		return s1->distance_to_player < s2->distance_to_player;
	}


	static void configure_lights()
	{
		// Configure depth map FBO
		glGenFramebuffers(NR_LIGHTS, depthMapFBO);
		// Create depth cubemap texture
		glGenTextures(NR_LIGHTS, depthCubemap);

		for (int j = 0; j < NR_SHADOW; j++) {
			glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap[j]);
			for (GLuint i = 0; i < 6; ++i)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			// Attach cubemap as depth map FBO's color buffer
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[j]);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap[j], 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer not complete!" << std::endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

		}


	}



	static void render_cubemap(Shader simpleDepthShader, int j, std::vector<Light*> & lights)
	{
		GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj * glm::lookAt(lights[j]->position, lights[j]->position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lights[j]->position, lights[j]->position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lights[j]->position, lights[j]->position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lights[j]->position, lights[j]->position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lights[j]->position, lights[j]->position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lights[j]->position, lights[j]->position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

		// 1. Render scene to depth cubemap
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[j]);
		glClear(GL_DEPTH_BUFFER_BIT);
		simpleDepthShader.Use();
		for (GLuint i = 0; i < 6; ++i)
			glUniformMatrix4fv(glGetUniformLocation(simpleDepthShader.Program, ("shadowTransforms[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(shadowTransforms[i]));
		glUniform1f(glGetUniformLocation(simpleDepthShader.Program, "far_plane"), far);
		glUniform3fv(glGetUniformLocation(simpleDepthShader.Program, "lightPos"), 1, &lights[j]->position[0]);
	}


	static void render_shadows(Shader shader, GLuint gBuffer, glm::mat4 & view, GLfloat & Zoom, glm::vec3 Position, std::vector<Light*> & lights)
	{

		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.Use();
		glm::mat4 projection = glm::perspective(Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniform3fv(glGetUniformLocation(shader.Program, "viewPos"), 1, &Position[0]);
		glUniform1f(glGetUniformLocation(shader.Program, "far_plane"), far);
		glUniform1i(glGetUniformLocation(shader.Program, "NR_SHADOWS"), NR_SHADOW);


		for (int i = 0; i < NR_SHADOW; i++)
		{
			glUniform3fv(glGetUniformLocation(shader.Program, ("lightPos[" + std::to_string(i) + "]").c_str()), 1, &lights[i]->position[0]);
			glUniform3fv(glGetUniformLocation(shader.Program, ("lightColor[" + std::to_string(i) + "]").c_str()), 1, &lights[i]->color[0]);
			glActiveTexture(GL_TEXTURE2+i);
			glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap[i]);
		}


		

	}


};
