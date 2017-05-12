#pragma once
#include <glm/glm.hpp>
#include <algorithm>
#include "constants.h"
#include "dungeon.h"
#include <list>


glm::vec3 collision(glm::vec3 current_place, glm::vec3 next_place, std::vector<hitbox> &hitbox, float *land_y, float *y, bool jump, int *player_hp, std::map<std::string, std::vector<Object*>> * objects)
{
	glm::vec3 result = next_place;
	for (int i = 0; i < hitbox.size(); i++)
	{
		glm::vec3 new_position = result;
		bool inside = false;
		if (hitbox[i].type == CUBE || hitbox[i].type == MEDKIT)
		{
			if (hitbox[i].rotation.y == 0 || hitbox[i].rotation.y == pi)
			{
				if (next_place.x > hitbox[i].place.x - hitbox[i].size.x / 2 - player_size && next_place.x < hitbox[i].place.x + hitbox[i].size.x / 2 + player_size)
				{
					if (next_place.z > hitbox[i].place.z - hitbox[i].size.z / 2 - player_size && next_place.z < hitbox[i].place.z + hitbox[i].size.z / 2 + player_size)
					{
						if (hitbox[i].type == CUBE)
						{
							if (current_place.x >= hitbox[i].place.x + hitbox[i].size.x / 2 + player_size)
								new_position.x = hitbox[i].place.x + hitbox[i].size.x / 2 + player_size;
							else if (current_place.x <= hitbox[i].place.x - hitbox[i].size.x / 2 - player_size)
								new_position.x = hitbox[i].place.x - hitbox[i].size.x / 2 - player_size;
							if (current_place.z >= hitbox[i].place.z + hitbox[i].size.z / 2 + player_size)
								new_position.z = hitbox[i].place.z + hitbox[i].size.z / 2 + player_size;
							else if (current_place.z <= hitbox[i].place.z - hitbox[i].size.z / 2 - player_size)
								new_position.z = hitbox[i].place.z - hitbox[i].size.z / 2 - player_size;
							inside = true;
						}
						else
						{
							if (*player_hp < 100)
							{
								*player_hp = *player_hp + healthpack;
								for (int it = 0; it < (*objects)[medkit_path].size(); it++) {
									if ((*objects)[medkit_path][it]->translate.x == hitbox[i].place.x && (*objects)[medkit_path][it]->translate.z == hitbox[i].place.z)
									{
										Object *abc;
										abc = (*objects)[medkit_path][it];
										(*objects)[medkit_path][0]->model->number_of_visible_objects = (*objects)[medkit_path][0]->model->number_of_visible_objects - 1;
										(*objects)[medkit_path].erase((*objects)[medkit_path].begin() + it);
										delete(abc);
										break;
									}
								}
								hitbox.erase(hitbox.begin() + i);
							}
							if (*player_hp > 100)
								*player_hp = 100;
						}

					}
				}
			}
			else if (hitbox[i].rotation.y == pi2 || hitbox[i].rotation.y == pi32)
			{
				if (next_place.x > hitbox[i].place.x - hitbox[i].size.z / 2 - player_size && next_place.x < hitbox[i].place.x + hitbox[i].size.z / 2 + player_size)
				{
					if (next_place.z > hitbox[i].place.z - hitbox[i].size.x / 2 - player_size && next_place.z < hitbox[i].place.z + hitbox[i].size.x / 2 + player_size)
					{
						if (hitbox[i].type == CUBE)
						{
							if (current_place.x >= hitbox[i].place.x + hitbox[i].size.z / 2 + player_size)
								new_position.x = hitbox[i].place.x + hitbox[i].size.z / 2 + player_size;
							else if (current_place.x <= hitbox[i].place.x - hitbox[i].size.z / 2 - player_size)
								new_position.x = hitbox[i].place.x - hitbox[i].size.z / 2 - player_size;
							if (current_place.z >= hitbox[i].place.z + hitbox[i].size.x / 2 + player_size)
								new_position.z = hitbox[i].place.z + hitbox[i].size.x / 2 + player_size;
							else if (current_place.z <= hitbox[i].place.z - hitbox[i].size.x / 2 - player_size)
								new_position.z = hitbox[i].place.z - hitbox[i].size.x / 2 - player_size;
							inside = true;
						}
						else
						{
							if (*player_hp < 100)
							{
								*player_hp = *player_hp + healthpack;
								for (int it = 0; it < (*objects)[medkit_path].size(); it++) {
									if ((*objects)[medkit_path][it]->translate.x == hitbox[i].place.x && (*objects)[medkit_path][it]->translate.z == hitbox[i].place.z)
									{
										Object *abc;
										abc = (*objects)[medkit_path][it];
										(*objects)[medkit_path][0]->model->number_of_visible_objects = (*objects)[medkit_path][0]->model->number_of_visible_objects - 1;
										(*objects)[medkit_path].erase((*objects)[medkit_path].begin() + it);
										delete(abc);
										break;
									}
								}
								hitbox.erase(hitbox.begin() + i);
							}
							if (*player_hp > 100)
								*player_hp = 100;
						}
					}
				}
			}
			else
			{

			}
		}
		else if (hitbox[i].type == CYLINDER || hitbox[i].type == CRYSTAL || hitbox[i].type == APPLE)
		{
			if (sqrt((hitbox[i].place.x - next_place.x)*(hitbox[i].place.x - next_place.x) + (hitbox[i].place.z - next_place.z)*(hitbox[i].place.z - next_place.z)) <= hitbox[i].size.x + player_size)
			{
				if (hitbox[i].type == CYLINDER)
				{
					float wz = next_place.z - hitbox[i].place.z;
					float wx = next_place.x - hitbox[i].place.x;
					float k = (hitbox[i].size.x + player_size) / sqrt(wx*wx + wz*wz);
					new_position.x = wx*k + hitbox[i].place.x;
					new_position.z = wz*k + hitbox[i].place.z;
					inside = true;
				}
				else if(hitbox[i].type == CRYSTAL)
				{
					for (int it = 0; it < (*objects)[crystal_path].size(); it++) {
						if ((*objects)[crystal_path][it]->translate.x == hitbox[i].place.x && (*objects)[crystal_path][it]->translate.z == hitbox[i].place.z)
						{
							Object *abc;
							abc = (*objects)[crystal_path][it];
							(*objects)[crystal_path][0]->model->number_of_visible_objects = (*objects)[crystal_path][0]->model->number_of_visible_objects - 1;
							(*objects)[crystal_path].erase((*objects)[crystal_path].begin() + it);
							delete(abc);
							break;
						}
					}
					hitbox.erase(hitbox.begin() + i);
					crystal_count--;
				}
				else
				{
					if (*player_hp < 100)
					{
						*player_hp = *player_hp + appleheal;
						for (int it = 0; it < (*objects)[apple_path].size(); it++) {
							if ((*objects)[apple_path][it]->translate.x == hitbox[i].place.x && (*objects)[apple_path][it]->translate.z == hitbox[i].place.z)
							{
								Object *abc;
								abc = (*objects)[apple_path][it];
								(*objects)[apple_path][0]->model->number_of_visible_objects = (*objects)[apple_path][0]->model->number_of_visible_objects - 1;
								(*objects)[apple_path].erase((*objects)[apple_path].begin() + it);
								delete(abc);
								break;
							}
						}
						hitbox.erase(hitbox.begin() + i);
					}
					if (*player_hp > 100)
						*player_hp = 100;
				}
			}
		}
		else if (hitbox[i].type == SPHERE)
		{

		}
		if (inside && next_place.y >= hitbox[i].place.y + hitbox[i].size.y / 2 + 3 * player_height / 4)
		{
			if (*land_y <= hitbox[i].place.y + hitbox[i].size.y / 2 + 3 * player_height / 4)
				*land_y = hitbox[i].place.y + hitbox[i].size.y / 2 + 3 * player_height / 4;
		}
		else if (inside && jump == false && next_place.y + player_height / 5 >= hitbox[i].place.y + hitbox[i].size.y / 2 + 3 * player_height / 4 && *y < hitbox[i].place.y + hitbox[i].size.y / 2 + 3 * player_height / 4)
		{
			*y = hitbox[i].place.y + hitbox[i].size.y / 2 + 3 * player_height / 4;
			*land_y = hitbox[i].place.y + hitbox[i].size.y / 2 + 3 * player_height / 4;
			result.y = hitbox[i].place.y + hitbox[i].size.y / 2 + 3 * player_height / 4;
		}
		else if (inside &&  next_place.y < hitbox[i].place.y - hitbox[i].size.y / 2 - player_height / 4)
		{

		}
		else
			result = new_position;
	}
	return result;

}

struct Point_with_parent
{
	int x;
	int y;
	int px;
	int py;
	int cost;
	int distance;
};


bool sortBycost(const Point_with_parent &lhs, const Point_with_parent &rhs) { return lhs.cost > rhs.cost; }


vector<Punkt> a_star_algorithm(int start_x, int start_y, int end_x, int end_y, segment **map, int height, int width)
{
	vector<Point_with_parent> openList; // na mapie jako wartosc cost
	vector<Point_with_parent> closedList; //na mapie jako -3
	Point_with_parent present;
	Point_with_parent added;
	present.x = start_x;
	present.y = start_y;
	present.px = -1;
	present.py = -1;
	present.cost = 0;
	present.distance = 0;
	openList.push_back(present);
	while (true)
	{
		std::sort(openList.begin(), openList.end(), sortBycost);
		present = openList[openList.size() - 1];
		openList.pop_back();
		closedList.push_back(present);
		int x = present.x;
		int y = present.y;
		map[x][y].type = -3;
		if (present.x == end_x && present.y == end_y)
		{
			vector<Punkt> result;
			Punkt start;
			start.z = end_x;
			start.x = end_y;
			result.push_back(start);
			for (int i = closedList.size() - 1; i > 0; i--)
			{
				if (result[result.size() - 1].z == closedList[i].x && result[result.size() - 1].x == closedList[i].y)
				{
					Punkt parent;
					parent.z = closedList[i].px;
					parent.x = closedList[i].py;
					result.push_back(parent);
				}
				map[closedList[i].x][closedList[i].y].type = -1;

			}
			result.pop_back();
			map[start_x][start_y].type = -1;
			closedList.clear();
			for (int i = openList.size() - 1; i >= 0; i--)
			{
				map[openList[i].x][openList[i].y].type = -1;
			}
			openList.clear();
			return result;
		}
		if (present.x > 0 && present.y < height * 2 - 2 && map[x - 1][y + 1].type != 0 && map[x][y + 1].type != 0 && map[x - 1][y].type != 0)
		{
			if (map[x - 1][y + 1].type == -1)
			{
				added.x = x - 1;
				added.y = y + 1;
				added.px = x;
				added.py = y;
				added.cost = abs(end_x - (x - 1)) * 10 + abs(end_y - (y + 1)) * 10 + present.distance + 14;
				added.distance = present.distance + 14;
				openList.push_back(added);
				map[x - 1][y + 1].type = added.cost;
			}
			else if (map[x - 1][y + 1].type == -3)
			{

			}
			else
			{
				int cost = abs(end_x - (x - 1)) * 10 + abs(end_y - (y + 1)) * 10 + present.distance + 14;
				if (map[x - 1][y + 1].type > cost)
				{
					for (int i = 0; i < openList.size(); i++)
					{
						if (x - 1 == openList[i].x && y + 1 == openList[i].y)
						{
							openList[i].px = x;
							openList[i].py = y;
							openList[i].cost = cost;
							openList[i].distance = present.distance + 14;
							map[x - 1][y + 1].type = cost;
							break;
						}
					}
				}
			}
		}
		if (present.y < height * 2 - 2 && map[x][y + 1].type != 0)
		{
			if (map[x][y + 1].type == -1)
			{
				added.x = x;
				added.y = y + 1;
				added.px = x;
				added.py = y;
				added.cost = abs(end_x - x) * 10 + abs(end_y - (y + 1)) * 10 + present.distance + 10;
				added.distance = present.distance + 10;
				openList.push_back(added);
				map[x][y + 1].type = added.cost;
			}
			else if (map[x][y + 1].type == -3)
			{

			}
			else
			{
				int cost = abs(end_x - x) * 10 + abs(end_y - (y + 1)) * 10 + present.distance + 10;
				if (map[x][y + 1].type > cost)
				{
					for (int i = 0; i < openList.size(); i++)
					{
						if (x == openList[i].x && y + 1 == openList[i].y)
						{
							openList[i].px = x;
							openList[i].py = y;
							openList[i].cost = cost;
							openList[i].distance = present.distance + 10;
							map[x][y + 1].type = cost;
							break;
						}
					}
				}
			}
		}
		if (present.x < width * 2 - 2 && present.y < height * 2 - 2 && map[x + 1][y + 1].type != 0 && map[x][y + 1].type != 0 && map[x + 1][y].type != 0)
		{
			if (map[x + 1][y + 1].type == -1)
			{
				added.x = x + 1;
				added.y = y + 1;
				added.px = x;
				added.py = y;
				added.cost = abs(end_x - (x + 1)) * 10 + abs(end_y - (y + 1)) * 10 + present.distance + 14;
				added.distance = present.distance + 14;
				openList.push_back(added);
				map[x + 1][y + 1].type = added.cost;
			}
			else if (map[x + 1][y + 1].type == -3)
			{

			}
			else
			{
				int cost = abs(end_x - (x + 1)) * 10 + abs(end_y - (y + 1)) * 10 + present.distance + 14;
				if (map[x + 1][y + 1].type > cost)
				{
					for (int i = 0; i < openList.size(); i++)
					{
						if (x + 1 == openList[i].x && y + 1 == openList[i].y)
						{
							openList[i].px = x;
							openList[i].py = y;
							openList[i].cost = cost;
							openList[i].distance = present.distance + 14;
							map[x + 1][y + 1].type = cost;
							break;
						}
					}
				}
			}
		}
		if (present.x < width * 2 - 2 && map[x + 1][y].type != 0)
		{
			if (map[x + 1][y].type == -1)
			{
				added.x = x + 1;
				added.y = y;
				added.px = x;
				added.py = y;
				added.cost = abs(end_x - (x + 1)) * 10 + abs(end_y - y) * 10 + present.distance + 10;
				added.distance = present.distance + 10;
				openList.push_back(added);
				map[x + 1][y].type = added.cost;
			}
			else if (map[x + 1][y].type == -3)
			{

			}
			else
			{
				int cost = abs(end_x - (x + 1)) * 10 + abs(end_y - y) * 10 + present.distance + 10;
				if (map[x + 1][y].type > cost)
				{
					for (int i = 0; i < openList.size(); i++)
					{
						if (x + 1 == openList[i].x && y == openList[i].y)
						{
							openList[i].px = x;
							openList[i].py = y;
							openList[i].cost = cost;
							openList[i].distance = present.distance + 10;
							map[x + 1][y].type = cost;
							break;
						}
					}
				}
			}
		}
		if (present.x < width * 2 - 2 && present.y > 0 && map[x + 1][y - 1].type != 0 && map[x][y - 1].type != 0 && map[x + 1][y].type != 0)
		{
			if (map[x + 1][y - 1].type == -1)
			{
				added.x = x + 1;
				added.y = y - 1;
				added.px = x;
				added.py = y;
				added.cost = abs(end_x - (x + 1)) * 10 + abs(end_y - (y - 1)) * 10 + present.distance + 14;
				added.distance = present.distance + 14;
				openList.push_back(added);
				map[x + 1][y - 1].type = added.cost;
			}
			else if (map[x + 1][y - 1].type == -3)
			{

			}
			else
			{
				int cost = abs(end_x - (x + 1)) * 10 + abs(end_y - (y - 1)) * 10 + present.distance + 14;
				if (map[x + 1][y - 1].type > cost)
					for (int i = 0; i < openList.size(); i++)
					{
						if (x + 1 == openList[i].x && y - 1 == openList[i].y)
						{
							openList[i].px = x;
							openList[i].py = y;
							openList[i].cost = cost;
							openList[i].distance = present.distance + 14;
							map[x + 1][y - 1].type = cost;
							break;
						}
					}
			}
		}
		if (present.x > 0 && map[x - 1][y].type != 0)
		{
			if (map[x - 1][y].type == -1)
			{
				added.x = x - 1;
				added.y = y;
				added.px = x;
				added.py = y;
				added.cost = abs(end_x - (x - 1)) * 10 + abs(end_y - y) * 10 + present.distance + 10;
				added.distance = present.distance + 10;
				openList.push_back(added);
				map[x - 1][y].type = added.cost;
			}
			else if (map[x - 1][y].type == -3)
			{

			}
			else
			{
				int cost = abs(end_x - (x - 1)) * 10 + abs(end_y - y) * 10 + present.distance + 10;
				if (map[x - 1][y].type > cost)
				{
					for (int i = 0; i < openList.size(); i++)
					{
						if (x - 1 == openList[i].x && y == openList[i].y)
						{
							openList[i].px = x;
							openList[i].py = y;
							openList[i].cost = cost;
							openList[i].distance = present.distance + 10;
							map[x - 1][y].type = cost;
							break;
						}
					}
				}
			}
		}
		if (present.x > 0 && present.y > 0 && map[x - 1][y - 1].type != 0 && map[x][y - 1].type != 0 && map[x - 1][y].type != 0)
		{
			if (map[x - 1][y - 1].type == -1)
			{
				added.x = x - 1;
				added.y = y - 1;
				added.px = x;
				added.py = y;
				added.cost = abs(end_x - (x - 1)) * 10 + abs(end_y - (y - 1)) * 10 + present.distance + 14;
				added.distance = present.distance + 14;
				openList.push_back(added);
				map[x - 1][y - 1].type = added.cost;
			}
			else if (map[x - 1][y - 1].type == -3)
			{

			}
			else
			{
				int cost = abs(end_x - (x - 1)) * 10 + abs(end_y - (y - 1)) * 10 + present.distance + 14;
				if (map[x - 1][y - 1].type > cost)
				{
					for (int i = 0; i < openList.size(); i++)
					{
						if (x - 1 == openList[i].x && y - 1 == openList[i].y)
						{
							openList[i].px = x;
							openList[i].py = y;
							openList[i].cost = cost;
							openList[i].distance = present.distance + 14;
							map[x - 1][y - 1].type = cost;

							break;
						}
					}
				}
			}
		}
		if (present.y > 0 && map[x][y - 1].type != 0)
		{
			if (map[x][y - 1].type == -1)
			{
				added.x = x;
				added.y = y - 1;
				added.px = x;
				added.py = y;
				added.cost = abs(end_x - x) * 10 + abs(end_y - (y - 1)) * 10 + present.distance + 10;
				added.distance = present.distance + 10;
				openList.push_back(added);
				map[x][y - 1].type = added.cost;
			}
			else if (map[x][y - 1].type == -3)
			{

			}
			else
			{
				int cost = abs(end_x - x) * 10 + abs(end_y - (y - 1)) * 10 + present.distance + 10;
				if (map[x][y - 1].type > cost)
					for (int i = 0; i < openList.size(); i++)
					{
						if (x == openList[i].x && y - 1 == openList[i].y)
						{
							openList[i].px = x;
							openList[i].py = y;
							openList[i].cost = cost;
							openList[i].distance = present.distance + 10;
							map[x][y - 1].type = cost;
							break;
						}
					}
			}
		}

	}
}

bool sphere_collision(glm::vec3 position, glm::vec3 front, glm::vec3 center, float radius)
{
	glm::vec3 P1P0 = glm::vec3(center.x - position.x, center.y - position.y, center.z - position.z);
	glm::vec3 top = glm::vec3(P1P0.y*front.z - front.y*P1P0.z, front.x*P1P0.z - P1P0.x*front.z, P1P0.x*front.y - front.x*P1P0.y);
	float length = sqrt(top.x*top.x + top.y*top.y + top.z*top.z);
	float bottom = sqrt(front.x*front.x + front.y*front.y + front.z*front.z);
	if (length / bottom < radius)
		return true;
	else
		return false;

}

bool wall_collision(glm::vec3 player, glm::vec3 bot, segment **map, int px, int pz, int bx, int bz)//px,pz -player x,z; bx,bz - bot x,z
{
	if (px == bx && pz == bz)
		return true;
	if (px == bx)
	{
		if (pz < bz)
		{
			for (int i = pz + 1; i < bz; i++)
			{
				if (map[i][px].type == 0)
					return false;
			}
			return true;
		}
		for (int i = bz + 1; i < pz; i++)
		{
			if (map[i][px].type == 0)
				return false;
		}
		return true;
	}
	if (pz == bz)
	{
		if (px < bx)
		{
			for (int i = px + 1; i < bx; i++)
			{
				if (map[pz][i].type == 0)
					return false;
			}
			return true;
		}
		for (int i = bx + 1; i < px; i++)
		{
			if (map[pz][i].type == 0)
				return false;
		}
		return true;
	}
	float wx, wz;
	wx = bot.x - player.x;
	wz = bot.z - player.z;
	if (abs(wx) > abs(wz))
	{
		if (wx > 0)
		{
			float x_border = px * scale + scale / 2;
			if (wz > 0)
			{
				float z_border = pz * scale + scale / 2;
				int x = px;
				int z = pz;
				glm::vec3 tmp = player;
				float dx = x_border - player.x;
				tmp.z = tmp.z + dx / wx*wz;
				if (tmp.z >= z_border)
				{
					z = z + 1;
					if (map[z][x].type == 0)
						return false;
					z_border = z * scale + scale / 2;
				}
				x = x + 1;
				while (x < bx || z < bz)
				{
					if (map[z][x].type == 0)
						return false;
					tmp.z = tmp.z + scale / wx*wz;
					if (tmp.z >= z_border)
					{
						z = z + 1;
						if (map[z][x].type == 0)
							return false;
						z_border = z * scale + scale / 2;
					}
					x = x + 1;
				}
				return true;
			}
			if (wz < 0)
			{
				float z_border = pz * scale - scale / 2;
				int x = px;
				int z = pz;
				glm::vec3 tmp = player;
				float dx = x_border - player.x;
				tmp.z = tmp.z + dx / wx*wz;
				if (tmp.z <= z_border)
				{
					z = z - 1;
					if (map[z][x].type == 0)
						return false;
					z_border = z * scale - scale / 2;
				}
				x = x + 1;
				while (x < bx || z > bz)
				{
					if (map[z][x].type == 0)
						return false;
					tmp.z = tmp.z + scale / wx*wz;
					if (tmp.z <= z_border)
					{
						z = z - 1;
						if (map[z][x].type == 0)
							return false;
						z_border = z * scale - scale / 2;
					}
					x = x + 1;
				}
				return true;
			}
		}
		else
		{
			float x_border = px * scale - scale / 2;
			if (wz > 0)
			{
				float z_border = pz * scale + scale / 2;
				int x = px;
				int z = pz;
				glm::vec3 tmp = player;
				float dx = x_border - player.x;
				tmp.z = tmp.z + dx / wx*wz;
				if (tmp.z >= z_border)
				{
					z = z + 1;
					if (map[z][x].type == 0)
						return false;
					z_border = z * scale + scale / 2;
				}
				x = x - 1;
				while (x > bx || z < bz)
				{
					if (map[z][x].type == 0)
						return false;
					tmp.z = tmp.z + scale / (-wx)*wz;
					if (tmp.z >= z_border)
					{
						z = z + 1;
						if (map[z][x].type == 0)
							return false;
						z_border = z * scale + scale / 2;
					}
					x = x - 1;
				}
				return true;
			}
			if (wz < 0)
			{
				float z_border = pz * scale - scale / 2;
				int x = px;
				int z = pz;
				glm::vec3 tmp = player;
				float dx = x_border - player.x;
				tmp.z = tmp.z + dx / wx*wz;
				if (tmp.z <= z_border)
				{
					z = z - 1;
					if (map[z][x].type == 0)
						return false;
					z_border = z * scale - scale / 2;
				}
				x = x - 1;
				while (x > bx || z > bz)
				{
					if (map[z][x].type == 0)
						return false;
					tmp.z = tmp.z + scale / (-wx)*wz;
					if (tmp.z <= z_border)
					{
						z = z - 1;
						if (map[z][x].type == 0)
							return false;
						z_border = z * scale - scale / 2;
					}
					x = x - 1;
				}
				return true;
			}
		}

	}
	else
	{
		if (wz > 0)
		{
			float z_border = pz * scale + scale / 2;
			if (wx > 0)
			{
				float x_border = px * scale + scale / 2;
				int z = pz;
				int x = px;
				glm::vec3 tmp = player;
				float dz = z_border - player.z;
				tmp.x = tmp.x + dz / wz*wx;
				if (tmp.x >= x_border)
				{
					x = x + 1;
					if (map[z][x].type == 0)
						return false;
					x_border = x * scale + scale / 2;
				}
				z = z + 1;
				while (z < bz || x < bx)
				{
					if (map[z][x].type == 0)
						return false;
					tmp.x = tmp.x + scale / wz*wx;
					if (tmp.x >= x_border)
					{
						x = x + 1;
						if (map[z][x].type == 0)
							return false;
						x_border = x * scale + scale / 2;
					}
					z = z + 1;
				}
				return true;
			}
			if (wx < 0)
			{
				float x_border = px * scale - scale / 2;
				int z = pz;
				int x = px;
				glm::vec3 tmp = player;
				float dz = z_border - player.z;
				tmp.x = tmp.x + dz / wz*wx;
				if (tmp.x <= x_border)
				{
					x = x - 1;
					if (map[z][x].type == 0)
						return false;
					x_border = x * scale - scale / 2;
				}
				z = z + 1;
				while (z < bz || x > bx)
				{
					if (map[z][x].type == 0)
						return false;
					tmp.x = tmp.x + scale / wz*wx;
					if (tmp.x <= x_border)
					{
						x = x - 1;
						if (map[z][x].type == 0)
							return false;
						x_border = x * scale - scale / 2;
					}
					z = z + 1;
				}
				return true;
			}
		}
		else
		{
			float z_border = pz * scale - scale / 2;
			if (wx > 0)
			{
				float x_border = px * scale + scale / 2;
				int z = pz;
				int x = px;
				glm::vec3 tmp = player;
				float dz = z_border - player.z;
				tmp.x = tmp.x + dz / wz*wx;
				if (tmp.x >= x_border)
				{
					x = x + 1;
					if (map[z][x].type == 0)
						return false;
					x_border = x * scale + scale / 2;
				}
				z = z - 1;
				while (z > bz || x < bx)
				{
					if (map[z][x].type == 0)
						return false;
					tmp.x = tmp.x + scale / (-wz)*wx;
					if (tmp.x >= x_border)
					{
						x = x + 1;
						if (map[z][x].type == 0)
							return false;
						x_border = x * scale + scale / 2;
					}
					z = z - 1;
				}
				return true;
			}
			if (wx < 0)
			{
				float x_border = px * scale - scale / 2;
				int z = pz;
				int x = px;
				glm::vec3 tmp = player;
				float dz = z_border - player.z;
				tmp.x = tmp.x + dz / wz*wx;
				if (tmp.x <= x_border)
				{
					x = x - 1;
					if (map[z][x].type == 0)
						return false;
					x_border = x * scale - scale / 2;
				}
				z = z - 1;
				while (z > bz || x > bx)
				{
					if (map[z][x].type == 0)
						return false;
					tmp.x = tmp.x + scale / (-wz)*wx;
					if (tmp.x <= x_border)
					{
						x = x - 1;
						if (map[z][x].type == 0)
							return false;
						x_border = x * scale - scale / 2;
					}
					z = z - 1;
				}
				return true;
			}
		}
	}
}

float calculate_angle(glm::vec3 player, glm::vec3 bot)
{
	float angle;
	if (player.z == bot.z)
	{
		if (player.x < bot.x)
			angle = pi2;
		else
			angle = pi32;
	}
	else
	{
		float a = (player.x - bot.x) / (player.z - bot.z);
		angle = atan(a);
		if (player.z > bot.z)
			angle = angle + pi;
		if (angle < 0)
			angle = 2 * pi + angle;
	}
	return angle;
}



bool zderzenie(glm::vec3 punkt, glm::vec3 podstawa, glm::vec3 sufit, float promien)//punkt vs walec
{
	if (podstawa.x == sufit.x && podstawa.y == sufit.y && podstawa.z == sufit.z)//punkt w przestrzeni - brak kolizji
		return false;
	else if (podstawa.x == sufit.x && podstawa.z == sufit.z)//prosta rownolegla do y
	{
		if (punkt.y >= std::min(podstawa.y, sufit.y) && punkt.y <= std::max(podstawa.y, sufit.y))
		{
			float odleglosc;
			odleglosc = (punkt.x - podstawa.x)*(punkt.x - podstawa.x) + (punkt.z - podstawa.z)*(punkt.z - podstawa.z);
			if (odleglosc <= (promien * promien))
				return true;
		}
		return false;
	}
	else if (podstawa.z == sufit.z && podstawa.y == sufit.y)//prosta rownolegla do x
	{
		if (punkt.x >= std::min(podstawa.x, sufit.x) && punkt.x <= std::max(podstawa.x, sufit.x))
		{
			float odleglosc;
			odleglosc = (punkt.z - podstawa.z)*(punkt.z - podstawa.z) + (punkt.y - podstawa.y)*(punkt.y - podstawa.y);
			if (odleglosc <= promien ^ 2)
				return true;
		}
		return false;
	}
	else if (podstawa.x == sufit.x && podstawa.y == sufit.y) //prosta rownolegla do z
	{
		if (punkt.z >= std::min(podstawa.z, sufit.z) && punkt.z <= std::max(podstawa.z, sufit.z))
		{
			float odleglosc;
			odleglosc = (punkt.x - podstawa.x)*(punkt.x - podstawa.x) + (punkt.y - podstawa.y)*(punkt.y - podstawa.y);
			if (odleglosc <= promien ^ 2)
				return true;
		}
		return false;
	}
	else
	{
		float A, B, C, D;
		float A1, B1, C1, D1;
		A = podstawa.x - sufit.x;
		B = podstawa.y - sufit.y;
		C = podstawa.z - sufit.z;
		D = -1 * (A*podstawa.x + B*podstawa.y + C*podstawa.z);
		if (punkt.x*A + punkt.y*B + punkt.z*C + D <= 0)
		{
			A1 = sufit.x - podstawa.x;
			B1 = sufit.y - podstawa.y;
			C1 = sufit.z - podstawa.z;
			D1 = -1 * (A1*sufit.x + B1*sufit.y + C1*sufit.z);
			if (punkt.x*A1 + punkt.y*B1 + punkt.z*C1 + D1 <= 0)
			{
				float dA, dB, dC;
				float i, j, k;
				float odleglosc;
				dA = podstawa.x - punkt.x;
				dB = podstawa.y - punkt.y;
				dC = podstawa.z - punkt.z;
				i = dB*C1 - dC*B1;
				j = dC*A1 - dA*C1;
				k = dA*B1 - dB*A1;
				odleglosc = sqrt(i*i + j*j + k*k) / sqrt(A1*A1 + B1*B1 + C1*C1);
				if (odleglosc <= promien)
					return true;
			}
		}
	}
	return false;
}


float kat(glm::vec3 bot, glm::vec3 punkt)//plaszczyzna prostopadla do y
{
	float a;
	if (punkt.z != bot.z && punkt.x != bot.x)
	{
		a = (punkt.x - bot.x) / (punkt.z - bot.z);
		a = atan(a);
		if (a < 0)
			a = a + pi;
		if (punkt.x < bot.x)
			a = a + pi;
	}
	else if (punkt.z != bot.z)
	{
		if (punkt.z >= bot.z)
			a = 0;
		else
			a = pi;
	}
	else
	{
		if (punkt.x >= bot.x)
			a = pi / 2;
		else
			a = 3 * pi / 2;
	}
	return a;
}

bool czy_widzi(glm::vec3 bot, float obrot, float obszar, glm::vec3 punkt)//czy przeciwnik przed nim obszar - kat
{
	float kat_1, kat_2;
	kat_1 = obrot - obszar / 2;
	if (kat_1 < 0)
		kat_1 = 2 * pi + kat_1;
	kat_2 = obrot + obszar / 2;
	if (kat_2 > 2 * pi)
		kat_2 = kat_2 - 2 * pi;
	float a = kat(bot, punkt);
	if (((a >= kat_1) && (a <= kat_2)) || ((a >= kat_1 || a <= kat_2) && (kat_2 < kat_1)))
		return true;
	else
		return false;
}
/*bool czy_przeszkody(glm::vec3 bot, glm::vec3 punkt, float promien, std::vector<glm::vec3> przeszkoda)//zakladam, ze przeszkody takie same jak ludzie
{//slabe
float A, B, C;

A = bot.x - punkt.x;
B = bot.z - punkt.z;
C = -1 * (bot.x*A + bot.z*B);
glm::vec3 wektor;
for each(wektor in przeszkoda)
{
if (A*wektor.x + B*wektor.z + C >= 0)
continue;
else
{
A = -A;
B = -B;
C = -1 * (punkt.x*A + punkt.z*B);
if (A*wektor.x + B*wektor.z + C >= 0)
continue;
else
{
float a, a_1;
float b1, b2;
glm::vec3 pom;
if (B != 0 && A != 0)
{
float x, z, x1, x2, z1, z2;
a = A / B;
a_1 = -1/a;
z = promien / sqrt((a_1*a_1 + 1));
x = abs(a_1 * z);
if (a_1 < 0)
{
x1 = punkt.x + x;
z1 = punkt.z - z;
x2 = punkt.x - x;
z2 = punkt.z + z;
}
else
{
x1 = punkt.x + x;
z1 = punkt.z + z;
x2 = punkt.x - x;
z2 = punkt.z - z;
}
b1 = -a*z1 + x1;
b2 = -a*z2 + x2;
if (b1 > b2)
{
if (wektor.x - a*wektor.z - b1 > 0)
continue;
if (wektor.x - a*wektor.z - b2 < 0)
continue;
}
else
{
if (wektor.x - a*wektor.z - b2 > 0)
continue;
if (wektor.x - a*wektor.z - b1 < 0)
continue;
}

}
else if (B != 0)
{
if (wektor.x > bot.x + promien)
continue;
if (wektor.x < bot.x - promien)
continue;
}
else if (A != 0)
{
if (wektor.z > bot.z + promien)
continue;
if (wektor.z < bot.z - promien)
continue;
}
else
return true;



}
}
return true;
}
return false;
};
*/
bool zderzenie_prosta_walec_pionowy(glm::vec3 srodek, float promien, float wysokosc, glm::vec3 start, glm::vec3 stop)
{
	float a, b, c;
	float c_x, e_x;
	if (start.x != stop.x && start.z != stop.z)
	{
		c_x = -1 * (stop.z - start.z) / (stop.x - start.x);//funkcja z od x
		e_x = (stop.z - start.z) / (stop.x - start.x) + start.z;
		a = 1 / (c_x*c_x) + 1;
		b = (2 * e_x) / (c_x*c_x) + 2 * srodek.x / c_x - 2 * srodek.z;
		c = e_x*e_x / (c_x*c_x) + 2 * srodek.x*e_x / c_x - promien*promien + srodek.x*srodek.x + srodek.z*srodek.z;
		float delta;
		delta = b*b - 4 * a*c;
		if (delta <= 0)
			return false;
		else
		{
			float z1, z2, y1, y2;
			float wz, wy, t;
			z1 = (-b + sqrt(delta)) / (2 * a); //obliczanie z1 i z1
			z2 = (-b - sqrt(delta)) / (2 * a);
			if (((z1 >= start.z && z1 <= stop.z) || (z1 <= start.z && z1 <= stop.z)) || ((z2 >= start.z && z2 <= stop.z) || (z2 <= start.z && z2 <= stop.z)))
			{
				wz = stop.z - start.z; //obliczanie wektorow w przestrzeni
				wy = stop.y - start.y;
				if (wy != 0) //gdy y sie zmienia
				{
					t = (z1 - start.z) / wz; //obliczanie pkt
					y1 = t*wy + start.y;
					t = (z2 - start.z) / wz;
					y2 = t*wy + start.y;
					if (y1 > y2)//uporzadkowac y - y1 ma byc mniejsze od y2
					{
						t = y1;
						y1 = y2;
						y2 = t;
					};
					if (y1 <= srodek.y + wysokosc && y2 >= srodek.y)
						return true;
					else
						return false;
				}
				else //y sie nie zmienia
				{
					if (start.y <= srodek.y + wysokosc && start.y >= srodek.y)
						return true;
					else
						return false;
				}
			}
			else
				return false;
		}
	}
	else if (start.x != stop.x)
	{
		float a, b, c, k;
		float delta;
		a = 1;
		k = (stop.z - srodek.z)*(stop.z - srodek.z);//stpo.z = start.z
		b = -2 * srodek.x;
		c = srodek.x*srodek.x + k*k - promien*promien;
		delta = b*b - 4 * a * c;
		if (delta <= 0)
			return false;
		else
		{
			float x1, x2, y1, y2;
			float wx, wy, t;
			x1 = (-b + sqrt(delta)) / (2 * a); //obliczanie x1 i x2
			x2 = (-b - sqrt(delta)) / (2 * a);
			if (((x1 >= start.x && x1 <= stop.x) || (x1 <= start.x && x1 <= stop.x)) || ((x2 >= start.x && x2 <= stop.x) || (x2 <= start.x && x2 <= stop.x)))
			{
				wx = stop.x - start.x; //obliczanie wektorow w przestrzeni
				wy = stop.y - start.y;
				if (wy != 0) //gdy y sie zmienia
				{
					t = (x1 - start.x) / wx; //obliczanie pkt
					y1 = t*wy + start.y;
					t = (x2 - start.x) / wx;
					y2 = t*wy + start.y;
					if (y1 > y2)//uporzadkowac y - y1 ma byc mniejsze od y2
					{
						t = y1;
						y1 = y2;
						y2 = t;
					};
					if (y1 <= srodek.y + wysokosc && y2 >= srodek.y)
						return true;
					else
						return false;
				}
				else //y sie nie zmienia
				{
					if (start.y <= srodek.y + wysokosc && start.y >= srodek.y)
						return true;
					else
						return false;
				}
			}
			else
				return false;
		}

	}
	else if (start.z != stop.z)
	{
		float a, b, c, k;
		float delta;
		a = 1;
		k = (stop.x - srodek.x)*(stop.x - srodek.x);//stop.x = start.x
		b = -2 * srodek.x;
		c = srodek.x*srodek.x + k*k - promien*promien;
		delta = b*b - 4 * a * c;
		if (delta <= 0)
			return false;
		else
		{
			float z1, z2, y1, y2;
			float wz, wy, t;
			z1 = (-b + sqrt(delta)) / (2 * a); //obliczanie z1 i z2
			z2 = (-b - sqrt(delta)) / (2 * a);
			if (((z1 >= start.z && z1 <= stop.z) || (z1 <= start.z && z1 <= stop.z)) || ((z2 >= start.z && z2 <= stop.z) || (z2 <= start.z && z2 <= stop.z)))
			{
				wz = stop.z - start.z; //obliczanie wektorow w przestrzeni
				wy = stop.y - start.y;
				if (wy != 0) //gdy y sie zmienia
				{
					t = (z1 - start.z) / wz; //obliczanie pkt
					y1 = t*wy + start.y;
					t = (z2 - start.z) / wz;
					y2 = t*wy + start.y;
					if (y1 > y2)//uporzadkowac y - y1 ma byc mniejsze od y2
					{
						t = y1;
						y1 = y2;
						y2 = t;
					};
					if (y1 <= srodek.y + wysokosc && y2 >= srodek.y)
						return true;
					else
						return false;
				}
				else //y sie nie zmienia
				{
					if (start.y <= srodek.y + wysokosc && start.y >= srodek.y)
						return true;
					else
						return false;
				}
			}
			else
				return false;
		}
	}
	else
	{
		if (start.y == stop.y)
			return false;
		else
		{
			if ((start.x - srodek.x)*(start.x - srodek.x) + (start.z - srodek.z)*(start.z - srodek.z) <= promien*promien)
			{
				if (start.y > stop.y)
				{
					if (start.y >= srodek.y && stop.y <= srodek.y + wysokosc)
						return true;
					else
						return false;
				}
				else
				{
					if (stop.y >= srodek.y && start.y <= srodek.y + wysokosc)
						return true;
					else
						return false;
				}
			}
			else
				return false;

		}
	}
};

bool zderzenie_prosta_walec_pochylony(glm::vec3 podstawa, glm::vec3 sufit, float promien, glm::vec3 start, glm::vec3 stop)
{
	float k, r;
	glm::vec3 nowe;
	k = kat(podstawa, sufit);
	r = sqrt((sufit.x - start.x)*(sufit.x - start.x) + (sufit.z - start.z)*(sufit.z - start.z));
	nowe.z = podstawa.z + r;
	nowe.x = podstawa.x;
	nowe.y = podstawa.y;
	//kat w plaszczyznie zx to k
	float alfa;
	alfa = k - kat(podstawa, start);
	r = sqrt((start.x - podstawa.x)*(start.x - podstawa.x) + (start.z - podstawa.z)*(start.z - podstawa.z)); //promien okregu o srodku w podstawie i pkt przez start
	float x, z;
	if (alfa == pi / 2 || alfa == -3 * alfa / 2)
	{
		x = podstawa.x + r;
		z = podstawa.z;
	}
	else if (alfa == -pi / 2 || alfa == 3 * alfa / 2)
	{
		x = podstawa.x - r;
		z = podstawa.z;
	}
	else
	{
		float a, b;
		a = tan(alfa);
		b = podstawa.x - a*podstawa.z;
		float c, d, e;
		c = a*a + 1;
		d = 2 * a*b - 2 * podstawa.x*b - 2 * podstawa.z;
		e = b*b - 2 * podstawa.x*b + podstawa.x*podstawa.x + podstawa.z + podstawa.z - r*r;
		float delta;
		delta = d*d - 4 * c*e; //zawsze >0 bo przeciecie prostej przebiegajacej przez srodek okregu
		float z1, z2;
		z1 = -d + sqrt(delta) / (2 * c);
		z2 = -d - sqrt(delta) / (2 * c);
		if ((alfa > -pi / 2 && alfa < pi / 2) || (alfa > 3 * pi / 2 && alfa < 2 * pi) || (alfa > 2 * pi && alfa < -3 * pi / 2)) //prawa polowa
		{
			if (z1 > z2)
				z = z1;
			else
				z = z2;
		}
		else
		{
			if (z1 < z2)
				z = z1;
			else
				z = z2;
		}
	}
	glm::vec3 nowy_start(x, start.y, z);

	alfa = k - kat(podstawa, stop);
	r = sqrt((stop.x - podstawa.x)*(stop.x - podstawa.x) + (stop.z - podstawa.z)*(stop.z - podstawa.z)); //promien okregu o srodku w podstawie i pkt przez stop
	if (alfa == pi / 2 || alfa == -3 * alfa / 2)
	{
		x = podstawa.x + r;
		z = podstawa.z;
	}
	else if (alfa == -pi / 2 || alfa == 3 * alfa / 2)
	{
		x = podstawa.x - r;
		z = podstawa.z;
	}
	else
	{
		float a, b;
		a = tan(alfa);
		b = podstawa.x - a*podstawa.z;
		float c, d, e;
		c = a*a + 1;
		d = 2 * a*b - 2 * podstawa.x*b - 2 * podstawa.z;
		e = b*b - 2 * podstawa.x*b + podstawa.x*podstawa.x + podstawa.z + podstawa.z - r*r;
		float delta;
		delta = d*d - 4 * c*e; //zawsze >0 bo przeciecie prostej przebiegajacej przez srodek okregu
		float z1, z2;
		z1 = -d + sqrt(delta) / (2 * c);
		z2 = -d - sqrt(delta) / (2 * c);
		if ((alfa > -pi / 2 && alfa < pi / 2) || (alfa > 3 * pi / 2 && alfa < 2 * pi) || (alfa > 2 * pi && alfa < -3 * pi / 2)) //prawa polowa
		{
			if (z1 > z2)
				z = z1;
			else
				z = z2;
		}
		else
		{
			if (z1 < z2)
				z = z1;
			else
				z = z2;
		}
	}
	glm::vec3 nowy_stop(x, stop.y, z);

	return false;
}

bool zderzenie_prosta_prostopadloscian_pionowy(glm::vec3 srodek, float obrot, float sz, float sx, float gz, float gx, glm::vec3 start, glm::vec3 stop)//obrot od 0 do pi/2
{

	if (obrot == pi / 2)
	{

	}
	else if (obrot == 0)
	{

	}
	else
	{
		if (start.x != stop.x && start.z != stop.z)
		{
			float ap, bp;
			ap = (stop.x - start.x) / (stop.z - start.z);
			bp = start.x - ap*start.z;
			float a1, a2;//a1 - bok pionowy, a2 - bok poziomy
			a2 = tan(obrot);
			a1 = -1 / a2;
			float x, z;
			x = srodek.x + gx; // najpierw dla pionowej czyli zaleznej od glebokosci
			z = srodek.z + gz;
			float b;
			b = x - a1*z;
			if (ap != a1)
			{
				float pz, px;//punkty przeciecia
				pz = (b - bp) / (a1 - ap);
				px = ap*pz + bp;

			}

		}
	}


	return false;

}
