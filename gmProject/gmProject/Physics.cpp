#include "Physics.h"


Physics::Physics()
{
	

	degreeAngle = 5.0f;
	alpha = degreeAngle * toRad;

	degreeRotate = 90.0f;
	gamma = degreeRotate * toRad;

	resistance = -0.1f;
	reset();
}
Physics::~Physics(){}

void Physics::move(MeshHolder* mesh, fpsCounter* fpsC)
{
	delta_t = fpsC->deltaTime();
	if (mesh->GetPosition().y > 0)
	{
		vec3 asdf;
		//asdf += xComp();
		//asdf += yComp();
		//printf("%f ", asdf.y);
		printf(" %f ", asdf.z);
		mesh->UpdatePosition(asdf);
	}	
}
void Physics::move(Line* line, fpsCounter* fpsC)
{
	delta_t = fpsC->deltaTime();
	vec3 asdf = vec3(line->GetLastPoint());
	if (asdf.y > 0)
	{
		dVelocity = dVelocity0 + dAccel0 * delta_t;

		vel = glm::sqrt(dVelocity.x * dVelocity.x + dVelocity.y * dVelocity.y);
		alpha = glm::atan(dVelocity.y / dVelocity.x);

		dAccel0.x = resistance * vel * vel * glm::cos(alpha) * cos(gamma);
		dAccel0.y = resistance * vel * vel * glm::sin(alpha) - g;
		dAccel0.z = resistance * vel * vel * glm::cos(alpha) * sin(gamma);

		dVelocity0 = dVelocity;

		asdf += dVelocity;

		line->AddPoint(asdf);
	}
}








void Physics::angleCannon(float change)
{
	degreeAngle += change;

	alpha = degreeAngle * toRad;

	reset();
}

void Physics::rotateCannon(float change)
{
	degreeRotate += change;

	gamma = degreeRotate * toRad;

	alpha = degreeAngle * toRad;

	reset();
}

void Physics::reset()
{
	vel = 6.0f;

	dVelocity = vec3(vel * cos(alpha) * cos(gamma), vel * sin(alpha), vel * cos(alpha) * sin(gamma));
	dVelocity0 = dVelocity;
	dAccel = vec3(0.0f, -g, 0.0f);
	dAccel0 = dAccel;
}