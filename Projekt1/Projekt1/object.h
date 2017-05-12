#pragma once
#include <string>
#include <glm\glm.hpp>
#include "common/model.h"
#include "common/shader.h"

class Object {
public:
	string name;
	Model *model;
	glm::vec3 translate;
	glm::vec3 rotate;
	glm::vec3 scale;
	glm::mat4 model_mat;
	char update_model = 1;
	char visible = 1;
	char forward_rendering = 0;
	glm::vec3 color;

	Object(std::string const & path, std::string const & nazwa, glm::vec3 start = glm::vec3(0,0,0))
	{
		model = Model::getModel(path);
		translate = start;
		rotate = glm::vec3(0, 0, 0);
		scale = glm::vec3(1, 1, 1);
		name = nazwa;
		UpdatePosition();
	}

	Object(std::string const & path, std::string const & nazwa, glm::vec3 start, glm::vec3 skala, glm::vec3 rotacja, char forward_rendering = 0, glm::vec3 color = glm::vec3(1.0,1.0,1.0))
	{
		this->color = color;
		this->forward_rendering = forward_rendering;
		model = Model::getModel(path);
		translate = start;
		rotate = rotacja;
		scale = skala;
		name = nazwa;
		UpdatePosition();
	}

	void Draw(Shader shader)
	{
		if (update_model)
			UpdatePosition();
		model->Draw(shader);
	}

	static bool sort_visible(const Object * s1, const Object * s2)
	{
		return s1->visible > s2->visible;
	}


	void UpdatePosition()
	{
		model_mat = glm::mat4();
		model_mat = glm::translate(model_mat, translate);
		model_mat = glm::rotate(model_mat, rotate.x, glm::vec3(1, 0, 0));
		model_mat = glm::rotate(model_mat, rotate.y, glm::vec3(0, 1, 0));
		model_mat = glm::rotate(model_mat, rotate.z, glm::vec3(0, 0, 1));
		model_mat = glm::scale(model_mat, scale);
		update_model = 0;
	}
	


};



