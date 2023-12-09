#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"
#include "../External/SDL/include/SDL.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	void Stop();
	void ReStart();
	void Resume();
	void Pause();
	bool IsRunning();

	Uint32 Read();

private:

	bool	running = false;
	Uint32	started_at;
	Uint32	stopped_at;
};

#endif //__TIMER_H__