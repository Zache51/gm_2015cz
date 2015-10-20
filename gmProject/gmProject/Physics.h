#pragma once

#include "MeshHolder.hpp"
#include "FPSCounter.hpp"
#include "Line.h"

class Physics
{
private:
	struct Cannon
	{
		float alpha; // Y-axis angle
		float gamma; // X & Z angle

		float length;
		float elev; // Elevation of base
	};
	Cannon can;

	float atime = 0.0f;
	float delta_t;
	vec3 delta_v = vec3(0.0f);
	vec3 delta_v0 = delta_v;
	vec3 delta_s = vec3(0.0f);
	vec3 delta_s0 = delta_s;

	const float degree = 3.141592f / 180.0f;
	const float g = 9.82f;
	const float mass = 5.0f;
	const float FORCE = 150;

	float cosY;

	float acc() const;
	float vel() const;

	vec3 yComp();
	vec3 xComp();

public:
	Physics();
	~Physics();

	void move(MeshHolder* mesh, fpsCounter* fpsC);
	void move(Line* line, fpsCounter* fpsC);
};

