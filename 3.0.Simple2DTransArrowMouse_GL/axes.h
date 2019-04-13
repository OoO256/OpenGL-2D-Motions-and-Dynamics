#pragma once
#include "main.h"

extern GLfloat axes[4][2];
extern GLfloat axes_color[3];
extern GLuint VBO_axes, VAO_axes;

void prepare_axes(void);

void update_axes(void);

void draw_axes(void);