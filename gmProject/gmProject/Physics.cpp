#include "Physics.h"


Physics::Physics()
{
}


Physics::~Physics()
{
}

void Physics::move(MeshHolder* mesh, fpsCounter* fpsC, vec3& pos)
{
	//const float mass = 25.0f;
	//const float g = 9.82f;
	//float delta_v = 0.0f;
	//float delta_v0 = delta_v;
	//vec3 delta_s = vec3(0.0f, 0.0f, 0.0f);
	//vec3 delta_s0 = delta_s;
	delta_t = fpsC->deltaTime();
	if (pos.y > 0 && delta_t > 0)
	{
		delta_v = delta_v0 + g * delta_t;
		delta_s.y = delta_s0.y + delta_v * delta_t;

		pos -= delta_s;

		delta_v0 = delta_v;
		delta_s0 = delta_s;
	}

	mesh->SetTranslation(glm::translate(mat4(1.0f), pos));
}
