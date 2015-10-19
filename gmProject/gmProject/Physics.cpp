#include "Physics.h"


Physics::Physics()
{
	Cannon can = Cannon();
	can.alpha = 45.0f;
	can.gamma = 0.0f;

}


Physics::~Physics()
{
}

void Physics::move(MeshHolder* mesh, fpsCounter* fpsC)
{
	delta_t = fpsC->deltaTime();
	if (mesh->GetPosition().y > 0)
	{
		vec3 asdf;
		asdf += qwer();
		asdf += freeFall();
		//printf("%f ", asdf.y);
		printf(" %f ", asdf.z);
		mesh->UpdatePosition(asdf);
	}	
}

vec3 Physics::freeFall()
{
	delta_v.y = delta_v0.y - g * delta_t;
	delta_s.y = delta_s0.y + delta_v.y * delta_t;

	delta_v0.y = delta_v.y;
	delta_s0.y = delta_s.y;

	return delta_s;
}
vec3 Physics::qwer()
{
	//delta_v.z = delta_v0.z + (FORCE / mass) * delta_t;
	//delta_s.z = delta_s0.z + delta_v.z * delta_t;

	//delta_v0.z = delta_v.z;
	//delta_s0.z = delta_s.z;
	delta_s.z = vel();

	return delta_s;
}

float Physics::acc() const
{
	return (FORCE / mass);
}
float Physics::vel() const
{
	return (acc() * delta_t);
}