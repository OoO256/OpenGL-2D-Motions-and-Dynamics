#include "object.h"
#include <cmath>

Object::Object()
	: position(0),
	velocity(0),
	acceleration(0),
	orientation(0),
	scale(1),
	mass(1.0f),
	base_orientation(0),
	friction(0)
{

}

Object::Object(glm::vec3 p = glm::vec3(0), glm::vec3 v = glm::vec3(0), glm::vec3 a = glm::vec3(0))
	: position(p),
	velocity(v),
	acceleration(a),
	orientation(0),
	scale(1),
	mass(1.0f),
	base_orientation(0),
	friction(0)
{

}

glm::mat4 Object::genModelMatrix()
{
	this->move();

	glm::mat4 ModelMatrix(1.0f);



	ModelMatrix = glm::translate(
		ModelMatrix,
		this->position
	);
	// TODO : reflect

	auto ShearMatirx = glm::mat4(1.0f);
	ShearMatirx[1][0] = shear;

	ModelMatrix *= ShearMatirx;

	ModelMatrix = glm::rotate(
		ModelMatrix,
		this->orientation,
		glm::vec3(0.0f, 0.0f, 1.0f)
	);

	ModelMatrix = glm::scale(
		ModelMatrix,
		this->scale
	);


	return ModelMatrix;
}

void Object::getIntoBlackhole()
{
	velocity *= 0.9;
	shear *= 1.05;
	scale *= 0.99;
	mass *= 0.99;
	orientation += 1;

	if (shear > 1000) {
		shear = 0.01;
		position = glm::vec3({1, 0, 0}) * float(rand()%800 - 400);
		velocity = glm::vec3(0);
		acceleration = glm::vec3(0);
		scale = glm::vec3(1);
		mass = 1;
	}
}

void Object::prepare() {
	GLsizeiptr buffer_size = 0;
	for (auto& f : this->figures)
		buffer_size += f.size() * sizeof(vertex);

	std::cout << buffer_size;

	// Initialize vertex buffer object.
	glGenBuffers(1, &this->VBO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	int cum = 0;
	for (auto& f : this->figures)
	{
		glBufferSubData(GL_ARRAY_BUFFER, cum, f.size() * sizeof(vertex), &f[0]);
		cum += f.size() * sizeof(vertex);
	}

	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Object::draw()
{
	glBindVertexArray(this->VAO);

	int cum = 0;
	for (int i = 0; i < this->figures.size() - 1; i++)
	{
		glUniform3fv(loc_primitive_color, 1, &(this->colors[i][0]));
		glDrawArrays(GL_TRIANGLE_FAN, cum, this->figures[i].size());
		cum += this->figures[i].size();
	}

	glUniform3fv(loc_primitive_color, 1, &(this->colors[this->figures.size() - 1][0]));
	glPointSize(5.0);
	glDrawArrays(GL_POINTS, cum, 1);
	glPointSize(1.0);
	glBindVertexArray(0);
}

void Object::set_velocity_toward(glm::vec3 destination) {
	velocity = normalize(destination - position) * sqrtf(velocity_rate);
	this->move();
}

void Object::move() {

	if (velocity != glm::vec3(0) && length(velocity) > 0.1)
	{
		friction = normalize(-velocity)*friction_rate;
	}
	else {
		friction = glm::vec3(0);
	}
	
	glm::vec3 pos_blackholes[4] = {
	{ win_width / 2 - 100, win_height / 2 - 100, 0 },
	{ win_width / 2 - 100, -win_height / 2 + 100, 0 },
	{ -win_width / 2 + 100, win_height / 2 - 100, 0 },
	{ -win_width / 2 + 100, -win_height / 2 + 100, 0 }
	};

	auto a = glm::length(glm::vec3( 2,1, 1 ));

	acceleration = glm::vec3(0);
	for (int i = 0; i< 4; i++) {

		if (glm::length(pos_blackholes[i] - position) > 10) {
			acceleration += gravity_constant / dot(pos_blackholes[i] - position, pos_blackholes[i] - position) * glm::normalize(pos_blackholes[i] - position);
		}
		else
		{
			std::cout << 'A' << std::endl;
			getIntoBlackhole();
		}
	}


	position += velocity * dtime + 0.5f * (acceleration + friction) * dtime * dtime;
	velocity += (acceleration + friction) * dtime;

	if (velocity != glm::vec3(0) && length(velocity) > 1)
		orientation = std::atan2(velocity.y, velocity.x) + base_orientation;

	if (position.x<-win_width / 2 + radius || position.x > win_width / 2 - radius) {
		velocity.x *= -1;
	}
	if (position.y<-win_height / 2 + radius || position.y > win_height / 2 - radius) {
		velocity.y *= -1;
	}	
}

void  Object::print() {
	using namespace std;

	cout << "-------------------------------------" << endl;
	cout << "pos : " << position.x << ' ' << position.y << endl;
	cout << "vel : " << velocity.x << ' ' << velocity.y << endl;
	cout << "acc : " << acceleration.x << ' ' << acceleration.y << endl;
	cout << "mass : " << mass << endl;
	cout << "mass : " << mass << endl;


}

BlackHole::BlackHole() : Object(glm::vec3(0), glm::vec3(0), glm::vec3(0))
{
	figures =
	{
		{{ -9 , -128 } , { -55 , -118 } , { -84 , -106 } , { -95 , -92 } , { -106 , -67 } , { -117 , -49 } , { -131 , -40 } , { -142 , -32 } , { -155 , -15 } , { -163 , 17 } , { -155 , 41 } , { -134 , 75 } , { -118 , 96 } , { -84 , 117 } , { -49 , 142 } , { -21 , 1100 } , { 7 , 151 } , { 42 , 144 } , { 68 , 127 } , { 85 , 112 } , { 104 , 101 } , { 119 , 92 } , { 140 , 72 } , { 146 , 48 } , { 156 , 29 } , { 151 , -13 } , { 144 , -51 } , { 132 , -79 } , { 103 , -90 } , { 65 , -112 } , { 48 , -129 } , { 21 , -129 }},
		{{ -113 , 36 } , { -97 , 65 } , { -72 , 81 } , { -51 , 108 } , { -7 , 120 } , { 25 , 110 } , { 58 , 100 } , { 86 , 80 } , { 103 , 60 } , { 119 , 25 } , { 115 , -8 } , { 98 , -32 } , { 86 , -41 } , { -122 , 13 } , { -106 , -27 } , { -83 , -39 } , { -68 , -62 } , { -45 , -72 } , { -26 , -67 } , { -36 , -43 } , { 68 , -36 } , { 52 , -22 } , { 38 , 10 } , { 24 , 21 } , { 1 , 25 } , { -23 , 12 } , { -34 , -16 }},
		{{ -73 , -23 } , { -91 , -5 } , { -85 , 29 } , { -63 , 55 } , { -21 , 85 } , { 23 , 95 } , { 87 , 51 } , { 94 , 20 } , { 77 , 1 } , { 59 , 8 } , { 48 , 20 } , { 26 , 41 } , { -1 , 45 } , { -29 , 35 } , { -45 , 13 } , { -52 , -15 } , { 60 , 74 }},
		{ { 0 , 72 } , { 18 , 73 } , { 39 , 62 } , { 54 , 46 } , { 44 , 44 } , { 30 , 54 } , { 15 , 58 } , { 6 , 62 }},
		{{ -5 , -58 } , { -26 , -45 } , { -31 , -28 } , { -34 , -1 } , { -19 , 20 } , { -3 , 29 } , { 33 , 10 } , { 45 , -6 } , { 50 , -37 } , { 34 , -54 } , { 14 , -63 } },
		{{0,0}}
	};

	colors = {
		{132 / 255.0f, 23 / 255.0f, 3 / 255.0f},
		{204 / 255.0f, 68 / 255.0f, 3 / 255.0f},
		{250 / 255.0f, 182 / 255.0f, 49 / 255.0f},
		{253 / 255.0f, 249 / 255.0f, 219 / 255.0f},
		{50 / 255.0f, 5 / 255.0f, 0 / 255.0f},
		{50 / 255.0f, 5 / 255.0f, 0 / 255.0f}
	};

	base_orientation = 0 * TO_RADIAN;
	radius = sqrt(80000);
	scale = { 1.0f / 4, 1.0f / 4, 1 };
}

BlackHole::BlackHole(glm::vec3 p, glm::vec3 v, glm::vec3 a) : Object(p, v, a)
{
	figures =
	{
		{{ -9 , -128 } , { -55 , -118 } , { -84 , -106 } , { -95 , -92 } , { -106 , -67 } , { -117 , -49 } , { -131 , -40 } , { -142 , -32 } , { -155 , -15 } , { -163 , 17 } , { -155 , 41 } , { -134 , 75 } , { -118 , 96 } , { -84 , 117 } , { -49 , 142 } , { -21 , 150 } , { 7 , 151 } , { 42 , 144 } , { 68 , 127 } , { 85 , 112 } , { 104 , 101 } , { 119 , 92 } , { 140 , 72 } , { 146 , 48 } , { 156 , 29 } , { 151 , -13 } , { 144 , -51 } , { 132 , -79 } , { 103 , -90 } , { 65 , -112 } , { 48 , -129 } , { 21 , -129 }},
		{{ -113 , 36 } , { -97 , 65 } , { -72 , 81 } , { -51 , 108 } , { -7 , 120 } , { 25 , 110 } , { 58 , 100 } , { 86 , 80 } , { 103 , 60 } , { 119 , 25 } , { 115 , -8 } , { 98 , -32 } , { 86 , -41 } , { -122 , 13 } , { -106 , -27 } , { -83 , -39 } , { -68 , -62 } , { -45 , -72 } , { -26 , -67 } , { -36 , -43 } , { 68 , -36 } , { 52 , -22 } , { 38 , 10 } , { 24 , 21 } , { 1 , 25 } , { -23 , 12 } , { -34 , -16 }},
		{{ -73 , -23 } , { -91 , -5 } , { -85 , 29 } , { -63 , 55 } , { -21 , 85 } , { 23 , 95 } , { 87 , 51 } , { 94 , 20 } , { 77 , 1 } , { 59 , 8 } , { 48 , 20 } , { 26 , 41 } , { -1 , 45 } , { -29 , 35 } , { -45 , 13 } , { -52 , -15 } , { 60 , 74 }},
		{ { 0 , 72 } , { 18 , 73 } , { 39 , 62 } , { 54 , 46 } , { 44 , 44 } , { 30 , 54 } , { 15 , 58 } , { 6 , 62 }},
		{{ -5 , -58 } , { -26 , -45 } , { -31 , -28 } , { -34 , -1 } , { -19 , 20 } , { -3 , 29 } , { 33 , 10 } , { 45 , -6 } , { 50 , -37 } , { 34 , -54 } , { 14 , -63 } },
		{{0,0}}
	};

	colors = {
		{132 / 255.0f, 23 / 255.0f, 3 / 255.0f},
		{204 / 255.0f, 68 / 255.0f, 3 / 255.0f},
		{250 / 255.0f, 182 / 255.0f, 49 / 255.0f},
		{253 / 255.0f, 249 / 255.0f, 219 / 255.0f},
		{50 / 255.0f, 5 / 255.0f, 0 / 255.0f},
		{50 / 255.0f, 5 / 255.0f, 0 / 255.0f}
	};

	base_orientation = 0 * TO_RADIAN;
	radius = sqrt(80000);
	scale = { 1.0f / 4, 1.0f / 4, 1 };
}

void BlackHole::move()
{
	orientation += dtime;
}

void House::move()
{
	if (velocity != glm::vec3(0) && length(velocity) > 0.1)
	{
		friction = normalize(-velocity)*friction_rate;
	}
	else {
		friction = glm::vec3(0);
	}

	glm::vec3 pos_blackholes[4] = {
	{ win_width / 2 - 100, win_height / 2 - 100, 0 },
	{ win_width / 2 - 100, -win_height / 2 + 100, 0 },
	{ -win_width / 2 + 100, win_height / 2 - 100, 0 },
	{ -win_width / 2 + 100, -win_height / 2 + 100, 0 }
	};

	auto a = glm::length(glm::vec3(2, 1, 1));

	acceleration = glm::vec3(0);
	for (int i = 0; i < 4; i++) {

		if (glm::length(pos_blackholes[i] - position) > 10) {
			acceleration += gravity_constant / dot(pos_blackholes[i] - position, pos_blackholes[i] - position) * glm::normalize(pos_blackholes[i] - position);
		}
		else
		{
			std::cout << 'A' << std::endl;
			getIntoBlackhole();
		}
	}


	position += velocity * dtime + 0.5f * (acceleration + friction) * dtime * dtime;
	velocity += (acceleration + friction) * dtime;

	orientation = base_orientation;

	if (position.x<-win_width / 2 + radius || position.x > win_width / 2 - radius) {
		velocity.x *= -1;
	}
	if (position.y<-win_height / 2 + radius || position.y > win_height / 2 - radius) {
		velocity.y *= -1;
	}
}

void House::getIntoBlackhole()
{
	velocity *= 0.9;
	shear *= 1.05;
	scale *= 0.99;
	mass *= 0.99;
	orientation += 1;

	if (shear > 1000) {
		shear = 0.01;
		position = glm::vec3({ 1, 0, 0 }) * float(rand() % 800 - 400);
		velocity = glm::vec3(0);
		acceleration = glm::vec3(0);

		mass = 10;
		scale = { 3, 1, 0 };
	}
}
