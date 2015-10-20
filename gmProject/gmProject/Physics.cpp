#include "Physics.h"


Physics::Physics()
{
	vel = 40.0f;
	alpha = 85.0f * toRad;
	resistance = -0.1f;
	dVelocity = vec3(vel * cos(alpha), vel * sin(alpha), 0.0f);
	dVelocity0 = dVelocity;
	dAccel = vec3(0.0f, -g, 0.0f);
	dAccel0 = dAccel;
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

		dAccel.x = resistance * vel * vel * glm::cos(alpha);
		dAccel.y = resistance * vel * vel * glm::sin(alpha) - g;

		dAccel0 = dAccel;
		dVelocity0 = dVelocity;

		asdf += dVelocity;

		line->AddPoint(asdf);
	}
}