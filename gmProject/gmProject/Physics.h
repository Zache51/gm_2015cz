#pragma once

#include "MeshHolder.hpp"
#include "FPSCounter.hpp"

class Physics
{
private:
	float delta_t;
	float delta_v = 0.0f;
	float delta_v0 = delta_v;
	vec3 delta_s = vec3(0.0f, 0.0f, 0.0f);
	vec3 delta_s0 = delta_s;
	const float mass = 25.0f;
	const float g = 9.82f;

	vec3 freeFall() const;

public:
	Physics();
	~Physics();

	void move(MeshHolder* mesh, fpsCounter* fpsC, vec3& pos);
};

