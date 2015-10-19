#pragma once

#include "MeshHolder.hpp"
#include "FPSCounter.hpp"

class Physics
{
private:
	struct Cannon
	{
		float alpha; // Y-axis angle
		float gamma; // X & Z angle

		float muzzle_v; // Muzzle velocity
	};

	float delta_t;
	vec3 delta_v = vec3(0.0f);
	vec3 delta_v0 = delta_v;
	vec3 delta_s = vec3(0.0f);
	vec3 delta_s0 = delta_s;

	const float mass = 50.0f;
	const float g = 9.82f;
	const float FORCE = 1500;

	float acc() const;
	float vel() const;

	vec3 freeFall();
	vec3 qwer();

public:
	Physics();
	~Physics();

	void move(MeshHolder* mesh, fpsCounter* fpsC);
};

