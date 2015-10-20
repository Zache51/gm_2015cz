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

	// Drag force on sphere
	const float dC = 0.47f; // Drag coefficient - Sphere
	float radius = 0.12f;
	float area = 3.141592f * radius * radius; // Cross-section
	float volume = (4 * 3.141592f * radius*radius*radius) / 3;
	float density = 7800; // Iron
	float mass = volume * density;
	// Cannonball
	float vel; // Total velocity
	float alpha; // Angle in radians
	float gamma;
	float degreeAngle;
	float degreeRotate;
	float resistance;
	vec3 dVelocity;
	vec3 lastDVelocity;
	vec3 dAccel;

	void reset();

	vec3 lastDAccel;
	
public:
	Physics();
	~Physics();

	void move(MeshHolder* mesh, fpsCounter* fpsC);
	void move(Line* line, fpsCounter* fpsC);

	void angleCannon(float change);
	void rotateCannon(float change);
};

