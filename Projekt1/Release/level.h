#pragma once
#include "object.h"
#include "light.h"
#include "dungeon.h"
#include "bot.h"


class Level
{
public:
	static Level *selected_level;
	std::map<std::string, std::vector<Object*>> objects;
	std::vector<Light*> lights;
	Dungeon * dungeon;
	std::vector<Bot*> bots;
	std::vector<prop_type> prop_types;
	std::vector<shot_path> shots;
	

	void add_object(Object * new_object)
	{
		objects[new_object->model->path].push_back(new_object);
	}

	void draw_lights(Shader shader)
	{

		const int minimum = Light::NR_LIGHTS < lights.size() ? Light::NR_LIGHTS : lights.size();


		for (GLuint i = 0; i < minimum; i++)
		{
			glUniform3fv(glGetUniformLocation(shader.Program, ("lights[" + std::to_string(i) + "].Position").c_str()), 1, &Level::selected_level->lights[i]->position[0]);
			glUniform3fv(glGetUniformLocation(shader.Program, ("lights[" + std::to_string(i) + "].Color").c_str()), 1, &Level::selected_level->lights[i]->color[0]);
			// Update attenuation parameters and calculate radius
			const GLfloat constant = 1.0; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
			GLfloat linear = Level::selected_level->lights[i]->linear;
			GLfloat quadratic = Level::selected_level->lights[i]->quadratic;
			glUniform1f(glGetUniformLocation(shader.Program, ("lights[" + std::to_string(i) + "].Linear").c_str()), linear);
			glUniform1f(glGetUniformLocation(shader.Program, ("lights[" + std::to_string(i) + "].Quadratic").c_str()), quadratic);
			// Then calculate radius of light volume/sphere
			const GLfloat lightThreshold = 1.0; // 5 / 256
			const GLfloat maxBrightness = std::fmaxf(std::fmaxf(Level::selected_level->lights[i]->color.r, Level::selected_level->lights[i]->color.g), Level::selected_level->lights[i]->color.b);
			GLfloat radius = (-linear + static_cast<float>(std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0 / lightThreshold) * maxBrightness)))) / (2 * quadratic);
			glUniform1f(glGetUniformLocation(shader.Program, ("lights[" + std::to_string(i) + "].Radius").c_str()), radius);
		}
		

	}

	void draw_objects(Shader shader, char fr = 0)
	{
		for (auto it = objects.begin(); it != objects.end(); ++it) {
			if (it->second.size() > 0) {
				char fo = it->second.front()->forward_rendering;
				if (((fo == 1 || fo == 2) && fr == 1) || (fr == 0 && fo == 0) || (fr == 2 && (fo == 0 || fo == 2))) {
					const int a = it->second.front()->model->meshes.size();
					const int b = it->second.front()->model->number_of_visible_objects;
					for (int i = 0; i < a; i++) {
						it->second.front()->model->meshes[i].Load(shader);
						for (int j = 0; j < b; j++) {
							if (fo == 1 || fo == 2)
								glUniform3fv(glGetUniformLocation(shader.Program, "lightColor"), 1, &it->second[j]->color[0]);
							glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(it->second[j]->model_mat));
							it->second.front()->model->meshes[i].Draw();
						}
						it->second.front()->model->meshes[i].Unload(shader);
					}


				}
			}
		}
	}



};
