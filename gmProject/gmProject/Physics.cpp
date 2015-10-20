#include "Physics.h"


Physics::Physics()
{
	can = Cannon();
	can.alpha = 45.0f * degree;
	can.gamma = 0.0f * degree;

}


Physics::~Physics(){}

void Physics::move(MeshHolder* mesh, fpsCounter* fpsC)
{
	delta_t = fpsC->deltaTime();
	if (mesh->GetPosition().y > 0)
	{
		vec3 asdf;
		asdf += xComp();
		asdf += yComp();
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
		asdf += xComp();
		asdf += yComp();
		//printf("%f ", asdf.y);
		//printf(" %f ", atime);
		line->AddPoint(asdf);
		atime += delta_t;
	}
}

vec3 Physics::xComp()
{
	delta_s.x = vel();

	return delta_s;
}
vec3 Physics::yComp()
{
	cosY = glm::cos(can.alpha);

	delta_v.y = delta_v0.y - g * delta_t;
	delta_s.y = delta_s0.y + delta_v.y * delta_t;

	delta_v0.y = delta_v.y;
	delta_s0.y = delta_s.y;

	return delta_s;
}

float Physics::acc() const
{
	// N2: F = m * a
	return (FORCE / mass);
}
float Physics::vel() const
{
	// dv = a * dt
	return (acc() * delta_t);
}