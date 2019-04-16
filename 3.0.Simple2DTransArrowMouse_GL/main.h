#pragma once;
#include <array>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <chrono>
#include <vector>
#include <utility>
#include <array>
#include <string>
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, ortho, etc.

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))

#define LOC_VERTEX 0

constexpr float gravity_constant = 1000.0f;

extern int win_width, win_height;
extern float centerx, centery, rotate_angle;

extern GLuint h_ShaderProgram; // handle to shader program
extern GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables

#define DEBUG
#ifdef DEBUG
#define DEBUG(x) x
#else
#define DEBUG(x) do{}while(0);
#endif

class Mouse
{
public:
	int button;
	int state;
	int x;
	int y;


	Mouse() {};
	Mouse(int button, int state, int x, int y)
		: button(button), state(state), x(x), y(y)
	{}
	~Mouse() = default;
};

using vertex = std::pair<GLfloat, GLfloat>;
using figure = std::vector<vertex>;