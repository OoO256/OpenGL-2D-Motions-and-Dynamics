#include "object.h"
#include <cmath>

Object::Object()
	: position(0),
	velocity(0),
	acceleration(0),
	orientation(0),
	scale(1)
{

}

Object::Object(glm::vec3 p, glm::vec3 v, glm::vec3 a)
	: position(p),
	velocity(v),
	acceleration(a),
	orientation(0),
	scale(1)
{

}
/*
void Object::accelerate_toward(glm::vec3 destination) {
	acceleration = normalize(destination - position) * sqrtf(accelerate_rate);
	this->move();
	acceleration = { 0, 0, 0 };
}
*/

glm::mat4 Object::genModelMatrix()
{
	this->move();

	glm::mat4 ModelMatrix(1.0f);

	ModelMatrix = glm::translate(
		ModelMatrix,
		this->position
	);
	// TODO : reflect

	ModelMatrix = glm::rotate(
		ModelMatrix,
		this->orientation + 90*TO_RADIAN,
		glm::vec3(0.0f, 0.0f, 1.0f)
	);

	ModelMatrix = glm::scale(
		ModelMatrix,
		this->scale
	);


	return ModelMatrix;
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
		acceleration = normalize(-velocity)*friction_rate;
	}
	
	position += velocity * dtime + 0.5f * acceleration * dtime * dtime;
	velocity += acceleration * dtime;

	if (velocity != glm::vec3(0) && length(velocity) > 1)
		orientation = std::atan2(velocity.y, velocity.x) + base_orientation;

	if (position.x<-win_width / 2 + radius || position.x > win_width / 2 - radius) {
		velocity.x *= -1;
	}
	if (position.y<-win_height / 2 + radius || position.y > win_height / 2 - radius) {
		velocity.y *= -1;
	}

}