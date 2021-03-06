#include "FPSCounter.hpp"
#include <iostream>
using std::stringstream;

fpsCounter::fpsCounter()
{
	t = time(0);
	localtime_s(&previous, &t);
}

fpsCounter::~fpsCounter()
{

}

// return the fps in variable
std::string fpsCounter::get() const
{
	stringstream ss;
	ss << "FPS: " << m_fps;
	
	return ss.str();
}

void fpsCounter::tick()
{
	m_fpscount++;

	// one second elapsed? (= 1000 milliseconds)
	t = time(0);
	localtime_s(&current, &t);
	if (current.tm_sec - previous.tm_sec > 1)
	{
		// save the current counter value to m_fps
		m_fps = m_fpscount;

		// reset the counter and the interval
		m_fpscount = 0;
		localtime_s(&previous, &t);

		dTime = 1 / (float)m_fps; // Seconds per tick
	}
}

float fpsCounter::deltaTime()
{
	return dTime;
}