#pragma once

#include "common/camera.h"
#include "level.h"
#include "physic.h"


class Player : public Camera
{
public:
	int x = 0;
	int z = 0;
	float y = 3 * player_height / 4;
	bool jump = false;
	float land_y = 3 * player_height / 4;
	int hp = 100;
	Object * player_model;

	Player(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : Camera(position, up, yaw, pitch)
	{

	};

	Player(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Camera(posX, posY, posZ, upX, upY, upZ, yaw, pitch)
	{
		
	};

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
	{
		glm::vec3 new_position;
		GLfloat velocity = this->MovementSpeed * deltaTime;
		if (direction == FORWARD)
			new_position = this->Position + this->Move_front * velocity;
		if (direction == BACKWARD)
			new_position = this->Position - this->Move_front * velocity;
		if (direction == LEFT)
			new_position = this->Position - this->Move_right * velocity;
		if (direction == RIGHT)
			new_position = this->Position + this->Move_right * velocity;
		land_y = 3 * player_height / 4;
		new_position = collision(this->Position, new_position, Level::selected_level->dungeon->map[z][x].props, &land_y, &y, jump, &hp, &Level::selected_level->objects);
		if (x > 0)
			new_position = collision(this->Position, new_position, Level::selected_level->dungeon->map[z][x - 1].props, &land_y, &y, jump, &hp, &Level::selected_level->objects);
		if (x > 0 && z > 0)
			new_position = collision(this->Position, new_position, Level::selected_level->dungeon->map[z - 1][x - 1].props, &land_y, &y, jump, &hp, &Level::selected_level->objects);
		if (x > 0 && z < Level::selected_level->dungeon->width * 2 - 2)
			new_position = collision(this->Position, new_position, Level::selected_level->dungeon->map[z + 1][x - 1].props, &land_y, &y, jump, &hp, &Level::selected_level->objects);
		if (x <  Level::selected_level->dungeon->height * 2 - 2)
			new_position = collision(this->Position, new_position, Level::selected_level->dungeon->map[z][x + 1].props, &land_y, &y, jump, &hp, &Level::selected_level->objects);
		if (x <  Level::selected_level->dungeon->height * 2 - 2 && z > 0)
			new_position = collision(this->Position, new_position, Level::selected_level->dungeon->map[z - 1][x + 1].props, &land_y, &y, jump, &hp, &Level::selected_level->objects);
		if (x <  Level::selected_level->dungeon->height * 2 - 2 && z < Level::selected_level->dungeon->width * 2 - 2)
			new_position = collision(this->Position, new_position, Level::selected_level->dungeon->map[z + 1][x + 1].props, &land_y, &y, jump, &hp, &Level::selected_level->objects);
		if (z > 0)
			new_position = collision(this->Position, new_position, Level::selected_level->dungeon->map[z - 1][x].props, &land_y, &y, jump, &hp, &Level::selected_level->objects);
		if (z < Level::selected_level->dungeon->width * 2 - 2)
			new_position = collision(this->Position, new_position, Level::selected_level->dungeon->map[z + 1][x].props, &land_y, &y, jump, &hp, &Level::selected_level->objects);
		this->Position = new_position;
		if (jump == false)
			y = land_y;
	}

	void ProcessKeyboardAndDraw(Camera_Movement direction, GLfloat deltaTime)
	{
		ProcessKeyboard(direction, deltaTime);

		if (Position.x > (float)x*scale + scale / 2 - player_size)
		{
			int posZ = (int)(Position.z + scale / 2) / scale;
			if (x == (Level::selected_level->dungeon->height - 1) * 2 || Level::selected_level->dungeon->map[posZ][x + 1].type == 0)
				Position.x = (float)x*scale + scale / 2 - 0.5;
		}
		if (Position.x < (float)x*scale - scale / 2 + player_size)
		{
			int posZ = (int)(Position.z + scale / 2) / scale;
			if (x == 0 || Level::selected_level->dungeon->map[posZ][x - 1].type == 0)
				Position.x = (float)x*scale - scale / 2 + 0.5;
		}
		if (Position.z >(float)z*scale + scale / 2 - player_size)
		{
			int posX = (int)(Position.x + scale / 2) / scale;
			if (z == (Level::selected_level->dungeon->width - 1) * 2 || Level::selected_level->dungeon->map[z + 1][posX].type == 0)
				Position.z = (float)z*scale + scale / 2 - 0.5;
		}
		if (Position.z < (float)z*scale - scale / 2 + player_size)
		{
			int posX = (int)(Position.x + scale / 2) / scale;
			if (z == 0 || Level::selected_level->dungeon->map[z - 1][posX].type == 0)
				Position.z = (float)z*scale - scale / 2 + 0.5;
		}

		if ((int)((Position.x + scale / 2) / scale) != x || (int)((Position.z + scale / 2) / scale) != z)
		{
			x = (int)(Position.x + scale / 2) / scale;
			z = (int)(Position.z + scale / 2) / scale;
			if (Level::selected_level->dungeon != NULL)
			{
				calculate_visibility_of_objects(&Level::selected_level->objects);
				calculate_visibility_of_lights(&Level::selected_level->lights);


			}
		}
		player_model->translate = Position;
		player_model->translate.y = Position.y - 3.0 / 4 * player_height;
		player_model->UpdatePosition();
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= this->MouseSensitivity;
		yoffset *= this->MouseSensitivity;

		this->Yaw += xoffset;
		this->Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (this->Pitch > 89.0f)
				this->Pitch = 89.0f;
			if (this->Pitch < -89.0f)
				this->Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		this->updateCameraVectors();
		player_model->rotate.y = glm::radians(-Yaw-90);
		player_model->UpdatePosition();

	}

	void ProcessMouseButton()
	{
		Object *aaa;
		aaa = new Object("resources/objects/pewpewpew/untitled.obj", "pew", glm::vec3(Position.x,Position.y-0.1,Position.z), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0),1, glm::vec3(1.0, 1.0, 0.0));
		aaa->model_mat = glm::rotate(aaa->model_mat, glm::radians(-Yaw - 90), glm::vec3(0, 1, 0));
		
		aaa->model_mat = glm::rotate(aaa->model_mat, glm::radians(Pitch - 90), glm::vec3(1, 0, 0));
		
		aaa->model_mat = glm::scale(aaa->model_mat, glm::vec3(shot_radius, size_of_draw/2, shot_radius));
		
		Level::selected_level->objects[aaa->model->path].push_back(aaa);
		Level::selected_level->objects[aaa->model->path][0]->model->number_of_visible_objects = Level::selected_level->objects[aaa->model->path][0]->model->number_of_visible_objects + 1;
		Level::selected_level->shots.push_back(*(new shot_path(aaa)));
		for (int i = 0; i < Level::selected_level->bots.size(); i++)
		{
			if (sqrt((Position.x - Level::selected_level->bots[i]->Position.x)*(Position.x - Level::selected_level->bots[i]->Position.x) + (Position.z - Level::selected_level->bots[i]->Position.z)*(Position.z - Level::selected_level->bots[i]->Position.z)) < shooting_range)
			{
				if (sphere_collision(Position, Front, Level::selected_level->bots[i]->Position, Level::selected_level->bots[i]->Scale.x) && wall_collision(Position, Level::selected_level->bots[i]->Position, Level::selected_level->dungeon->map, x, z, Level::selected_level->bots[i]->x, Level::selected_level->bots[i]->z))
				{
					Bot * abc;
					abc = Level::selected_level->bots[i];
					abc->life--;
					if (abc->mode == WALKING)
					{
						abc->mode = AIMING;
						abc->aim_angle = calculate_angle(abc->Position, Position);
					}
					if (abc->life <= 0)
					{
						Level::selected_level->bots.erase(Level::selected_level->bots.begin() + i);
						for (int it = 0; it < Level::selected_level->objects[abc->object->model->path].size(); it++) {
							if (Level::selected_level->objects[abc->object->model->path][it] == abc->object)
							{
								Level::selected_level->objects[abc->object->model->path].erase(Level::selected_level->objects[abc->object->model->path].begin() + it);
								delete(abc);
								break;
							}
						}
					}
				}
			}
		}
	}

	void calculate_visibility_of_lights(std::vector<Light*> * lights) {
		const int size = lights->size();
		float x;
		float z;
		const float xx = this->x*scale;
		const float zz = this->z*scale;

		for (int i = 0; i < size; i++) {
			x = (*lights)[i]->position.x - xx;
			z = (*lights)[i]->position.z - zz;
			(*lights)[i]->distance_to_player = sqrt(x*x + z*z);
			(*lights)[i]->visible = 0;
		}

		std::sort(lights->begin(), lights->end(), Light::sort_distance);
		const int minimum = Light::NR_LIGHTS < lights->size() ? Light::NR_LIGHTS: lights->size();
		for(int i = 0;i<minimum;i++)
		{
			(*lights)[i]->visible = 1;
		}

	}


	void calculate_visibility_of_objects(std::map<std::string, std::vector<Object*>> * objects) {
		int a;
		int vis = 0;
		float x;
		float z;
		const float xx = this->x*scale;
		const float zz = this->z*scale;
		float distance;
		for (auto it = objects->begin(); it != objects->end(); ++it) {
			int vis = 0;
			a = it->second.size();
			if (a == 0)
				continue;
			it->second.front()->model->number_of_visible_objects = 0;
			for (int i = 0; i < a; i++)
			{
				x = it->second[i]->translate.x - xx;
				z = it->second[i]->translate.z - zz;
				distance = sqrt(x*x + z*z);
				if (distance < size_of_draw)
				{
					it->second[i]->visible = 1;
					vis++;
				}
				else
				{
					it->second[i]->visible = 0;
				}


			}

			it->second.front()->model->number_of_visible_objects = vis;

			std::sort(it->second.begin(), it->second.end(), Object::sort_visible);
		}
	}

	void Do_Movement(bool keys[1024], bool keysPressed[1024], GLfloat deltaTime, GLuint *draw_mode, GLboolean wireframe)
	{
		// Camera controls
		if (keys[GLFW_KEY_W])
			ProcessKeyboardAndDraw(FORWARD, deltaTime);
		if (keys[GLFW_KEY_S])
			ProcessKeyboardAndDraw(BACKWARD, deltaTime);
		if (keys[GLFW_KEY_A])
			ProcessKeyboardAndDraw(LEFT, deltaTime);
		if (keys[GLFW_KEY_D])
			ProcessKeyboardAndDraw(RIGHT, deltaTime);
		if (keys[GLFW_KEY_O])
			exposure -= 0.1f;
		if (keys[GLFW_KEY_P])
			exposure += 0.1f;
		if (keys[GLFW_KEY_SPACE])
		{
			if (y == Position.y)
			{
				y = y + player_jump;
				jump = true;
			}
		}

		/*if (keys[GLFW_KEY_1])
			*draw_mode = 1;
		if (keys[GLFW_KEY_2])
			*draw_mode = 2;
		if (keys[GLFW_KEY_3])
			*draw_mode = 3;
		if (keys[GLFW_KEY_4])
			*draw_mode = 4;
		if (keys[GLFW_KEY_5])
			*draw_mode = 5;
			
		if (keys[GLFW_KEY_Z] && !keysPressed[GLFW_KEY_Z])
		{
			wireframe = !wireframe;
			keysPressed[GLFW_KEY_Z] = true;
		}
		*/
		if (y>Position.y)
		{

			Position.y += deltaTime * 4;
			if (y <= Position.y)
			{
				Position.y = y;
				y = land_y;
				//y = Process_height();
			}
			player_model->translate.y = Position.y - 3.0 / 4 * player_height;
			player_model->UpdatePosition();
			
		}
		else if (y<Position.y)
		{
			jump = false;
			Position.y -= deltaTime * 4;
			if (y > Position.y)
				Position.y = y;
			player_model->translate.y = Position.y - 3.0 / 4 * player_height;
			player_model->UpdatePosition();
		}
	}
			
};