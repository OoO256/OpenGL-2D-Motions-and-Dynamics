#pragma once
#include "main.h"

class Object
{
public:
	GLuint VBO, VAO;
	std::vector<std::vector<vertex>> figures;
	std::vector<std::array<float, 3>> colors;

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 scale;
	float orientation;
	float base_orientation = 0;
	float radius;
	int mass = 1;

	float dtime = 0.1;
	//float accelerate_rate = 50;
	float velocity_rate = 10;
	float friction_rate = 0.01;

	bool xReflected = false;
	bool yReflected = false;

	Object();
	Object(glm::vec3 p, glm::vec3 v, glm::vec3 a);
	~Object() = default;
	void move();
	void set_velocity_toward(glm::vec3);
	void accelerate_toward(glm::vec3);
	glm::mat4 genModelMatrix();
	void prepare();
	void draw();
};