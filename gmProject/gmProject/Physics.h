#pragma once

#include "MeshHolder.hpp"
#include "FPSCounter.hpp"
#include "Line.h"

class Physics
{
private:
	float delta_t;
	const float toRad = 3.141592f / 180.0f;
	const float g = 9.82f;

	// Cannonball
	float vel; // Total velocity
	float alpha; // Angle in radians
	float gamma;
	float degreeAngle;
	float degreeRotate;
	float resistance;
	vec3 dVelocity;
	vec3 dVelocity0;
	vec3 dAccel;
	vec3 dAccel0;

	void reset();
	
public:
	Physics();
	~Physics();

	void move(MeshHolder* mesh, fpsCounter* fpsC);
	void move(Line* line, fpsCounter* fpsC);

	void angleCannon(float change);
	void rotateCannon(float change);
};

