#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include "object.h"
#include "shared.h"
#include "constants.h"
#include "level.h"
#include <ctime>

class Editor {
public:
	std::vector<Object*> * objects;
	inline void editor();
	char enabled = 1;
	float m_x = 0;
	float m_y = 0;
	float m_z = 0;
	float s_x = 0;
	float s_y = 0;
	float s_z = 0;
	float k_x = 0;
	float k_y = 0;
	float k_z = 0;
	float ka_x = 0;
	float ka_y = 0;
	float ka_z = 0;


	Object * selected_object = NULL;
	Light * selected_light = NULL;

	inline void add_object() {
		std::vector<std::string> v;
		int i = 0;
			for (auto it = Model::models.begin(); it != Model::models.end(); ++it,i++) {
				v.push_back(it->first);
				std::cout << i << ") " << it->first << "\n";
			}

		std::cout << "\nn jesli nie ma\n";
		std::cout << "q aby anulowac\n";

		std::string temp;
		std::cin >> temp;
		if (temp.c_str()[0] == 'q')
			return;

		std::string path;

		if (temp.c_str()[0] == 'n')
		{
			std::cout << "wpisz scieszkê modelu\n";
			std::cin >> path;

		}
		else
		{
			int i1 = std::stoi(temp);
			path = v[i1];
		}

		std::string name;
		std::cout << "wpisz nazwe modelu\n";
		std::cin >> name;
		Object * aaa;
		if (selected_object != NULL)
			aaa = new Object(path, name, selected_object->translate, selected_object->scale, selected_object->rotate);
		else
			aaa = new Object(path, name);
		(*objects).push_back(aaa);
		selected_object = aaa;
		selected_light = NULL;
	}


	inline void edit_object()
	{
		int i = 0;
		for (i; i < (*objects).size(); i++)
		{
			std::cout << i << ")" << (*objects)[i]->name << "\n";
		}
		i = i + (*objects).size();
		for(i;i<Level::selected_level->lights.size()+(*objects).size();i++)
		{
			std::cout << i << ")(swiatlo)" << Level::selected_level->lights[i- (*objects).size()]->name << "\n";
		}

		std::cout << "\nq aby anulowac\n";
		std::string temp;
		std::cin >> temp;

		if (temp.c_str()[0] == 'q')
			return;

		int a = std::stoi(temp);
		if (a < (*objects).size())
		{
			selected_object = (*objects)[a];
			selected_light = NULL;
		}
		else
		{
			selected_object = NULL;
			selected_light = Level::selected_level->lights[a -(*objects).size()];
		}

	}

	inline void remove_object()
	{
		int i = 0;
		for (i; i < (*objects).size(); i++)
		{
			std::cout << i << ")" << (*objects)[i]->name << "\n";
		}
		i = i + (*objects).size();
		for (i; i < Level::selected_level->lights.size() + (*objects).size(); i++)
		{
			std::cout << i << ")(swiatlo)" << Level::selected_level->lights[i - (*objects).size()]->name << "\n";
		}

		std::cout << "\nq aby anulowac\n";
		std::string temp;
		std::cin >> temp;

		if (temp.c_str()[0] == 'q')
			return;

		int a = std::stoi(temp);
		if (a < (*objects).size())
		{
			if (selected_object == (*objects)[a])
				selected_object = NULL;
			(*objects).erase((*objects).begin() + a);
		}
		else
		{
			if (selected_light == Level::selected_level->lights[a- (*objects).size()])
				selected_light = NULL;
			Level::selected_level->lights.erase(Level::selected_level->lights.begin() + a - (*objects).size());
		}

	}

	inline void modify_object()
	{
		for (int i = 0; i < (*objects).size(); i++)
		{
			std::cout << i << ")" << (*objects)[i]->name << "\n";
		}

		std::cout << "\nq aby anulowac\n";
		std::string temp;
		std::cin >> temp;

		if (temp.c_str()[0] == 'q')
			return;

		int a = std::stoi(temp);
		selected_object = (*objects)[a];
		std::cout << "\nPodaj translate->x : ";
		std::cin >> (*objects)[a]->translate.x;
		std::cout << "Podaj translate->y : ";
		std::cin >> (*objects)[a]->translate.y;
		std::cout << "Podaj translate->z : ";
		std::cin >> (*objects)[a]->translate.z;
		std::cout << "Podaj scale->x : ";
		std::cin >> (*objects)[a]->scale.x;
		std::cout << "Podaj scale->y : ";
		std::cin >> (*objects)[a]->scale.y;
		std::cout << "Podaj scale->z : ";
		std::cin >> (*objects)[a]->scale.z;
		std::cout << "Podaj rotate->x : ";
		std::cin >> (*objects)[a]->rotate.x;
		std::cout << "Podaj rotate->y : ";
		std::cin >> (*objects)[a]->rotate.y;
		std::cout << "Podaj rotate->z : ";
		std::cin >> (*objects)[a]->rotate.z;
		std::cout << "\n";
	}

	inline void add_light()
	{
		Light * aaa;
		std::string name;
		glm::vec3 position;
		glm::vec3 color;
		GLfloat linear;
		GLfloat quadratic;
		std::cout << "Wpisz nazwe swiatla\n";
		std::cin >> name;
		std::cout << "\nPodaj position.x : ";
		std::cin >> position.x;
		std::cout << "Podaj position.y : ";
		std::cin >> position.y;
		std::cout << "Podaj position.z : ";
		std::cin >> position.z;
		std::cout << "Podaj color->x : ";
		std::cin >> color.x;
		std::cout << "Podaj color.y : ";
		std::cin >> color.y;
		std::cout << "Podaj color.z : ";
		std::cin >> color.z;
		std::cout << "Podaj linear : ";
		std::cin >> linear;
		std::cout << "Podaj quadratic : ";
		std::cin >> quadratic;
		aaa = new Light(name, position, color, linear, quadratic);
		Level::selected_level->lights.push_back(aaa);
		selected_light = aaa;
		selected_object = NULL;

	}

	inline void save()
	{
		std::fstream plik;
		plik.open("dane.txt", std::ios::out | std::ios::trunc);
		plik << (*objects).size() << "\n";
		for (int i = 0; i < (*objects).size(); i++)
		{
			plik << (*objects)[i]->name << "\n";
			plik << (*objects)[i]->model->path << "\n";
			plik << (*objects)[i]->translate.x << "\n";
			plik << (*objects)[i]->translate.y << "\n";
			plik << (*objects)[i]->translate.z << "\n";
			plik << (*objects)[i]->scale.x << "\n";
			plik << (*objects)[i]->scale.y << "\n";
			plik << (*objects)[i]->scale.z << "\n";
			plik << (*objects)[i]->rotate.x << "\n";
			plik << (*objects)[i]->rotate.y << "\n";
			plik << (*objects)[i]->rotate.z << "\n";
		}
		plik << Level::selected_level->lights.size() << "\n";
		for (int i = 0; i <  Level::selected_level->lights.size(); i++)
		{
			plik << Level::selected_level->lights[i]->name << "\n";
			plik << Level::selected_level->lights[i]->position.x << "\n";
			plik << Level::selected_level->lights[i]->position.y << "\n";
			plik << Level::selected_level->lights[i]->position.z << "\n";
			plik << Level::selected_level->lights[i]->color.x << "\n";
			plik << Level::selected_level->lights[i]->color.y << "\n";
			plik << Level::selected_level->lights[i]->color.z << "\n";
		}
		plik.close();
	}

	inline void load()
	{
		std::fstream plik;
		plik.open("dane.txt", std::ios::in);
		if (!(*objects).empty())
			(*objects).clear();
		int k;
		plik >> k;
		string name;
		string path;
		glm::vec3 translate;
		glm::vec3 scale;
		glm::vec3 rotate;
		for (int i = 0; i < k; i++)
		{
			plik >> name;
			plik >> path;
			plik >> translate.x;
			plik >> translate.y;
			plik >> translate.z;
			plik >> scale.x;
			plik >> scale.y;
			plik >> scale.z;
			plik >> rotate.x;
			plik >> rotate.y;
			plik >> rotate.z;
			Object * aaa;
			aaa = new Object(path, name, translate, scale, rotate);
			(*objects).push_back(aaa);
		}
		if (!Level::selected_level->lights.empty())
			Level::selected_level->lights.clear();
		plik >> k;
		glm::vec3 position;
		glm::vec3 color;
		GLfloat linear;
		GLfloat quadratic;
		for (int i = 0; i < k; i++)
		{
			plik >> name;
			plik >> position.x;
			plik >> position.y;
			plik >> position.z;
			plik >> color.x;
			plik >> color.y;
			plik >> color.z;
			plik >> linear;
			plik >> quadratic;
			Light * aaa;
			aaa = new Light(name, position, color, linear, quadratic);
			Level::selected_level->lights.push_back(aaa);
		}
		plik.close();
	}


	void keyboard_callback(int action,int key){
		if (action == GLFW_PRESS)
		{
			if (key == GLFW_KEY_KP_ADD)
			{
				s_x = 1;
				s_y = 1;
				s_z = 1;
			}
			if (key == GLFW_KEY_KP_SUBTRACT)
			{
				s_x = -1;
				s_y = -1;
				s_z = -1;
			}
			if (key == GLFW_KEY_LEFT) m_x = -1;
			if (key == GLFW_KEY_RIGHT) m_x = 1;
			if (key == GLFW_KEY_UP) m_y = 1;
			if (key == GLFW_KEY_DOWN) m_y = -1;
			if (key == GLFW_KEY_O) m_z = -1;
			if (key == GLFW_KEY_L) m_z = 1;
			if (key == GLFW_KEY_Y) s_x = 1;
			if (key == GLFW_KEY_H) s_x = -1;
			if (key == GLFW_KEY_U) s_y = 1;
			if (key == GLFW_KEY_J) s_y = -1;
			if (key == GLFW_KEY_I) s_z = 1;
			if (key == GLFW_KEY_K) s_z = -1;
			if (key == GLFW_KEY_X) k_x = -1;
			if (key == GLFW_KEY_C) k_x = 1;
			if (key == GLFW_KEY_V) k_y = -1;
			if (key == GLFW_KEY_B) k_y = 1;
			if (key == GLFW_KEY_N) k_z = -1;
			if (key == GLFW_KEY_M) k_z = 1;
			if (key == GLFW_KEY_ENTER)
			{
				if (selected_object != NULL)
					printf("\n%f\tplolzenie x\n%f\tpolozenie y\n%f\tpolozenie z\n%f\tskala x\n%f\tskala y\n%f\tskala z\n%f\tkat x\n%f\tkat y\n%f\tkat z\n", selected_object->translate.x, selected_object->translate.y, selected_object->translate.z, selected_object->scale.x, selected_object->scale.y, selected_object->scale.z, selected_object->rotate.x, selected_object->rotate.y, selected_object->rotate.z);
				else if (selected_light != NULL)
					printf("\n%f\tplolzenie x\n%f\tpolozenie y\n%f\tpolozenie z\n%f\tkolor x\n%f\tkolor y\n%f\tkolor z\n%f\tlinear\n%f\tquadratic\n", selected_light->position.x, selected_light->position.y, selected_light->position.z, selected_light->color.x, selected_light->color.y, selected_light->color.z, selected_light->linear, selected_light->quadratic);
			}
			if (key == GLFW_KEY_Q)
			{
				show_editor();
			}
			if (key == GLFW_KEY_E)
			{
				srand(time(NULL));
			
			}
		}
		else if (action == GLFW_RELEASE)
		{
			if (key == GLFW_KEY_KP_ADD)
			{
				s_x = 0;
				s_y = 0;
				s_z = 0;
			}
			if (key == GLFW_KEY_KP_SUBTRACT)
			{
				s_x = 0;
				s_y = 0;
				s_z = 0;
			}
			if (key == GLFW_KEY_LEFT) m_x = 0;
			if (key == GLFW_KEY_RIGHT) m_x = 0;
			if (key == GLFW_KEY_UP) m_y = 0;
			if (key == GLFW_KEY_DOWN) m_y = 0;
			if (key == GLFW_KEY_O) m_z = 0;
			if (key == GLFW_KEY_L) m_z = 0;
			if (key == GLFW_KEY_Y) s_x = 0;
			if (key == GLFW_KEY_H) s_x = 0;
			if (key == GLFW_KEY_U) s_y = 0;
			if (key == GLFW_KEY_J) s_y = 0;
			if (key == GLFW_KEY_I) s_z = 0;
			if (key == GLFW_KEY_K) s_z = 0;
			if (key == GLFW_KEY_X) k_x = 0;
			if (key == GLFW_KEY_C) k_x = 0;
			if (key == GLFW_KEY_V) k_y = 0;
			if (key == GLFW_KEY_B) k_y = 0;
			if (key == GLFW_KEY_N) k_z = 0;
			if (key == GLFW_KEY_M) k_z = 0;
		}
	}




	void move_object(float deltaTime){
		if (selected_object != NULL)
		{
			selected_object->translate.x = selected_object->translate.x + deltaTime*m_x;
			selected_object->translate.y = selected_object->translate.y + deltaTime*m_y;
			selected_object->translate.z = selected_object->translate.z + deltaTime*m_z;
			selected_object->scale.x = selected_object->scale.x + deltaTime*s_x;
			selected_object->scale.y = selected_object->scale.y + deltaTime*s_y;
			selected_object->scale.z = selected_object->scale.z + deltaTime*s_z;
			selected_object->rotate.x = selected_object->rotate.x + deltaTime*k_x;
			if (selected_object->rotate.x > 2 * pi)
				selected_object->rotate.x = 0;
			else if (selected_object->rotate.x < 0)
				selected_object->rotate.x = 2 * pi;
			selected_object->rotate.y = selected_object->rotate.y + deltaTime*k_y;
			if (selected_object->rotate.y > 2 * pi)
				selected_object->rotate.y = 0;
			else if (selected_object->rotate.y < 0)
				selected_object->rotate.y = 2 * pi;
			selected_object->rotate.z = selected_object->rotate.z + deltaTime*k_z;
			if (selected_object->rotate.z > 2 * pi)
				selected_object->rotate.z = 0;
			else if (selected_object->rotate.z < 0)
				selected_object->rotate.z = 2 * pi;
		}
		else if(selected_light != NULL)
		{
			selected_light->position.x = selected_light->position.x + deltaTime*m_x;
			selected_light->position.y = selected_light->position.y + deltaTime*m_y;
			selected_light->position.z = selected_light->position.z + deltaTime*m_z;
			selected_light->color.x = selected_light->color.x + deltaTime*s_x;
			selected_light->color.y = selected_light->color.y + deltaTime*s_y;
			selected_light->color.z = selected_light->color.z + deltaTime*s_z;
			selected_light->linear = selected_light->linear + deltaTime*k_x;
			selected_light->quadratic = selected_light->quadratic+ deltaTime*k_y;
		}
		
	}


	inline void show_editor() {
		auto objects = &Level::selected_level->objects;
		std::cout << "1)Dodaj nowy objekt.\n";
		std::cout << "2)Edytuj objekt.\n";
		std::cout << "3)Usun objekt.\n";
		std::cout << "4)Modyfikuj objekt.\n";
		std::cout << "5)Dodaj switalo.\n";
		std::cout << "6)Zapisz.\n";
		std::cout << "7)Wczytaj.\n";

		int a;
		std::cin >> a;

		switch (a)
		{
		case 1:
			add_object();
			break;
		case 2:
			edit_object();
			break;
		case 3:
			remove_object();
			break;
		case 4:
			modify_object();
			break;
		case 5:
			add_light();
			break;
		case 6:
			save();
			break;
		case 7:
			load();
			break;

		}
	}

};
