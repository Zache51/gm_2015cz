#include "Physics.h"

float square(float value)
{
	float s = value*value;
	return s;
}

float pytagoras(float a, float b)
{
	float c2 = square(a) + square(b);
	float c = glm::sqrt(c2);
	return c;
}

Physics::Physics()
{
	degreeAngle = 75.0f;
	alpha = degreeAngle * toRad;

	degreeRotate = 0.0f;
	gamma = degreeRotate * toRad;

	reset();
	resistance = (-dC * 0.5f * density * area) / mass;
}
Physics::~Physics(){}

void Physics::move(MeshHolder* mesh, fpsCounter* fpsC)
{
	delta_t = fpsC->deltaTime();
	if (mesh->GetPosition().y > 0)
	{
		dVelocity = lastDVelocity + lastDAccel * delta_t;

		float xz = pytagoras(dVelocity.x, dVelocity.z);

		vel = pytagoras(xz, dVelocity.y);

		float aa = dVelocity.y / xz;
		float bb = glm::atan(aa);
		float cc = alpha - bb;

		alpha = glm::atan(dVelocity.y / xz);

		dAccel.x = resistance * square(vel) * glm::cos(alpha) * cos(gamma);
		dAccel.y = resistance * square(vel) * glm::sin(alpha) - g;
		dAccel.z = resistance * square(vel) * glm::cos(alpha) * sin(gamma);

		float a = square(vel);
		float b = glm::cos(alpha);
		float c = sin(gamma);


		lastDAccel = dAccel;
		lastDVelocity = dVelocity;

		mesh->UpdatePosition(dVelocity);
	}	
}
void Physics::move(Line* line, fpsCounter* fpsC)
{
	delta_t = fpsC->deltaTime();
	vec3 asdf = vec3(line->GetLastPoint());
	if (asdf.y > 0)
	{
		dVelocity = lastDVelocity + lastDAccel * delta_t;

		float xz = pytagoras(dVelocity.x, dVelocity.z);

		vel = pytagoras(xz, dVelocity.y);

		float aa = dVelocity.y / xz;
		float bb = glm::atan(aa);
		float cc = alpha - bb;

		alpha = glm::atan(dVelocity.y / xz);

		dAccel.x = resistance * square(vel) * glm::cos(alpha) * cos(gamma);
		dAccel.y = resistance * square(vel) * glm::sin(alpha) - g;
		dAccel.z = resistance * square(vel) * glm::cos(alpha) * sin(gamma);
		
		float a = square(vel);
		float b = glm::cos(alpha);
		float c = sin(gamma);


		lastDAccel = dAccel;
		lastDVelocity = dVelocity;

		asdf += dVelocity;

		line->AddPoint(asdf);
	}
}








void Physics::angleCannon(float change)
{
	degreeAngle += change;

	if (degreeAngle > 90)
	{
		degreeAngle = 90;
	}

	if (degreeAngle > 360)
	{
		degreeAngle -= 360;
	}
	else if (degreeAngle < -360)
	{
		degreeAngle += 360;
	}


	alpha = degreeAngle * toRad;

	reset();
}

void Physics::rotateCannon(float change)
{
	degreeRotate += change;

	if (degreeRotate > 360)
	{
		degreeRotate -= 360;
	}
	else if (degreeRotate < -360)
	{
		degreeRotate += 360;
	}

	gamma = degreeRotate * toRad;

	alpha = degreeAngle * toRad;

	reset();
}

void Physics::reset()
{
	vel = 3.0f;

	dVelocity = vec3(vel * cos(alpha) * cos(gamma), vel * sin(alpha), vel * cos(alpha) *sin(gamma));
	lastDVelocity = dVelocity;
	dAccel = vec3(0.0f, -g, 0.0f);
	lastDAccel = dAccel;
}