#pragma once
#include "common/model.h" 
#include "object.h"
#include "level.h"
#include "text.h"


std::map<std::string, Model*> Model::models;
Level * Level::selected_level;
FT_Library Text::ft;
GLuint Text::VAO;
GLuint Text::VBO;
