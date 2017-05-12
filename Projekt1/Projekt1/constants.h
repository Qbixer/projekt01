#pragma once

//Pi
const float pi = 3.14159265358979323846;
const float pi2 = pi / 2;
const float pi32 = 3 * pi2;
const float pi4 = pi / 4;
const float p2 = sqrt(2);
//Drawing
const int size_of_draw = 36;
const float scale = 3;
const char period_of_lights = 10;
const GLuint SCR_WIDTH = 800, SCR_HEIGHT = 600;
const GLfloat near = 1.0f;
const GLfloat far = 25.0f;
GLfloat exposure = 1.0f;
int Pause = 0;
const float shot_lifetime = 0.2;
const float shot_radius = 0.01;
const float light_position_y = 3.9;
//Player
const float player_height = 1.5f; //na 3/4 kamera
const float player_size = 0.5f; //promien
const float player_jump = 0.75;
const int shooting_range = 60;
const float player_model_offset = 0.07;
//Dungeon

const int dungeon_size = 20;
const int distance_from_player = 10;
const int healthpack = 10;
const int appleheal = 5;
const string medkit_path = "resources/objects/abc/medkit4.obj";
const string crystal_path = "resources/objects/octahedron/octahedron.obj";
const string apple_path = "resources/objects/modele/Apple/apple.obj";
const float medkit_position_y = 0.2;
const float offset = 0.3f;
const float min_size = 0.3f;
const int number_of_crystal = 5;
int crystal_count = 5;
//Bot
const float bot_position_y = 2;
const float angle_speed = 2.0;
const float bot_speed = 1.5;
const float bot_reaction = 1.f;
const int bot_health_points = 3;
const float bot_shooting_time = 1;
const int number_of_bots = 30;
const float bot_range = 15;

enum Hitbox_type {
	CUBE,
	CYLINDER, //size podawac w x promien
	MEDKIT,
	SPHERE,
	CRYSTAL,
	APPLE

};

struct hitbox
{
	glm::vec3 place; //przy dodawaniu obeiektow mozna rand od -0.5 do 0.5 i przeniesc pozniej, te x,y w swiecie odpowiadac maja	
	glm::vec3 size;
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	Hitbox_type type = CUBE;
	hitbox() {

	};
	hitbox(glm::vec3 Position, glm::vec3 Scale, glm::vec3 Rotate, Hitbox_type typ) : place(Position), size(Scale), rotation(Rotate), type(typ)
	{

	};
};

struct prop_type { //obiekty znormalizowane do wielkosci max(x,z) == 1 maja byc
	string name;
	const string path;
	const hitbox box;
	const glm::vec3 Position;
	const glm::vec3 Rotation;
	const glm::vec3 Scale;
	prop_type(string nazwa, string sciezka, hitbox * abc, glm::vec3 place, glm::vec3 size, glm::vec3 rotate) : name(nazwa), path(sciezka), box(*abc), Position(place), Scale(size), Rotation(rotate)
	{

	};
};

struct shot_path
{
	Object * object;
	float lifetime = 0;
	shot_path(Object *obiekt) : object(obiekt)
	{

	}
};





