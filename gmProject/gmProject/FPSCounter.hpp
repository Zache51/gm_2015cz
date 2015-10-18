#ifndef FPSCOUNTER_HPP
#define FPSCOUNTER_HPP

#include <time.h>
#include <sstream>
#include <string>

class fpsCounter
{
public:
	fpsCounter();
	~fpsCounter();

	// get fps and tick function
	std::string get() const;
	void tick();
	float deltaTime(); 

private:

	unsigned int m_fps = 0;
	unsigned int m_fpscount = 0;

	// time for when to update fps
	time_t t;
	tm current;
	tm previous;

	float dTime;
};

#endif