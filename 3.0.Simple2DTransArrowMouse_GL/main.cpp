#include "main.h"
#include "axes.h"
#include "airplane.h"
#include "object.h"

#include "Shaders/LoadShaders.h"
GLuint h_ShaderProgram; // handle to shader program
GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables

glm::mat4 ModelViewProjectionMatrix;
glm::mat4 ViewMatrix, ProjectionMatrix, ViewProjectionMatrix;

int win_width = 900, win_height = 900;
float centerx = 0.0f, centery = 0.0f, rotate_angle = 0.0f;

using t_time = std::chrono::milliseconds;

auto last_Crashed = std::chrono::high_resolution_clock::now();

std::vector<BlackHole *>blackholes;
std::vector<Object* >objects;
Object* airplane1;
Mouse* mouse1;

constexpr int idx_sword = 3;

void checkCrash() {

	
	float e = 0.9;
	for (int i = 0; i < objects.size(); i++)
	{
		for (int j = i+1; j < objects.size(); j++)
		{
			if (glm::distance(objects[i]->position, objects[j]->position) < objects[i]->radius + objects[j]->radius 
				&& (std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::high_resolution_clock::now() - last_Crashed)).count() > 100) {
				
				
				auto diff = std::chrono::high_resolution_clock::now() - last_Crashed;

				glm::vec3 v1 = objects[i]->velocity;
				glm::vec3 v2 = objects[j]->velocity;

				float m1 = objects[i]->mass;
				float m2 = objects[j]->mass;
				
				objects[i]->velocity = v1 - m2*(1 + e)/ (m1 + m2) * (v1-v2);
				objects[j]->velocity = v2 + m1*(1 + e) / (m1 + m2) * (v1 - v2);

				if (i == idx_sword || j == idx_sword) {
					objects[i]->velocity = v1 - m2 * (1 + 0) / (m1 + m2) * (v1 - v2);
					objects[j]->velocity = v2 + m1 * (1 + 0) / (m1 + m2) * (v1 - v2);
				}

				last_Crashed = std::chrono::high_resolution_clock::now();
			}
		}
	}
}

void display(void) {
	
	checkCrash();
	int i;
	float x, r, s, delx, delr, dels;
	glm::mat4 ModelMatrix;

	glClear(GL_COLOR_BUFFER_BIT);

	for (const auto& bh : blackholes)
	{
		ModelMatrix = bh->genModelMatrix();
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		bh->draw();
	}

	ModelMatrix = glm::mat4(1.0f);
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_axes();

	for (auto& obj : objects)
	{
		ModelMatrix = obj->genModelMatrix();
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		obj->draw();
	}
	glFlush();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups.
		break;
	case 'A': 
		airplane1->radius *= 0.9;
		airplane1->scale *= 0.9;
		airplane1->mass *= 0.9;
		break;
	case 'a':
		airplane1->radius *= 0.9;
		airplane1->scale *= 0.9;
		airplane1->mass *= 0.9;
		break;
	case 'S':
		airplane1->radius *= 1.1;
		airplane1->scale *= 1.1;
		airplane1->mass *= 1.1;
		break;
	case 's':
		airplane1->radius *= 1.1;
		airplane1->scale *= 1.1;
		airplane1->mass *= 1.1;
		break;
	case ' ':
		if(airplane1->velocity != glm::vec3(0))
			airplane1->velocity += glm::normalize(airplane1->velocity) * 5.0f;
		break;
	}
}

void reshape(int width, int height) {
	win_width = width, win_height = height;

	glViewport(0, 0, win_width, win_height);
	ProjectionMatrix = glm::ortho(-win_width / 2.0, win_width / 2.0,
		-win_height / 2.0, win_height / 2.0, -1000.0, 1000.0);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	update_axes();
	
	blackholes[0]->position = { win_width / 2 - 100, win_height / 2 - 100, 0 };
	blackholes[1]->position = { win_width / 2 - 100, -win_height / 2 + 100, 0 };
	blackholes[2]->position = { -win_width / 2 + 100, win_height / 2 - 100, 0 };
	blackholes[3]->position = { -win_width / 2 + 100, -win_height / 2 + 100, 0 };

	glutPostRedisplay();
}

void cleanup(void) {
	glDeleteVertexArrays(1, &VAO_axes);
	glDeleteBuffers(1, &VBO_axes);

	//glDeleteVertexArrays(1, &VAO_line);
	//glDeleteBuffers(1, &VBO_line);

	glDeleteVertexArrays(1, &(airplane1->VAO));
	glDeleteBuffers(1, &(airplane1->VBO));

	// Delete others here too!!!
}

void timer(int time) {
	glutPostRedisplay();
	glutTimerFunc(1000/600, timer, 0);
}


void mouse(int button, int state, int x, int y) {
	x -= win_width / 2.0f;
	y = win_height/2.0f - y;
	*mouse1 = { button, state, x, y };
	//DEBUG(std::cout << "mouse : " << x << " "<< y << std::endl);
	//DEBUG(std::cout << "pos : " << airplane1->position.x << airplane1->position.y << std::endl);
	airplane1->set_velocity_toward({ mouse1->x, mouse1->y, 0 });
	glutPostRedisplay();


	DEBUG(airplane1->print());
}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutReshapeFunc(reshape);
	glutCloseFunc(cleanup);
	glutTimerFunc(0, timer, 0);
}

void prepare_shader_program(void) {
	ShaderInfo shader_info[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
		{ GL_NONE, NULL }
	};

	h_ShaderProgram = LoadShaders(shader_info);
	glUseProgram(h_ShaderProgram);

	loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewProjectionMatrix");
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram, "u_primitive_color");
}

void initialize_OpenGL(void) {
	glEnable(GL_MULTISAMPLE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glClearColor(10 / 255.0f, 0 / 255.0f, 30 / 255.0f, 1.0f);
	ViewMatrix = glm::mat4(1.0f);
}

void prepare_scene(void) {
	airplane1 = new Object({ 100 , 0, 0 }, { 0,0,0 }, { 0,0,0 });
	objects.push_back(airplane1);
	airplane1->figures = {
		{ { 0.0, 0.0 },{ -20.0, 15.0 },{ -20.0, 20.0 },{ 0.0, 23.0 },{ 20.0, 20.0 },{ 20.0, 15.0 } },
		{ { 0.0, -18.0 },{ -11.0, -12.0 },{ -12.0, -7.0 },{ 0.0, -10.0 },{ 12.0, -7.0 },{ 11.0, -12.0 } },
		{ { 0.0, -25.0 },{ -6.0, 0.0 },{ -6.0, 22.0 },{ 6.0, 22.0 },{ 6.0, 0.0 } },
		{ { 0.0, 25.0 },{ -7.0, 24.0 },{ -7.0, 21.0 },{ 7.0, 21.0 },{ 7.0, 24.0 } },
		{ { -20.0, 10.0 },{ -18.0, 3.0 },{ -16.0, 10.0 },{ -18.0, 20.0 },{ -20.0, 20.0 } },
		{ { 20.0, 10.0 },{ 18.0, 3.0 },{ 16.0, 10.0 },{ 18.0, 20.0 },{ 20.0, 20.0 } },
		{ { 0.0, 0.0 } }
	};

	airplane1->colors = {
	{ 150 / 255.0f, 129 / 255.0f, 183 / 255.0f },  // big_wing
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // small_wing
	{ 111 / 255.0f,  85 / 255.0f, 157 / 255.0f },  // body
	{ 150 / 255.0f, 129 / 255.0f, 183 / 255.0f },  // back
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // sidewinder1
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // sidewinder2
	{ 255 / 255.0f,   0 / 255.0f,   0 / 255.0f }   // center
	};

	airplane1->radius = sqrt(800);
	airplane1->base_orientation = 90 * TO_RADIAN;
	airplane1->mass = 1.5;
	airplane1->prepare();
	

	mouse1 = new Mouse();
	prepare_axes();


	Object* car1 = new Object();
	objects.push_back(car1);
	car1->figures =
	{
		{ { -16.0, -8.0 },{ -16.0, 0.0 },{ 16.0, 0.0 },{ 16.0, -8.0 } },
		{ { -10.0, 0.0 },{ -10.0, 10.0 },{ 10.0, 10.0 },{ 10.0, 0.0 } },
		{ { -8.0, 0.0 },{ -8.0, 8.0 },{ 8.0, 8.0 },{ 8.0, 0.0 } },
		{ { -9.0, -6.0 },{ -10.0, -5.0 },{ -9.0, -4.0 },{ -8.0, -5.0 } },
		{ { 9.0, -6.0 },{ 8.0, -5.0 },{ 9.0, -4.0 },{ 10.0, -5.0 } },
		{ { -10.0, -12.0 },{ -10.0, -8.0 },{ -6.0, -8.0 },{ -6.0, -12.0 } },
		{ { 6.0, -12.0 },{ 6.0, -8.0 },{ 10.0, -8.0 },{ 10.0, -12.0 } },
		{ { 0.0, 0.0 } }
	};
	car1->colors = 
	{
		{ 0 / 255.0f, 149 / 255.0f, 159 / 255.0f },
		{ 0 / 255.0f, 149 / 255.0f, 159 / 255.0f },
		{ 216 / 255.0f, 208 / 255.0f, 174 / 255.0f },
		{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f },
		{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f },
		{ 21 / 255.0f, 30 / 255.0f, 26 / 255.0f },
		{ 21 / 255.0f, 30 / 255.0f, 26 / 255.0f },
		{ 0 / 255.0f, 149 / 255.0f, 159 / 255.0f }
	};
	car1->base_orientation = 270 * TO_RADIAN;
	car1->prepare();
	car1->radius = sqrt(16*16+8*8);

	Object* car2 = new Object({-100, 0, 0}, {0,0,0}, {0,0,0});
	objects.push_back(car2);
	car2->figures =
	{
		{ { -16.0, -8.0 },{ -16.0, 0.0 },{ 16.0, 0.0 },{ 16.0, -8.0 } },
		{ { -10.0, 0.0 },{ -10.0, 10.0 },{ 10.0, 10.0 },{ 10.0, 0.0 } },
		{ { -8.0, 0.0 },{ -8.0, 8.0 },{ 8.0, 8.0 },{ 8.0, 0.0 } },
		{ { -9.0, -6.0 },{ -10.0, -5.0 },{ -9.0, -4.0 },{ -8.0, -5.0 } },
		{ { 9.0, -6.0 },{ 8.0, -5.0 },{ 9.0, -4.0 },{ 10.0, -5.0 } },
		{ { -10.0, -12.0 },{ -10.0, -8.0 },{ -6.0, -8.0 },{ -6.0, -12.0 } },
		{ { 6.0, -12.0 },{ 6.0, -8.0 },{ 10.0, -8.0 },{ 10.0, -12.0 } },
		{ { 0.0, 0.0 } }
	};
	car2->colors =
	{
		{ 0 / 255.0f, 149 / 255.0f, 159 / 255.0f },
		{ 0 / 255.0f, 149 / 255.0f, 159 / 255.0f },
		{ 216 / 255.0f, 208 / 255.0f, 174 / 255.0f },
		{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f },
		{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f },
		{ 21 / 255.0f, 30 / 255.0f, 26 / 255.0f },
		{ 21 / 255.0f, 30 / 255.0f, 26 / 255.0f },
		{ 0 / 255.0f, 149 / 255.0f, 159 / 255.0f }
	};
	car2->base_orientation = 270 * TO_RADIAN;
	car2->prepare();
	car2->radius = sqrt(16 * 16 + 8 * 8);

	Sword* sword1 = new Sword({ 0,100,0 }, { 10,10,0 }, {0, -1, 0});
	objects.push_back(sword1);
	sword1->figures =
	{
		{ { -6.0, 0.0 },{ -6.0, -4.0 },{ 6.0, -4.0 },{ 6.0, 0.0 } },
		{ { -2.0, -4.0 },{ -2.0, -6.0 } ,{ 2.0, -6.0 },{ 2.0, -4.0 } },
		{ { -2.0, 0.0 },{ -2.0, 16.0 } ,{ 2.0, 16.0 },{ 2.0, 0.0 } },
		{ { -2.0, 16.0 },{ 0.0, 19.46 } ,{ 2.0, 16.0 } },
		{ { -0.3, 0.7 },{ -0.3, 15.3 } ,{ 0.3, 15.3 },{ 0.3, 0.7 } },
		{ { -2.0, -6.0 } ,{ 2.0, -6.0 },{ 4.0, -8.0 },{ -4.0, -8.0 } },
		{ { 0.0, -1.0 } ,{ 1.0, -2.732 },{ 0.0, -4.464 },{ -1.0, -2.732 } },
		{ { 0.0, 0.0 } }
	};
	sword1->colors =
	{
		{ 139 / 255.0f, 69 / 255.0f, 19 / 255.0f },
		{ 139 / 255.0f, 69 / 255.0f, 19 / 255.0f },
		{ 155 / 255.0f, 155 / 255.0f, 155 / 255.0f },
		{ 155 / 255.0f, 155 / 255.0f, 155 / 255.0f },
		{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
		{ 139 / 255.0f, 69 / 255.0f, 19 / 255.0f },
		{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f },
		{ 139 / 255.0f, 69 / 255.0f, 19 / 255.0f }
	};
	sword1->base_orientation = 0 * TO_RADIAN;
	sword1->prepare();
	sword1->radius = sqrt(19.46*19.46);
	sword1->friction_rate = 0;


	House* house1 = new House({ 0,200,0 }, { 10,0,0 }, { 0, 0, 0 });
	objects.push_back(house1);
	house1->figures =
	{
		{ { -12.0, 0.0 },{ 0.0, 12.0 },{ 12.0, 0.0 } },
		{ { -12.0, -14.0 },{ -12.0, 0.0 },{ 12.0, 0.0 },{ 12.0, -14.0 } },
		{ { 6.0, 6.0 },{ 6.0, 14.0 },{ 10.0, 14.0 },{ 10.0, 2.0 } },
		{ { -8.0, -14.0 },{ -8.0, -8.0 },{ -4.0, -8.0 },{ -4.0, -14.0 } },
		{ { 4.0, -6.0 },{ 4.0, -2.0 },{ 8.0, -2.0 },{ 8.0, -6.0 } },
		{ { 0 , 0 }	}
	};
	house1->colors =
	{
		{ 200 / 255.0f, 39 / 255.0f, 42 / 255.0f },
		{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
		{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f },
		{ 233 / 255.0f, 113 / 255.0f, 23 / 255.0f },
		{ 44 / 255.0f, 180 / 255.0f, 49 / 255.0f },
		{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f }
	};
	house1->base_orientation = 0 * TO_RADIAN;
	house1->mass = 10;
	house1->friction_rate = 0.5;
	house1->scale = { 3, 1, 0 };
	house1->prepare();
	house1->radius = sqrt(12*12 + 14*14);


	Object *cocktail1 = new Object({ 0,-100,0 }, { 10,0,0 }, { 0, 0, 0 });
	objects.push_back(cocktail1);
	cocktail1->figures =
	{
		{ { -6.0, -12.0 },{ -6.0, -11.0 },{ -1.0, 0.0 },{ 1.0, 0.0 },{ 6.0, -11.0 },{ 6.0, -12.0 } }, 
		{ { -1.0, 0.0 },{ -9.0, 4.0 },{ -12.0, 7.0 },{ 12.0, 7.0 },{ 9.0, 4.0 },{ 1.0, 0.0 } }, 
		{ { -12.0, 7.0 },{ -12.0, 10.0 },{ 12.0, 10.0 },{ 12.0, 7.0 } }, 
		{ { 7.0, 7.0 },{ 12.0, 12.0 },{ 14.0, 12.0 },{ 9.0, 7.0 } }, 
		{ { 12.0, 12.0 },{ 10.0, 14.0 },{ 10.0, 16.0 },{ 12.0, 18.0 },{ 14.0, 18.0 },{ 16.0, 16.0 },{ 16.0, 14.0 },{ 14.0, 12.0 } }, 
		{ { 0 , 0 }	}
	};
	cocktail1->colors =
	{
		{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
		{ 0 / 255.0f, 63 / 255.0f, 122 / 255.0f },
		{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
		{ 191 / 255.0f, 255 / 255.0f, 0 / 255.0f },
		{ 218 / 255.0f, 165 / 255.0f, 32 / 255.0f },
		{ 0 / 255.0f, 63 / 255.0f, 122 / 255.0f }
	};
	cocktail1->base_orientation = 0 * TO_RADIAN;
	cocktail1->scale = { 1, 2, 0 };
	cocktail1->prepare();
	cocktail1->radius = sqrt(12 * 12 + 10 * 10);



	blackholes.resize(4);
	blackholes[0] = new BlackHole({ win_width / 2 - 100, win_height / 2 - 100, 0 }, glm::vec3(0), glm::vec3(0));
	blackholes[1] = new BlackHole({ win_width / 2 - 100, -win_height / 2 + 100, 0 }, glm::vec3(0), glm::vec3(0));
	blackholes[2] = new BlackHole({ -win_width / 2 + 100, win_height / 2 - 100, 0 }, glm::vec3(0), glm::vec3(0));
	blackholes[3] = new BlackHole({ -win_width / 2 + 100, -win_height / 2 + 100, 0 }, glm::vec3(0), glm::vec3(0));

	
	blackholes[0]->prepare();
	blackholes[1]->prepare();
	blackholes[2]->prepare();
	blackholes[3]->prepare();	
}

void initialize_renderer(void) {
	register_callbacks();
	prepare_shader_program();
	initialize_OpenGL();
	prepare_scene();
}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = GL_TRUE;

	error = glewInit();
	if (error != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(-1);
	}
	fprintf(stdout, "*********************************************************\n");
	fprintf(stdout, " - GLEW version supported: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, " - OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, " - OpenGL version supported: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "*********************************************************\n\n");
}


void greetings(char *program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    This program was coded for CSE4170 students\n");
	fprintf(stdout, "      of Dept. of Comp. Sci. & Eng., Sogang University.\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");
}

#define N_MESSAGE_LINES 1
int main(int argc, char *argv[]) {

	std::ios_base::sync_with_stdio(false);

	char program_name[64] = "Sogang CSE4170 2DObjects_GLSL_3.0.1.3";
	char messages[N_MESSAGE_LINES][256] = {
		"    - Keys used: 'ESC' "
	};

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize(900, 900);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_glew();
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutMainLoop();
}