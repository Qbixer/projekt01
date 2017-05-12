#pragma once
#include <cstdlib>
#include <vector>
#include "constants.h"
#include <ctime>

struct Punkt
{
	int x;
	int z;
};
struct Room
{
	int x;
	int y;
	int width;
	int height;
};

struct segment
{
	int light = 0;
	std::vector<hitbox> props;
	int type;
};


class Dungeon
{
public:
	int seed = 0;
	segment **map; // WAZNE mape traktuje jako siatke co 2 oczko jest dobre, chyba ze pokoj, lacze nieparzystymi parzyste
	int width;
	int height;
	int attemps = 400;
	std::vector<Room> rooms;

	Dungeon(int width, int height, int seed, std::map<std::string, std::vector<Object*>> * objects, std::vector<Light*> * lights, std::vector<prop_type> prop_types)
	{
		this->width = width;
		this->height = height;
		this->seed = seed;
		srand(this->seed);
		map = new segment *[width * 2 - 1];
		for (int i = 0; i < width * 2 - 1; i++)
		{
			map[i] = new segment[height * 2 - 1];
			for (int k = 0; k < height * 2 - 1; k++)
				map[i][k].type = 0;
		}
		place_rooms();
		make_tunnels();
		make_doors();
		fix();
		delete_dead_ends();
		add_crystals(objects);
		add_elements(objects, prop_types);
		add_light(lights);
	}


	Dungeon(int width, int height, std::map<std::string, std::vector<Object*>> * objects, std::vector<Light*> * lights, std::vector<prop_type> prop_types)
	{
		srand(time(NULL));
		this->width = width;
		this->height = height;
		this->seed = rand();
		printf("seed: %d\n", seed);
		srand(this->seed);
		map = new segment *[width * 2 - 1];
		for (int i = 0; i < width * 2 - 1; i++)
		{
			map[i] = new segment[height * 2 - 1];
			for (int k = 0; k < height * 2 - 1; k++)
			{
				map[i][k].type = 0;
			}
		}
		place_rooms();
		make_tunnels();
		make_doors();
		fix();
		delete_dead_ends();
		//print();
		add_crystals(objects);
		add_elements(objects, prop_types);
		add_light(lights);
	}

	void add_crystals(std::map<std::string, std::vector<Object*>> * objects)
	{
		std::vector<Punkt> places;
		for (int i = 0; i<number_of_crystal; i++)
		{
			bool able = true;
			while (able)
			{
				int k = rand() % rooms.size();
				int x, z;
				x = rooms[k].y + (int)(rooms[k].height / 2);
				z = rooms[k].x + (int)(rooms[k].width / 2);
				for (int j = 0; j<places.size(); j++)
				{
					if (x == places[j].x && z == places[j].z)
					{
						able = false;
						break;
					}
				}
				if (able)
				{
					Object * aaa;
					float r = rand() % RAND_MAX;
					float g = rand() % RAND_MAX;
					float b = rand() % RAND_MAX;
					float w = (r + g + b)*0.8;
					r /= w;
					g /= w;
					b /= w;
					aaa = new Object(crystal_path, "Crystal", glm::vec3(x*scale, 1, z*scale), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 2, glm::vec3(r,g,b));
					(*objects)[aaa->model->path].push_back(aaa);
					hitbox hitbox(glm::vec3(x*scale, 1, z*scale), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), CRYSTAL);
					map[z][x].props.push_back(hitbox);
					break;
				}
			}

		}
	}

	void add_prop(std::map<std::string, std::vector<Object*>> * objects, std::vector<prop_type> prop_types, int x, int z)
	{
		int a;
		if (!map[z][x].props.empty() && map[z][x].props[0].type == CRYSTAL)
			return;
		a = rand() % prop_types.size();
		float size;
		size = ((float)rand() / (RAND_MAX))*scale;
		if (size > scale - 4 * player_size && prop_types[a].box.size.y * 2 > player_jump)
			size = scale - 4 * player_size;
		if (size < min_size)
			size = min_size;
		hitbox hitbox;
		hitbox.place.y = prop_types[a].box.place.y*size;
		hitbox.size = prop_types[a].box.size*size;
		hitbox.rotation = prop_types[a].box.rotation;
		hitbox.type = prop_types[a].box.type;
		//hitbox
		float place_x;
		float place_z;
		glm::vec3 rotation = prop_types[a].Rotation;
		if (hitbox.type == CYLINDER || hitbox.type == CRYSTAL || hitbox.type == APPLE)
		{
			rotation.y = ((float)rand() / (RAND_MAX)) * 2 * pi;
			place_x = ((float)rand() / (RAND_MAX))*(scale - hitbox.size.x) + hitbox.size.x / 2;
			place_z = ((float)rand() / (RAND_MAX))*(scale - hitbox.size.z) + hitbox.size.z / 2;
			place_x = x*scale - scale / 2 + place_x;
			place_z = z*scale - scale / 2 + place_z;
		}
		else if (hitbox.type == CUBE || hitbox.type == MEDKIT)
		{
			int b = rand() % 4;
			switch (b)
			{
			case 0:
			{
				rotation.y = 0;
				hitbox.rotation.y = 0;
				place_x = ((float)rand() / (RAND_MAX))*(scale - hitbox.size.x) + hitbox.size.x / 2 - prop_types[a].box.place.x;
				place_z = ((float)rand() / (RAND_MAX))*(scale - hitbox.size.z) + hitbox.size.z / 2 - prop_types[a].box.place.z;

				place_x = x*scale - scale / 2 + place_x;
				place_z = z*scale - scale / 2 + place_z;
			}
			case 1:
			{
				rotation.y = pi2;
				hitbox.rotation.y = pi2;
				place_x = ((float)rand() / (RAND_MAX))*(scale - hitbox.size.z) + hitbox.size.z / 2 - prop_types[a].box.place.z;
				place_z = ((float)rand() / (RAND_MAX))*(scale - hitbox.size.x) + hitbox.size.x / 2 - prop_types[a].box.place.x;
				place_x = x*scale - scale / 2 + place_x;
				place_z = z*scale - scale / 2 + place_z;
			}
			case 2:
			{
				rotation.y = pi;
				hitbox.rotation.y = pi;
				place_x = ((float)rand() / (RAND_MAX))*(scale - hitbox.size.x) + hitbox.size.x / 2 - prop_types[a].box.place.x;
				place_z = ((float)rand() / (RAND_MAX))*(scale - hitbox.size.z) + hitbox.size.z / 2 - prop_types[a].box.place.z;
				place_x = x*scale - scale / 2 + place_x;
				place_z = z*scale - scale / 2 + place_z;
			}
			case 3:
			{
				rotation.y = pi32;
				hitbox.rotation.y = pi32;
				place_x = ((float)rand() / (RAND_MAX))*(scale - hitbox.size.z) + hitbox.size.z / 2 - prop_types[a].box.place.z;
				place_z = ((float)rand() / (RAND_MAX))*(scale - hitbox.size.x) + hitbox.size.x / 2 - prop_types[a].box.place.x;
				place_x = x*scale - scale / 2 + place_x;
				place_z = z*scale - scale / 2 + place_z;
			}
			}
		}
		glm::vec3 Pos;
		Pos.x = prop_types[a].Position.x*size + place_x;
		Pos.z = prop_types[a].Position.z*size + place_z;
		Pos.y = prop_types[a].Position.y*size;

		Object * aaa;
		aaa = new Object(prop_types[a].path, prop_types[a].name, Pos, prop_types[a].Scale*size, rotation);
		(*objects)[aaa->model->path].push_back(aaa);
		hitbox.place.x = place_x + prop_types[a].box.place.x*size;
		hitbox.place.z = place_z + prop_types[a].box.place.z*size;
		map[z][x].props.push_back(hitbox);
	}

	void add_elements(std::map<std::string, std::vector<Object*>> * objects, std::vector<prop_type> prop_types)
	{
		for (int i = 0; i <= width * 2 - 2; i++)
		{
			for (int j = 0; j <= height * 2 - 2; j++)
			{
				if (map[i][j].type != 0)
				{

					string a = "sciana " + to_string(i) + "," + to_string(j);
					Object * aaa;
					if ((j < height * 2 - 2 && map[i][j + 1].type == 0) || (j == height * 2 - 2))
					{
						aaa = new Object("resources/objects/untitled.obj", a, glm::vec3((j + 0.5)*scale, 2, i*scale), glm::vec3(0.1f, 2, 0.5*scale), glm::vec3(0, pi, 0));
						(*objects)[aaa->model->path].push_back(aaa);
					}

					if ((i < width * 2 - 2 && map[i + 1][j].type == 0) || (i == width * 2 - 2))
					{
						aaa = new Object("resources/objects/untitled.obj", a, glm::vec3(j*scale, 2, (i + 0.5)*scale), glm::vec3(0.1f, 2, 0.5*scale), glm::vec3(0, pi2, 0));
						(*objects)[aaa->model->path].push_back(aaa);
					}
					if ((j > 0 && map[i][j - 1].type == 0) || (j == 0))
					{
						aaa = new Object("resources/objects/untitled.obj", a, glm::vec3((j - 0.5)*scale, 2, i*scale), glm::vec3(0.1f, 2, 0.5*scale), glm::vec3(0, 0, 0));
						(*objects)[aaa->model->path].push_back(aaa);
					}
					if ((i > 0 && map[i - 1][j].type == 0) || (i == 0))
					{
						aaa = new Object("resources/objects/untitled.obj", a, glm::vec3(j*scale, 2, (i - 0.5)*scale), glm::vec3(0.1f, 2, 0.5*scale), glm::vec3(0, pi32, 0));
						(*objects)[aaa->model->path].push_back(aaa);
					}
					if (rand() % 4 == 0)
						add_prop(objects, prop_types, j, i);
					string b = "podloga " + to_string(i) + "," + to_string(j);
					aaa = new Object("resources/objects/Floor/untitled.obj", b, glm::vec3(j*scale, -0.05, i*scale), glm::vec3(0.5*scale, 0.1, 0.5*scale), glm::vec3(0, 0, 0));
					(*objects)[aaa->model->path].push_back(aaa);
					string c = "sufit " + to_string(i) + "," + to_string(j);
					aaa = new Object("resources/objects/untitled.obj", c, glm::vec3(j*scale, 4.05, i*scale), glm::vec3(0.5*scale, 0.1, 0.5*scale), glm::vec3(0, 0, 0));
					(*objects)[aaa->model->path].push_back(aaa);
				}
			}
		}
	}

	~Dungeon()
	{
		for (int i = 0; i<width * 2 - 1; i++)
			delete[] map[i];

		delete[] * map;
	}

	void place_rooms()
	{
		int x, y, dx, dy;
		for (int i = 0; i< attemps; i++)
		{
			x = rand() % (width - 1) * 2;
			y = rand() % (height - 1) * 2;
			if (map[x][y].type != 0)
				continue;
			dx = ((rand() % 3) + 1) * 2;
			dy = ((rand() % 3) + 1) * 2;
			if (x + dx > width * 2 - 2)
				dx = width * 2 - 2 - x;
			if (y + dy > height * 2 - 2)
				dy = height * 2 - 2 - y;
			bool overlap = false;
			for (int j = x; j <= x + dx; j = j + 2)
			{
				for (int k = y; k <= y + dy; k = k + 2)
				{
					if (map[j][k].type != 0)
					{
						overlap = true;
						break;
					}
				}
				if (overlap == true)
					break;
			}
			if (overlap == true)
				continue;
			for (int j = x; j <= x + dx; j++)
			{
				for (int k = y; k <= y + dy; k++)
				{
					map[j][k].type = 50;
				}

			}
			Room tmp;
			tmp.x = x;
			tmp.y = y;
			tmp.width = dx;
			tmp.height = dy;
			rooms.push_back(tmp);
		}
	}

	void crave_tunnels(int x, int y, int value)
	{
		map[x][y].type = value;
		int i = 0;
		bool up = false;
		bool down = false;
		bool right = false;
		bool left = false;
		int obieg = 0;
		do
		{
			obieg++;
			up = false;
			down = false;
			right = false;
			left = false;
			i = 0;
			if (x >= 2) //co drugie x moze tylko kopac, miedzy na 0 a 2 wydrazyc tunel
			{
				if (map[x - 2][y].type == 0)
				{
					i++;
					left = true;
				}
			}
			if (x <= width * 2 - 4)//ostatni element w talbicy
			{
				if (map[x + 2][y].type == 0)
				{
					i++;
					right = true;
				}
			}
			if (y >= 2)
			{
				if (map[x][y - 2].type == 0)
				{
					i++;
					down = true;
				}
			}
			if (y <= height * 2 - 4)
			{
				if (map[x][y + 2].type == 0)
				{
					i++;
					up = true;
				}
			}
			if (i == 0 && obieg == 1)//gdy za pierwszym razem niczego nie ma dookola
			{

			}
			else
			{
				if (i != 0)
				{
					int way;
					int distance;
					distance = (rand() % 4 + 1) * 2;
					way = rand() % i + 1;
					bool tmp = false;
					if (way == i && up == true)//paradoks kata
					{
						for (int k = y + 2; k <= y + distance; k = k + 2)
						{
							if (k <= height * 2 - 2 && map[x][k].type == 0)
							{
								map[x][k - 1].type = value;
								map[x][k].type = value;
							}
							else
							{
								crave_tunnels(x, k - 2, value);
								tmp = true;
								break;
							}
						}
						if (tmp == false)
							crave_tunnels(x, y + distance, value);
					}
					else if (way + 1 >= i && right == true)
					{
						for (int k = x + 2; k <= x + distance; k = k + 2)
						{
							if (k <= width * 2 - 2 && map[k][y].type == 0)
							{
								map[k - 1][y].type = value;
								map[k][y].type = value;
							}
							else
							{
								crave_tunnels(k - 2, y, value);
								tmp = true;
								break;
							}
						}
						if (tmp == false)
							crave_tunnels(x + distance, y, value);
					}
					else if (way + 2 >= i && down == true)
					{
						for (int k = y - 2; k >= y - distance; k = k - 2)
						{
							if (k >= 0 && map[x][k].type == 0)
							{
								map[x][k + 1].type = value;
								map[x][k].type = value;
							}
							else
							{
								crave_tunnels(x, k + 2, value);
								tmp = true;
								break;
							}
						}
						if (tmp == false)
							crave_tunnels(x, y - distance, value);
					}
					else
					{
						for (int k = x - 2; k >= x - distance; k = k - 2)
						{
							if (k >= 0 && map[k][y].type == 0)
							{
								map[k + 1][y].type = value;
								map[k][y].type = value;
							}
							else
							{
								crave_tunnels(k + 2, y, value);
								tmp = true;
								break;
							}
						}
						if (tmp == false)
							crave_tunnels(x - distance, y, value);
					}
					i--;
				}
			}


		} while (i > 0);

	}

	void make_tunnels()
	{
		int value = 1;
		for (int i = 0; i<width * 2 - 1; i = i + 2)
		{
			for (int j = 0; j<height * 2 - 1; j = j + 2)
			{
				if (map[i][j].type == 0)
				{
					crave_tunnels(i, j, value);
					value++;
				}
			}
		}

	}

	void make_doors()
	{
		int x, y;

		for (int i = 0; i < rooms.size(); i++)
		{
			do {

				if (rand() % 2 == 0) //najpierw sterujemy y
				{
					if (rooms[i].x == 0)
						x = rooms[i].x + rooms[i].width;
					else if (rooms[i].x + rooms[i].width == width * 2 - 2)
						x = rooms[i].x;
					else
					{
						if (rand() % 2 == 0)
							x = rooms[i].x;
						else
							x = rooms[i].x + rooms[i].width;
					}
					y = rooms[i].y + rand() % (rooms[i].height + 1);
					if (x == rooms[i].x)
					{
						if (map[x - 2][y].type != 0)
						{
							map[x - 1][y].type = map[x - 2][y].type;
							break;
						}

					}
					else
					{
						if (map[x + 2][y].type != 0)
						{
							map[x + 1][y].type = map[x + 2][y].type;
							break;
						}
					}
				}
				else
				{
					if (rooms[i].y == 0)
						y = rooms[i].y + rooms[i].height;
					else if (rooms[i].y + rooms[i].height == height * 2 - 2)
						y = rooms[i].y;
					else
					{
						if (rand() % 2 == 0)
							y = rooms[i].y;
						else
							y = rooms[i].y + rooms[i].height;
					}
					x = rooms[i].x + rand() % (rooms[i].width + 1);
					if (y == rooms[i].y)
					{
						if (map[x][y - 2].type != 0)
						{
							map[x][y - 1].type = map[x][y - 2].type;
							break;
						}
					}
					else
					{
						if (map[x][y + 2].type != 0)
						{
							map[x][y + 1].type = map[x][y + 2].type;
							break;
						}
					}
				}
			} while (true);
		}
	}

	void fill(int x, int y)
	{
		map[x][y].type = -1;
		if (y < height * 2 - 2 && map[x][y + 1].type != 0 && map[x][y + 1].type != -1)

			fill(x, y + 1);
		if (x < width * 2 - 2 && map[x + 1][y].type != 0 && map[x + 1][y].type != -1)
			fill(x + 1, y);
		if (y > 0 && map[x][y - 1].type != 0 && map[x][y - 1].type != -1)
			fill(x, y - 1);
		if (x > 0 && map[x - 1][y].type != 0 && map[x - 1][y].type != -1)
			fill(x - 1, y);

	}

	void fix()
	{
		for (int i = 0; i<width * 2 - 1; i++)
		{
			for (int j = 0; j<height * 2 - 1; j++)
			{
				if (map[i][j].type == 0)
					continue;
				if (map[i][j].type == -1)
					continue;
				if ((j < height * 2 - 3 && map[i][j + 2].type == -1))
				{
					map[i][j + 1].type = -1;
					fill(i, j);
					continue;
				}
				if (i < width * 2 - 3 && map[i + 2][j].type == -1)
				{
					map[i + 1][j].type = -1;
					fill(i, j);
					continue;
				}
				if (j > 1 && map[i][j - 2].type == -1)
				{
					map[i][j - 1].type = -1;
					fill(i, j);
					continue;
				}
				if (i > 1 && map[i - 2][j].type == -1)
				{
					map[i - 1][j].type = -1;
					fill(i, j);
					continue;
				}
				fill(i, j);
			}
		}
	}


	void delete_dead_ends()
	{
		int x, y, p;
		for (int i = 0; i < width * 2 - 1; i++)
		{
			for (int j = 0; j < height * 2 - 1; j++)
			{
				x = i;
				y = j;
				if (map[x][y].type != 0)
				{
					do
					{
						p = 0;
						if (y < height * 2 - 2 && map[x][y + 1].type != 0)
							p = p + 10;
						if (x < width * 2 - 2 && map[x + 1][y].type != 0)
							p = p + 11;
						if (y > 0 && map[x][y - 1].type != 0)
							p = p + 12;
						if (x > 0 && map[x - 1][y].type != 0)
							p = p + 13;
						if (p > 20)
							break;
						else
						{
							map[x][y].type = 0;
							switch (p)
							{
							case 10:
								y = y + 1;
								break;
							case 11:
								x = x + 1;
								break;
							case 12:
								y = y - 1;
								break;
							case 13:
								x = x - 1;
								break;
							}
						}

					} while (true);
				}
			}
		}
	}



	void add_light(std::vector<Light*>* lights)
	{
		static int number;
		std::string str = "Light";
		const int width = this->width * 2 - 1;
		const int height = this->height * 2 - 1;
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j<height; j++)
			{
				if (!(rand() % period_of_lights) && (map[i][j].type == -1))
				{
					map[i][j].light = 1;
					(*lights).push_back(new Light(str + std::to_string(number), glm::vec3(j*scale, light_position_y, i*scale), glm::vec3(1.f, 1.f, 1.f)));
				}
				else {
					map[i][j].light = 0;
				}
			}
		}
	}



	void print()
	{
		printf("\n");

		for (int i = height * 2 - 2; i >= 0; i--)
		{

			if (i == height * 2 - 2)
			{
				for (int j = 0; j < width * 2 + 1; j++)
					printf("*");
				printf("\n");
			}


			printf("*");
			for (int j = 0; j<width * 2 - 1; j++)
			{

				if (map[j][i].type == 0)
					printf("*");
				else if (map[j][i].type == -1)
					printf(" ");
				else
					printf(" ");
			}
			printf("*");
			printf("\n");
			if (i == 0)
			{
				for (int j = 0; j < width * 2 + 1; j++)
					printf("*");
				printf("\n");
			}

		}
		printf("\n");
	}

};
