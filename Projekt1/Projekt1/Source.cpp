// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H  
// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "common/shader.h"
#include "common/camera.h"
#include "common/model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL.h>
//#include "common/filesystem.h"
#include "shared.h"
#include "object.h"
#include "editor.h"
#include "level.h"
#include "player.h"
#include "text.h"
#include "sprite.h"
#include "bot.h"


int check_errors(char print)
{
	int error = glGetError();
	//if (error && print)
	//	std::cout << "blad " << error << std::endl;
	return error;
}

// Properties

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void button_callback(GLFWwindow* window, int button, int action, int mode);
void RenderQuad();

// Camera
Player camera(glm::vec3(0.0f, 0.0f, 0.0f));

// Delta
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLuint renderedTexture;

// Options
GLuint draw_mode = 1;
GLboolean wireframe = false;


Level level;
Editor editor;

GLuint cubeVAO;
GLuint cubeVBO;

bool keys[1024];
bool keysPressed[1024];

void RenderCube()
{
	// Initialize (if necessary)
	if (cubeVAO == 0)
	{
		GLfloat vertices[] = {
			// Back face
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
			0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
			-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
															  // Front face
															  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
															  0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
															  0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
															  0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
															  -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
															  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
																												 // Left face
																												 -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
																												 -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
																												 -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
																												 -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
																												 -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
																												 -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
																																								   // Right face
																																								   0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
																																								   0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
																																								   0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right
																																								   0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
																																								   0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
																																								   0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
																																																					// Bottom face
																																																					-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
																																																					0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
																																																					0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
																																																					0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
																																																					-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
																																																					-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
																																																																		// Top face
																																																																		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
																																																																		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
																																																																		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right
																																																																		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
																																																																		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
																																																																		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// Fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// Link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// Render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}




// The MAIN function, from here we start our application and run our Game loop
int main()
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Crystal Collector", nullptr, nullptr); // Windowed 
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, button_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Setup and compile our shaders
	Shader shaderLightingPass("deferred_shading.vs", "deferred_shading.frag");
	Shader shader("point_shadows.vs", "point_shadows.frag");
	Shader simpleDepthShader("point_shadows_depth.vs", "point_shadows_depth.frag", "point_shadows_depth.gs");
	Shader shaderLight("deferred_light_box.vs", "deferred_light_box.frag");
	Shader shaderBlur("blur.vs", "blur.frag");
	Shader shaderBloomFinal("bloom_final.vs", "bloom_final.frag");
	Shader textShader("text.vs", "text.frag");
	Shader spriteShader("sprite.vs", "sprite.frag");

	shaderBloomFinal.Use();
	glUniform1i(glGetUniformLocation(shaderBloomFinal.Program, "scene"), 0);
	glUniform1i(glGetUniformLocation(shaderBloomFinal.Program, "bloomBlur"), 1);


	check_errors(0);
	// Set samplers1
    shaderLightingPass.Use();
    glUniform1i(glGetUniformLocation(shaderLightingPass.Program, "gPosition"), 0);
    glUniform1i(glGetUniformLocation(shaderLightingPass.Program, "gNormal"), 1);
    glUniform1i(glGetUniformLocation(shaderLightingPass.Program, "gAlbedoSpec"), 2);
	Level::selected_level = &level;
	Level::selected_level->prop_types.push_back(*(new prop_type((string)"medkit", medkit_path, new hitbox(glm::vec3(0, 0.2, 0), glm::vec3(1, 0.4, 0.7), glm::vec3(0, 0, 0), MEDKIT), glm::vec3(0, 0, 0), glm::vec3(0.025, 0.025, 0.025), glm::vec3(0, 0, 0))));
	Level::selected_level->prop_types.push_back(*(new prop_type((string)"rock", "resources/objects/rock/rock.obj", new hitbox(glm::vec3(0, 0.2/0.7, 0), glm::vec3(0.5, 0.4/0.7, 0.5), glm::vec3(0, 0, 0), CYLINDER), glm::vec3(0, 0.05/0.7, 0), glm::vec3(0.25/0.7, 0.25 / 0.7, 0.25 / 0.7), glm::vec3(0, 0, 0))));
	Level::selected_level->prop_types.push_back(*(new prop_type((string)"boxes", "resources/objects/boxes/Civilian_Boxes_1.obj", new hitbox(glm::vec3(0, 0.5*0.5 / 0.7, 0.1*0.5 / 0.7), glm::vec3(1, 0.8*0.5 / 0.7, 1), glm::vec3(0, 0, 0), CUBE), glm::vec3(0, 0, 0), glm::vec3(0.25 / 0.7, 0.25 / 0.7, 0.25 / 0.7), glm::vec3(0, 0, 0))));
	Level::selected_level->prop_types.push_back(*(new prop_type((string)"microwave", "resources/objects/microwave/microwave.obj", new hitbox(glm::vec3(0, 0.2 / 0.7, 0), glm::vec3(1, 0.8*0.5 / 0.7, 0.5 / 0.7), glm::vec3(0, 0, 0), CUBE), glm::vec3(0, 0.05*0.5/0.7, 0), glm::vec3(0.3*0.5 / 0.7, 0.3*0.5 / 0.7, 0.3*0.5 / 0.7), glm::vec3(0, 0, 0))));
	Level::selected_level->prop_types.push_back(*(new prop_type((string)"Mr_Handy_Box", "resources/objects/Mr_Handy_Box/Mr_Handy_Box.obj", new hitbox(glm::vec3(0, 0.625, 0), glm::vec3(1, 1.25, 1), glm::vec3(0, 0, 0), CUBE), glm::vec3(0, 0, 0), glm::vec3(0.625, 0.625, 0.625), glm::vec3(0, 0, 0))));
	Level::selected_level->prop_types.push_back(*(new prop_type((string)"chair", "resources/objects/old_chair/chair.obj", new hitbox(glm::vec3(0, 1, 0), glm::vec3(1, 2, 1), glm::vec3(0, 0, 0), CUBE), glm::vec3(0, 1, 0), glm::vec3(0.1, 0.1, 0.1), glm::vec3(0, 0, 0))));
	Level::selected_level->prop_types.push_back(*(new prop_type((string)"apple", "resources/objects/modele/Apple/apple.obj", new hitbox(glm::vec3(0, 0.5, 0), glm::vec3(0.5, 1, 0.5), glm::vec3(0, 0, 0), APPLE), glm::vec3(0, 0, 0), glm::vec3(1.5, 1.5, 1.5), glm::vec3(0, 0, 0))));
	Level::selected_level->prop_types.push_back(*(new prop_type((string)"barrel", "resources/objects/modele/Barrel_02/Barrel_02.obj", new hitbox(glm::vec3(0, 0.8, 0), glm::vec3(0.5, 1.6, 0.5), glm::vec3(0, 0, 0), CYLINDER), glm::vec3(0, 0, 0), glm::vec3(0.9, 0.9, 0.9), glm::vec3(0, 0, 0))));
	Level::selected_level->prop_types.push_back(*(new prop_type((string)"oildrum", "resources/objects/modele/oil/oildrum.obj", new hitbox(glm::vec3(0, 0.8, 0), glm::vec3(0.5, 1.6, 0.5), glm::vec3(0, 0, 0), CYLINDER), glm::vec3(0, 0, 0), glm::vec3(1.4, 1.4, 1.4), glm::vec3(0, 0, 0))));

	Level::selected_level->dungeon = new Dungeon(dungeon_size, dungeon_size, &Level::selected_level->objects, &Level::selected_level->lights, Level::selected_level->prop_types);
	
	
	camera.Position.x = (Level::selected_level->dungeon->rooms[0].y)*scale;
	camera.Position.z = (Level::selected_level->dungeon->rooms[0].x + 1)*scale;
	camera.x = (int)(camera.Position.x + scale / 2) / scale;
	camera.z = (int)(camera.Position.z + scale / 2) / scale;
	camera.player_model = new Object("resources/objects/Robot/Robot.obj", "TY", glm::vec3(camera.Position.x, 0, camera.Position.z), glm::vec3(player_height * 3 / 8 + player_model_offset, player_height * 3 / 8 + player_model_offset, player_height * 3 / 8 + player_model_offset), glm::vec3(0, 0, 0));
	Level::selected_level->objects[camera.player_model->model->path].push_back(camera.player_model);
	
	camera.calculate_visibility_of_objects(&Level::selected_level->objects);
	camera.calculate_visibility_of_lights(&Level::selected_level->lights);
	
	Bot *abc;
	for(int i = 0;i<number_of_bots;i++)
	{
		abc = new Bot(camera.x, camera.z, Level::selected_level->dungeon->height, Level::selected_level->dungeon->width,Level::selected_level->dungeon->map,&Level::selected_level->objects,i*bot_reaction/number_of_bots,Level::selected_level->bots);
		Level::selected_level->bots.push_back(abc);
		
	}
	
	// Set up G-Buffer
	// 3 textures:
	// 1. Positions (RGB)
	// 2. Color (RGB) + Specular (A)
	// 3. Normals (RGB)
	GLuint gBuffer;
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    GLuint gPosition, gNormal, gAlbedoSpec;
    // - Position color buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
    // - Normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
    // - Color + Specular color buffer
    glGenTextures(1, &gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
    // - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);
	// - Create and attach depth buffer (renderbuffer)
	GLuint rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// - Finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	GLuint hdrFBO;
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	// - Create 2 floating point color buffers (1 for normal rendering, other for brightness treshold values)
	GLuint colorBuffers[2];
	glGenTextures(2, colorBuffers);
	for (GLuint i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // We clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
	}

	GLuint hdrDepth;
	glGenRenderbuffers(1, &hdrDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, hdrDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, hdrDepth);

	// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	GLuint attachments_hdr[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments_hdr);
	// - Finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	GLuint pingpongFBO[2];
	GLuint pingpongColorbuffers[2];
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColorbuffers);
	for (GLuint i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // We clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
		// Also check if framebuffers are complete (no need for depth buffer)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);





	shader.Use();
	glUniform1i(glGetUniformLocation(shader.Program, "diffuseTexture"), 0);
	glUniform1i(glGetUniformLocation(shader.Program, "normalTexture"), 1);


	for (int i = 0; i < NR_SHADOW; i++) {
		glUniform1i(glGetUniformLocation(shader.Program, (std::string("depthMap[") + std::to_string(i)+std::string("]")).c_str()), i + 2);
	}
	// Light source
	Light::configure_lights();


	if (FT_Init_FreeType(&Text::ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	spriteShader.Use();
	glUniform1i(glGetUniformLocation(shader.Program, "image"), 0);

	Text text(std::string("fonts/arial.ttf"), 46);
	Text text_hp(std::string("fonts/arial.ttf"), 24, 12);
	Text text_cry(std::string("fonts/arial.ttf"), 40);
	Text text_end(std::string("fonts/arial.ttf"), 80);
	Text text_col(std::string("fonts/arial.ttf"), 60);
	Sprite sprite("1.png");


	camera.calculate_visibility_of_objects(&Level::selected_level->objects);
	// Game loop

	float start_time = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{

		// Check and call events
		glfwPollEvents();

		if (Pause == 1) {
			Pause = 2;
			level.selected_level->objects.clear();
			level.selected_level->lights.clear();
			level.selected_level->shots.clear();
			level.selected_level->bots.clear();
			camera.hp = 100;
			Level::selected_level->dungeon = new Dungeon(dungeon_size, dungeon_size, &Level::selected_level->objects, &Level::selected_level->lights, Level::selected_level->prop_types);
			camera.Position.x = (Level::selected_level->dungeon->rooms[0].y)*scale;
			camera.Position.z = (Level::selected_level->dungeon->rooms[0].x + 1)*scale;
			camera.x = (int)(camera.Position.x + scale / 2) / scale;
			camera.z = (int)(camera.Position.z + scale / 2) / scale;
			crystal_count = 5;

			for (auto it = Model::models.begin(); it != Model::models.end(); ++it) {
				it->second->number_of_visible_objects = 0;
			}


			for (int i = 0; i<number_of_bots; i++)
			{
				abc = new Bot(camera.x, camera.z, Level::selected_level->dungeon->height, Level::selected_level->dungeon->width, Level::selected_level->dungeon->map, &Level::selected_level->objects, i*bot_reaction / number_of_bots, Level::selected_level->bots);
				Level::selected_level->bots.push_back(abc);
			}

			camera.player_model = new Object("resources/objects/Robot/Robot.obj", "TY", glm::vec3(camera.Position.x, 0, camera.Position.z), glm::vec3(player_height * 3 / 8 + player_model_offset, player_height * 3 / 8 + player_model_offset, player_height * 3 / 8 + player_model_offset), glm::vec3(0, 0, 0));
			Level::selected_level->objects[camera.player_model->model->path].push_back(camera.player_model);
			camera.calculate_visibility_of_objects(&Level::selected_level->objects);
			camera.calculate_visibility_of_lights(&Level::selected_level->lights);



			continue;
		}

		if (Pause == 2 && glfwGetTime() - start_time >3)
		{
			Pause = 0;
			start_time = glfwGetTime();
		}

		if (Pause == 2)
			continue;

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		camera.Do_Movement(keys, keysPressed, deltaTime, &draw_mode, wireframe);
		for (int i = 0; i < Level::selected_level->bots.size(); i++)
			Level::selected_level->bots[i]->Move(deltaTime, Level::selected_level->dungeon->map, Level::selected_level->dungeon->height, Level::selected_level->dungeon->width, camera.Position, camera.x, camera.z, &camera.hp, &Level::selected_level->shots, &Level::selected_level->objects);
		
		
		for(int i = 0;i<Level::selected_level->shots.size();i++)
		{
			Level::selected_level->shots[i].lifetime += deltaTime;
			if (Level::selected_level->shots[i].lifetime>shot_lifetime)
			{
				Object * abc = Level::selected_level->shots[i].object;
				Level::selected_level->shots.erase(Level::selected_level->shots.begin() + i);
				Level::selected_level->objects[abc->model->path][0]->model->number_of_visible_objects = Level::selected_level->objects[abc->model->path][0]->model->number_of_visible_objects - 1;
				Level::selected_level->objects[abc->model->path].erase(Level::selected_level->objects[abc->model->path].begin() + i);
				delete(abc);
				break;
			}
		}

		for (int i = 0; i<Level::selected_level->objects[crystal_path].size(); i++)
		{
			Level::selected_level->objects[crystal_path][i]->rotate.y = glfwGetTime();
			Level::selected_level->objects[crystal_path][i]->UpdatePosition();

		}

		glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);


		for (int i = 0; i < NR_SHADOW; i++) {
			Light::render_cubemap(simpleDepthShader, i,level.lights);
			Level::selected_level->draw_objects(simpleDepthShader,2);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}



		Light::render_shadows(shader,gBuffer,camera.GetViewMatrix(),camera.Zoom,camera.Position,level.lights);

		Level::selected_level->draw_objects(shader);

		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


        // 2. Lighting Pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the gbuffer's content.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderLightingPass.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
        // Also send light relevant uniforms

		glUniform1f(glGetUniformLocation(shaderLightingPass.Program, "exposure"), exposure);
		glUniform3fv(glGetUniformLocation(shaderLightingPass.Program, "viewPos"), 1, &camera.Position[0]);
		glUniform1i(glGetUniformLocation(shaderLightingPass.Program, "draw_mode"), draw_mode);
		Level::selected_level->draw_lights(shaderLightingPass);
		check_errors(1);
		RenderQuad();
	
		glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, hdrFBO); // Write to default framebuffer
												   // blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
												   // the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
												   // depth buffer in another stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
		glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	
		// 3. Render lights on top of scene, by blitting
		shaderLight.Use();
		glm::mat4 projection = glm::perspective(camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(shaderLight.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shaderLight.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
		for (GLuint i = 0; i < 32; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, Level::selected_level->lights[i]->position);
			model = glm::scale(model, glm::vec3(0.25f));
			glUniformMatrix4fv(glGetUniformLocation(shaderLight.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(glGetUniformLocation(shaderLight.Program, "lightColor"), 1, &Level::selected_level->lights[i]->color[0]);
			RenderCube();

		}

		
		Level::selected_level->draw_objects(shaderLight,1);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);


		
		glDisable(GL_CULL_FACE);
		

		spriteShader.Use();
		projection = glm::ortho(0.0f, (float)(SCR_WIDTH), 0.0f, (float)(SCR_HEIGHT));
		glUniformMatrix4fv(glGetUniformLocation(spriteShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		sprite.DrawSprite(spriteShader,glm::vec2(SCR_WIDTH/2-50, SCR_HEIGHT/2-50), glm::vec2(100, 100), glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f), 0.4f);

		glEnable(GL_CULL_FACE);
		
		textShader.Use();
		projection = glm::ortho(0.0f, (float)(SCR_WIDTH), 0.0f, (float)(SCR_HEIGHT));
		glUniformMatrix4fv(glGetUniformLocation(textShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		string a;
		a = "HP:";
		a += to_string(camera.hp>0?camera.hp:0);
		text.RenderText(textShader, a, 25.0f, 25.0f, 1.0f, glm::vec3(1 - camera.hp / 200.0, camera.hp / 100.0*0.8, 0.2f), 0.4f);
		string hp = std::string(camera.hp > 0 ? camera.hp / 2.2 : 0, '|');
		text_hp.RenderText(textShader, hp, 28.0f, 70.f, 1.0f, glm::vec3(1 - camera.hp / 200.0, camera.hp / 100.0*0.8, 0.2f), 0.8f);
		a = "Crystals:" + to_string(number_of_crystal - crystal_count) + "/" + to_string(number_of_crystal);
		text_cry.RenderText(textShader, a, 550.f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.6f), 0.4f);

		if (crystal_count <= 0) {
			a = "YOU WIN!";
			text_end.RenderText(textShader, a, 400.f, 80.0f, 1.0f, glm::vec3(1.0f, 0.0f, 1.0f), 0.4f);
			start_time = glfwGetTime();
			Pause = 1;
		}

		if (camera.hp <= 0) {
			a = "GAME OVER!";
			text_end.RenderText(textShader, a, 280.f, 80.0f, 1.0f, glm::vec3(0.9f, 0.4f, 0.4f), 0.4f);
			start_time = glfwGetTime();
			Pause = 1;
		}


		if (glfwGetTime() - start_time< 5 && Pause == 0)
		{
			a = "COLLECT ALL CRYSTALS!";
			text_col.RenderText(textShader, a, 20.f, 440.0f, 1.0f, glm::vec3(0.4f, 0.0f, 0.8f), 0.9f);
		}

		
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		// 2. Blur bright fragments w/ two-pass Gaussian Blur 
		GLboolean horizontal = true, first_iteration = true;
		GLuint amount = 10;
		shaderBlur.Use();
		for (GLuint i = 0; i < amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
			glUniform1i(glGetUniformLocation(shaderBlur.Program, "horizontal"), horizontal);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
			RenderQuad();
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 2. Now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderBloomFinal.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
		//glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
		glUniform1f(glGetUniformLocation(shaderBloomFinal.Program, "exposure"), exposure);
		RenderQuad();

		glEnable(GL_DEPTH_TEST);
		

		// Swap the buffers


		glfwSwapBuffers(window);

		
	}

	glfwTerminate();
	return 0;
}


// RenderQuad() Renders a 1x1 quad in NDC, best used for framebuffer color targets
// and post-processing effects.
double lastExposure = 0;
GLuint quadVAO = 0;
GLuint quadVBO;

void RenderQuad()
{
	if (quadVAO == 0)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}



	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	

}


GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key <= 1024)
	{
		//if (editor.enabled)
			//editor.keyboard_callback(action, key);
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
			keysPressed[key] = false;
		}
	}
}

void button_callback(GLFWwindow* window, int button, int action, int mode)
{
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
	{
		camera.ProcessMouseButton();
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//camera.ProcessMouseScroll(yoffset);
}
