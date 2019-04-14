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

	glm::vec3 friction;
	float friction_rate = 0.01;

	glm::vec3 scale;
	float orientation;
	float base_orientation;
	float radius;
	float mass = 1;

	float shear = 0.01;

	float dtime = 0.1;
	//float accelerate_rate = 50;
	float velocity_rate = 10;

	bool xReflected = false;
	bool yReflected = false;

	Object();
	Object(glm::vec3 p, glm::vec3 v, glm::vec3 a);
	~Object() = default;
	virtual void move();
	void set_velocity_toward(glm::vec3);
	glm::mat4 genModelMatrix();
	virtual void getIntoBlackhole();

	void prepare();
	void draw();
	void print();
};

class Sword : public Object 
{
public:
	Sword();
	Sword(glm::vec3 p, glm::vec3 v, glm::vec3 a) :
		Object(p, v, a) {};

	~Sword() = default;

};

class House : public Object
{
public:
	House();
	House(glm::vec3 p, glm::vec3 v, glm::vec3 a) :
		Object(p, v, a) {};

	virtual void move();
	virtual void getIntoBlackhole();
	~House() = default;

};

class BlackHole : public Object
{
public:
	BlackHole();
	BlackHole(glm::vec3 p, glm::vec3 v, glm::vec3 a);


	virtual void move();
	~BlackHole() = default;

};