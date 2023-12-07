#pragma once
#include "Globals.h"

struct Random 
{
public:
	Random() {};
	Random(uint num);
	~Random() {};

	uint GenerateRandomIntRange(int first, int last);
	uint GenerateRandomInt();

private:
	uint num;

};
