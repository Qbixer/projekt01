#pragma once

#include "constants.h"
#include "object.h"
#include "physic.h"

enum Modes
{
	WALKING,
	SHOOTING,
	AIMING
};

class Bot
{
public:

	glm::vec3 Position;
	glm::vec3 Scale = glm::vec3(0.4, 0.4, 0.4); //x - promien
	glm::vec3 Rotation = glm::vec3(0, 0, 0);
	Object * object;
	hitbox hitbox; //w scale.x - promien
	std::vector<Punkt> route; //trzeba pamietac ze od konca!!!
	float time = 0;
	float shooting_time = bot_shooting_time;
	float angle = 0;
	int life = bot_health_points;
	int x;
	int z;
	Modes mode = WALKING;
	float aim_angle;

	Bot(int player_x, int player_z, int height, int width, segment **map, std::map<std::string, std::vector<Object*>> * objects, float time, std::vector<Bot*> bots)
	{
		while(true)
		{
			bool able = true;
			int x = rand() % (height * 2 - 1);
			int z = rand() % (width * 2 - 1);
			if (map[z][x].type != 0 && sqrt((player_x - x)*(player_x - x) + (player_z - z)*(player_z - z)) > distance_from_player)
			{
				for (int i = 0; i < bots.size(); i++)
				{
					if (bots[i]->x == x && bots[i]->z == z)
					{
						able = false;
						break;
					}
				}
				if (able)
				{
					this->x = x;
					this->z = z;
					Position.x = x*scale;
					Position.z = z*scale;
					Position.y = bot_position_y;
					hitbox.place = Position;
					hitbox.place.y = hitbox.place.y + Scale.y/2;
					hitbox.rotation = Rotation;
					hitbox.size = Scale;
					hitbox.size.x = Scale.x / 2;
					hitbox.type = SPHERE;
					object = new Object("resources/objects/Drone/Drone.obj", "Bot", Position, Scale, Rotation);
					(*objects)[object->model->path].push_back(object);
					this->time = time;
					while (true)
					{
						int x = rand() % (height * 2 - 1);
						int z = rand() % (width * 2 - 1);
						if (map[z][x].type != 0 && x != this->x && z != this->z)
						{
							route = a_star_algorithm((int)(Position.z / scale), (int)(Position.x / scale), z, x, map, height, width);
							int next_x = route[route.size() - 1].x;
							int next_z = route[route.size() - 1].z;
							if (next_x > this->x && next_z < this->z)
								angle = pi2 + pi4;
							else if (next_x > this->x && next_z == this->z)
								angle = pi2;
							else if (next_x > this->x && next_z > this->z)
								angle = pi4;
							else if (next_x == this->x && next_z > this->z)
								angle = 0;
							else if (next_x < this->x && next_z > this->z)
								angle = pi32 + pi4;
							else if (next_x < this->x && next_z == this->z)
								angle = pi32;
							else if (next_x < this->x && next_z < this->z)
								angle = pi + pi4;
							else if (next_x == this->x && next_z < this->z)
								angle = pi;
							break;
						}
					}
					break;
				}
			}
		}
	}

	Bot(int x, int z, int height, int width, std::map<std::string, std::vector<Object*>> * objects, segment **map)
	{
		this->x = x;
		this->z = z;
		Position.x = x*scale;
		Position.z = z*scale;
		Position.y = bot_position_y;
		hitbox.place = Position;
		hitbox.place.y = hitbox.place.y + Scale.y/2;
		hitbox.rotation = Rotation;
		hitbox.size = Scale;
		hitbox.size.x = Scale.x / 2;
		hitbox.type = SPHERE;
		object = new Object("resources/objects/Drone/Drone.obj", "Bot", Position, Scale, Rotation);
		(*objects)[object->model->path].push_back(object);
		while (true)
		{
			int x = rand() % (height * 2 - 1);
			int z = rand() % (width * 2 - 1);
			if (map[z][x].type != 0 && x != this->x && z != this->z)
			{
				route = a_star_algorithm((int)(Position.z / scale), (int)(Position.x / scale), z, x, map, height, width);
				int next_x = route[route.size() - 1].x;
				int next_z = route[route.size() - 1].z;
				if (next_x > this->x && next_z < this->z)
					angle = pi2 + pi4;
				else if (next_x > this->x && next_z == this->z)
					angle = pi2;
				else if (next_x > this->x && next_z > this->z)
					angle = pi4;
				else if (next_x == this->x && next_z > this->z)
					angle = 0;
				else if (next_x < this->x && next_z > this->z)
					angle = pi32 + pi4;
				else if (next_x < this->x && next_z == this->z)
					angle = pi32;
				else if (next_x < this->x && next_z < this->z)
					angle = pi + pi4;
				else if (next_x == this->x && next_z < this->z)
					angle = pi;
				break;
			}
		}
	}

	~Bot()
	{
		route.clear();
		delete(object);
	}

	void Rotate(float deltaTime, float angle)
	{
		float kat = Rotation.y - angle;
		if (kat < 0)//obrot przeciwnie do wskazowek zegara, chyba ze kat wiekszy od pi
		{
			if (abs(kat) <= pi)
			{
				Rotation.y += deltaTime*angle_speed;
				if (Rotation.y > angle)
					Rotation.y = angle;
			}
			else
			{
				Rotation.y -= deltaTime*angle_speed;
				if (Rotation.y < 0)
					Rotation.y = 2 * pi + Rotation.y;
				if (Rotation.y > pi && Rotation.y < angle)
					Rotation.y = angle;
			}
		}
		else
		{
			if (abs(kat) <= pi)
			{
				Rotation.y -= deltaTime*angle_speed;
				if (Rotation.y < angle)
					Rotation.y = angle;
			}
			else
			{
				Rotation.y += deltaTime*angle_speed;
				if (Rotation.y > 2 * pi)
					Rotation.y = Rotation.y - 2 * pi;
				if (Rotation.y < pi && Rotation.y > angle)
					Rotation.y = angle;
			}
		}
		object->rotate = Rotation;

	}

	void Move(GLfloat deltaTime, segment **map, int height, int width, glm::vec3 Player_position, int player_x, int player_z, int *hp, std::vector<shot_path> * shots, std::map<std::string, std::vector<Object*>> * objects)
	{
		time += deltaTime;
		if (time >= bot_reaction)
		{
			time = time - bot_reaction;			
			if (mode == WALKING && sqrt((Position.x - Player_position.x)*(Position.x - Player_position.x) + (Position.z - Player_position.z)*(Position.z - Player_position.z)) <= bot_range && wall_collision(Position, Player_position, map, x, z, player_x, player_z))
			{
				mode = AIMING;
				aim_angle = calculate_angle(Position, Player_position);
			}
		}
		if (mode == WALKING)
		{
			ProcessMovement(deltaTime, map, height, width);
		}
		if (mode == AIMING)
		{
			Rotate(deltaTime, aim_angle);
			if(Rotation.y == aim_angle)
			{
				if(wall_collision(Position, Player_position, map, x, z, player_x, player_z) && sqrt((Position.x - Player_position.x)*(Position.x - Player_position.x) + (Position.z - Player_position.z)*(Position.z - Player_position.z)) <= bot_range)
				{
					mode = SHOOTING;
					shooting_time = bot_shooting_time;
				}
				else
				{
					mode = WALKING;
				}
			}
		}
		if (mode == SHOOTING)
		{
			aim_angle = calculate_angle(Position, Player_position);
			Rotate(deltaTime, aim_angle);
			shooting_time += deltaTime;
			if(Rotation.y == aim_angle && shooting_time>=bot_shooting_time)
			{
				shooting_time = 0;
				if (wall_collision(Position, Player_position, map, x, z, player_x, player_z))
				{
					float length;
					length = sqrt((Position.x - Player_position.x)*(Position.x - Player_position.x) + (Position.y - (Player_position.y -player_height/8))*(Position.y - (Player_position.y - player_height/8)) + (Position.z - Player_position.z)*(Position.z - Player_position.z));
					float length_y;
					length_y = Position.y - (Player_position.y- player_height/8);
					float angle_x;
					if(length_y<0)
					{
						length_y = -length_y;
						angle_x = asin(length_y / length);
						angle_x = pi2 - angle_x;
					}
					else
					{
						angle_x = pi2 + asin(length_y / length);
					}
					Object *aaa;
					aaa = new Object("resources/objects/pewpewpew/untitled.obj", "pew", glm::vec3(Position.x, Position.y, Position.z), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0),1,glm::vec3(1.0,1.0,0.0));
					aaa->model_mat = glm::rotate(aaa->model_mat, Rotation.y-pi, glm::vec3(0, 1, 0));
					aaa->model_mat = glm::rotate(aaa->model_mat, -angle_x, glm::vec3(1, 0, 0));

					aaa->model_mat = glm::scale(aaa->model_mat, glm::vec3(shot_radius, size_of_draw/2, shot_radius));

					(*objects)[aaa->model->path].push_back(aaa);
					(*objects)[aaa->model->path][0]->model->number_of_visible_objects = (*objects)[aaa->model->path][0]->model->number_of_visible_objects + 1;
					(*shots).push_back(*(new shot_path(aaa)));
					
					*hp = *hp - (rand()%4+3);
				}
				else
				{
					mode = WALKING;
				}
			}
		}

		
			
		object->UpdatePosition();
	}

	

	void ProcessMovement(GLfloat deltaTime, segment **map, int height, int width)
	{
		if (Position.x < x*scale - scale / 2)
			x--;
		if (Position.x > x*scale + scale / 2)
			x++;
		if (Position.z < z*scale - scale / 2)
			z--;
		if (Position.z > z*scale + scale / 2)
			z++;

			
		route.size();
		if (Position.x == route[route.size() - 1].x*scale && Position.z == route[route.size() - 1].z*scale)
		{
			x = route[route.size() - 1].x;
			z = route[route.size() - 1].z;
			route.pop_back();
			if (route.empty())
			{
				while (true)
				{
					int x = rand() % (height * 2 - 1);
					int z = rand() % (width * 2 - 1);
					if (map[z][x].type != 0 && x != this->x && z != this->z)
					{
						route = a_star_algorithm((int)(Position.z / scale), (int)(Position.x / scale), z, x, map, height, width);
						break;
					}
				}
			}
			int next_x = route[route.size() - 1].x;
			int next_z = route[route.size() - 1].z;
			if (next_x > x && next_z < z)
				angle = pi2 + pi4;
			else if (next_x > x && next_z == z)
				angle = pi2;
			else if (next_x > x && next_z > z)
				angle = pi4;
			else if (next_x == x && next_z > z)
				angle = 0;
			else if (next_x < x && next_z > z)
				angle = pi32 + pi4;
			else if (next_x < x && next_z == z)
				angle = pi32;
			else if (next_x < x && next_z < z)
				angle = pi + pi4;
			else if (next_x == x && next_z < z)
				angle = pi;
		}
		
		if (Rotation.y != angle)
		{
			Rotate(deltaTime, angle);			
		}
		else
		{
			if(angle == pi2 + pi4)
			{
				Position.x += deltaTime*bot_speed / p2;
				Position.z -= deltaTime*bot_speed / p2;
				if(Position.x >= route[route.size()-1].x*scale || Position.z <= route[route.size()-1].z*scale)
				{
					Position.x = route[route.size() - 1].x*scale;
					Position.z = route[route.size() - 1].z*scale;
				}
			}
			else if (angle == pi2)
			{
				Position.x += deltaTime*bot_speed;
				if (Position.x >= route[route.size() - 1].x*scale)
				{
					Position.x = route[route.size() - 1].x*scale;
				}
			}				
			else if (angle == pi4)
			{
				Position.x += deltaTime*bot_speed / p2;
				Position.z += deltaTime*bot_speed / p2;
				if (Position.x >= route[route.size() - 1].x*scale || Position.z >= route[route.size() - 1].z*scale)
				{
					Position.x = route[route.size() - 1].x*scale;
					Position.z = route[route.size() - 1].z*scale;
				}
			}				
			else if (angle == 0)
			{

				Position.z += deltaTime*bot_speed;
				if (Position.z >= route[route.size() - 1].z*scale)
				{
					Position.z = route[route.size() - 1].z*scale;
				}
			}				
			else if (angle == pi32 + pi4)
			{
				Position.x -= deltaTime*bot_speed / p2;
				Position.z += deltaTime*bot_speed / p2;
				if (Position.x <= route[route.size() - 1].x*scale || Position.z >= route[route.size() - 1].z*scale)
				{
					Position.x = route[route.size() - 1].x*scale;
					Position.z = route[route.size() - 1].z*scale;
				}
			}
			else if (angle == pi32)
			{
				Position.x -= deltaTime*bot_speed;
				if (Position.x <= route[route.size() - 1].x*scale)
				{
					Position.x = route[route.size() - 1].x*scale;
				}
			}				
			else if (angle == pi + pi4)
			{
				Position.x -= deltaTime*bot_speed / p2;
				Position.z -= deltaTime*bot_speed / p2;
				if (Position.x <= route[route.size() - 1].x*scale || Position.z <= route[route.size() - 1].z*scale)
				{
					Position.x = route[route.size() - 1].x*scale;
					Position.z = route[route.size() - 1].z*scale;
				}
			}				
			else if (angle == pi)
			{
				Position.z -= deltaTime*bot_speed;
				if (Position.z <= route[route.size() - 1].z*scale)
				{
					Position.x = route[route.size() - 1].x*scale;
					Position.z = route[route.size() - 1].z*scale;
				}
			}
			object->translate = Position;
			hitbox.place = Position;
				
		}
		
		
		
	}



};