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
	friction(0),
	name("")
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
	friction(0),
	name("")
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
		position = glm::vec3({ float(rand() % (win_width - 50) - win_width / 2),  float(rand() % (win_height - 50) - win_height / 2), 0 });
		velocity = glm::vec3(0);
		acceleration = glm::vec3(0);
		scale = glm::vec3(1);
		mass = 1;

		if (name == "airplane1") {

			mass = 1.5;
			radius = sqrt(800);
		}
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
			getIntoBlackhole();
		}
	}


	position += velocity * dtime + 0.5f * (acceleration + friction) * dtime * dtime;

	velocity += (acceleration + friction) * dtime;

	if (name == "cocktail1") {
		orientation += 0.01f;
	}
	else
	{
		if (velocity != glm::vec3(0) && length(velocity) > 1)
			orientation = std::atan2(velocity.y, velocity.x) + base_orientation;
	}

	if (position.x<-win_width / 2 + radius) 
	{
		velocity.x *= -1;
		position.x += 10;
	}
	else if (position.x > win_width / 2 - radius) 
	{
		velocity.x *= -1;
		position.x -= 10;
	}
	else if (position.y<-win_height / 2 + radius)
	{
		velocity.y *= -1;
		position.y += 10;
	}
	else if (position.y > win_height / 2 - radius)
	{
		velocity.y *= -1;
		position.y -= 10;
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
		{	{ -185 , -213 } ,
			{ -213 , -193 } ,
			{ -216 , -162 } ,
			{ -241 , -146 } ,
			{ -274 , -148 } ,
			{ -291 , -124 } ,
			{ -291 , -81 } ,
			{ -277 , -41 } ,
			{ -252 , -14 } ,
			{ -240 , 10 } ,
			{ -155 , -236 } ,
			{ -118 , -247 } ,
			{ -76 , -249 } ,
			{ -16 , -242 } ,
			{ 14 , -209 } ,
			{ 45 , -178 } ,
			{ 79 , -149 } ,
			{ 80 , -97 } ,
			{ 61 , -49 } ,
			{ 92 , -20 } ,
			{ 98 , 13 } ,
			{ 91 , 58 } ,
			{ 60 , 70 } ,
			{ 8 , 74 } ,
			{ -34 , 75 } ,
			{ -235 , 16 } ,
			{ -239 , 49 } ,
			{ -228 , 73 } ,
			{ -199 , 93 } ,
			{ -148 , 78 } ,
			{ -169 , 86 } ,
			{ -127 , 77 } ,
			{ -74 , 89 } ,
			{ -54 , 83 } ,
			{ -101 , 84 } ,
		},

		{
			{ -113 , -234 } ,
			{ -157 , -225 } ,
			{ -189 , -210 } ,
			{ -206 , -196 } ,
			{ -211 , -175 } ,
			{ -218 , -154 } ,
			{ -236 , -136 } ,
			{ -254 , -120 } ,
			{ -265 , -102 } ,
			{ -268 , -56 } ,
			{ -233 , -32 } ,
			{ -221 , -12 } ,
			{ -197 , 9 } ,
			{ -180 , 26 } ,
			{ -139 , 46 } ,
			{ -102 , 63 } ,
			{ -80 , 63 } ,
			{ -50 , 42 } ,
			{ -24 , 29 } ,
			{ 5 , 4 } ,
			{ 25 , -12 } ,
			{ 43 , -28 } ,
			{ 52 , -78 } ,
			{ 51 , -109 } ,
			{ 37 , -151 } ,
			{ 16 , -186 } ,
			{ 9 , -201 } ,
			{ -15 , -214 } ,
			{ -18 , -219 } ,
			{ -55 , -231 }
		},

		{
			{ -238 , -82 } ,
			{ -224 , -49 } ,
			{ -216 , -25 } ,
			{ -199 , -9 } ,
			{ -184 , 9 } ,
			{ -158 , 24 } ,
			{ -114 , 43 } ,
			{ -75 , 37 } ,
			{ -36 , 22 } ,
			{ -19 , -6 } ,
			{ 13 , -10 } ,
			{ 39 , -47 } ,
			{ 46 , -88 } ,
			{ 39 , -125 } ,
			{ 17 , -161 } ,
			{ -14 , -195 } ,
			{ -53 , -213 } ,
			{ -100 , -220 } ,
			{ -140 , -214 } ,
			{ -169 , -190 } ,
			{ -196 , -170 } ,
			{ -217 , -133 } ,
			{ -240 , -110 }
		},
		{
			{ -143 , -195 } ,
			{ -171 , -164 } ,
			{ -186 , -145 } ,
			{ -208 , -116 } ,
			{ -214 , -79 } ,
			{ -200 , -49 } ,
			{ -175 , -16 } ,
			{ -145 , 6 } ,
			{ -104 , 20 } ,
			{ -80 , 12 } ,
			{ -50 , 5 } ,
			{ -15 , -17 } ,
			{ 14 , -49 } ,
			{ 18 , -93 } ,
			{ -3 , -121 } ,
			{ -25 , -132 } ,
			{ -123 , -186 } ,
			{ -38 , -127 } ,
			{ -55 , -109 } ,
			{ -68 , -87 } ,
			{ -85 , -76 } ,
			{ -101 , -73 } ,
			{ -116 , -79 } ,
			{ -130 , -95 } ,
			{ -136 , -115 } ,
			{ -126 , -138 } ,
			{ -112 , -149 } ,
			{ -112 , -172 }
		},
		{
			{ -3 , -111 } ,
			{ 4 , -94 } ,
			{ 11 , -68 } ,
			{ 6 , -47 } ,
			{ -4 , -32 } ,
			{ -31 , -13 } ,
			{ -51 , 3 } ,
			{ -16 , -115 } ,
			{ -37 , -112 } ,
			{ -48 , -94 } ,
			{ -62 , -82 } ,
			{ -76 , -67 } ,
			{ -96 , -62 } ,
			{ -111 , -67 } ,
			{ -126 , -73 } ,
			{ -136 , -95 } ,
			{ -140 , -113 } ,
			{ -150 , -127 } ,
			{ -165 , -133 } ,
			{ -179 , -130 } ,
			{ -189 , -119 } ,
			{ -196 , -101 } ,
			{ -196 , -89 } ,
			{ -193 , -66 } ,
			{ -179 , -51 } ,
			{ -163 , -38 } ,
			{ -149 , -21 } ,
			{ -134 , -15 } ,
			{ -121 , -7 } ,
			{ -96 , -1 } ,
			{ -76 , 5 }
		},
		{
			{ -15 , -86 } ,
			{ -12 , -70 } ,
			{ -16 , -54 } ,
			{ -23 , -43 } ,
			{ -41 , -29 } ,
			{ -59 , -19 } ,
			{ -71 , -14 } ,
			{ -83 , -8 } ,
			{ -104 , -8 } ,
			{ -115 , -16 } ,
			{ -21 , -88 } ,
			{ -32 , -83 } ,
			{ -56 , -68 } ,
			{ -65 , -58 } ,
			{ -80 , -50 } ,
			{ -98 , -45 } ,
			{ -104 , -44 } ,
			{ -119 , -43 } ,
			{ -127 , -49 } ,
			{ -135 , -61 } ,
			{ -143 , -76 } ,
			{ -153 , -94 } ,
			{ -167 , -101 } ,
			{ -183 , -93 } ,
			{ -185 , -70 } ,
			{ -169 , -53 } ,
			{ -152 , -43 } ,
			{ -141 , -32 } ,
			{ -127 , -28 }
		},
		{
			{ -172 , -85 } ,
			{ -172 , -73 } ,
			{ -163 , -63 } ,
			{ -154 , -65 } ,
			{ -160 , -78 }
		}, 
		{
			{ -94 , -26 } ,
			{ -75 , -26 } ,
			{ -55 , -33 } ,
			{ -42 , -41 } ,
			{ -34 , -50 } ,
			{ -32 , -63 } ,
			{ -44 , -64 } ,
			{ -52 , -58 } ,
			{ -65 , -51 } ,
			{ -77 , -46 } ,
			{ -95 , -41 } ,
			{ -96 , -35 }
		},
		{
			{ -86 , -31 } ,
			{ -76 , -31 } ,
			{ -61 , -35 } ,
			{ -58 , -38 } ,
			{ -54 , -44 } ,
			{ -61 , -44 } ,
			{ -68 , -41 } ,
			{ -74 , -38 }
		},
		{
			{ -132 , -130 } ,
			{ -132 , -103 } ,
			{ -127 , -86 } ,
			{ -116 , -75 } ,
			{ -93 , -74 } ,
			{ -71 , -84 } ,
			{ -51 , -106 } ,
			{ -51 , -130 } ,
			{ -63 , -150 } ,
			{ -80 , -162 } ,
			{ -95 , -163 } ,
			{ -112 , -158 } ,
			{ -120 , -149 }
		},

		{
			{ -120 , -133 } ,
			{ -123 , -117 } ,
			{ -124 , -100 } ,
			{ -115 , -87 } ,
			{ -102 , -81 } ,
			{ -86 , -85 } ,
			{ -71 , -90 } ,
			{ -64 , -110 } ,
			{ -61 , -123 } ,
			{ -67 , -144 } ,
			{ -79 , -151 } ,
			{ -95 , -156 } ,
			{ -108 , -150 } ,
		},
		{{0,0}}
	};

	for (auto& f : figures)
	{
		for (auto& p : f)
		{
			p.first += 100; 
			p.second += 100;
		}
	}

	colors = {
		{44 / 255.0f, 0 / 255.0f, 1 / 255.0f},
		{104 / 255.0f, 11 / 255.0f, 1 / 255.0f},
		{144 / 255.0f, 27 / 255.0f, 0 / 255.0f},
		{211 / 255.0f, 89 / 255.0f, 0 / 255.0f},
		{237 / 255.0f, 5 / 117, 6 / 255.0f},
		{253 / 255.0f, 174 / 255.0f, 38 / 255.0f},
		{255 / 255.0f, 232 / 255.0f, 127 / 255.0f},
		{255 / 255.0f, 231 / 255.0f, 114 / 255.0f},
		{255 / 255.0f, 248 / 255.0f, 214 / 255.0f},
		{119 / 255.0f, 31 / 255.0f, 0 / 255.0f},
		{77 / 255.0f, 10 / 255.0f, 1 / 255.0f},
		{77 / 255.0f, 10 / 255.0f, 1 / 255.0f}
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
			getIntoBlackhole();
		}
	}


	position += velocity * dtime + 0.5f * (acceleration + friction) * dtime * dtime;
	velocity += (acceleration + friction) * dtime;

	orientation = base_orientation;


	position.x += rand() % 2 - 0.5;
	position.y += rand() % 2 - 0.5;

	if (velocity != glm::vec3(0) && length(velocity) > 1)
			orientation = std::atan2(velocity.y, velocity.x) + base_orientation;

	if (position.x < -win_width / 2 + radius)
	{
		velocity.x *= -1;
		position.x += 10;
	}
	else if (position.x > win_width / 2 - radius)
	{
		velocity.x *= -1;
		position.x -= 10;
	}
	else if (position.y < -win_height / 2 + radius)
	{
		velocity.y *= -1;
		position.y += 10;
	}
	else if (position.y > win_height / 2 - radius)
	{
		velocity.y *= -1;
		position.y -= 10;
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
		position = glm::vec3({ float(rand() % (win_width - 50) - win_width / 2),  float(rand() % (win_height - 50) - win_height / 2), 0 });
		velocity = glm::vec3(0);
		acceleration = glm::vec3(0);

		mass = 10;
		scale = { 3, 1, 0 };
	}
}


void Car::move() {

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

			shear += shear_constant;

			if (shear > 1) {
				shear_constant *= -1;
			}
			else if (shear < -1) {
				shear_constant *= -1;
			}

		}
		else
		{
			getIntoBlackhole();
		}
	}


	position += velocity * dtime + 0.5f * (acceleration + friction) * dtime * dtime;

	velocity += (acceleration + friction) * dtime;

	


	if (position.x<-win_width / 2 + radius) 
	{
		velocity.x *= -1;
		position.x += 10;
	}
	else if (position.x > win_width / 2 - radius) 
	{
		velocity.x *= -1;
		position.x -= 10;
	}
	else if (position.y<-win_height / 2 + radius)
	{
		velocity.y *= -1;
		position.y += 10;
	}
	else if (position.y > win_height / 2 - radius)
	{
		velocity.y *= -1;
		position.y -= 10;
	}
}