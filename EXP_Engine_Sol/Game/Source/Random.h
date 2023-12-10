#ifndef _RANDOM_H_
#define _RANDOM_H_
#include "Globals.h"

struct Random {
public:
	Random() {};
	Random(uint num);
	~Random() {};

	uint GenerateRandomIntRange(int first, int last);
	uint GenerateRandomInt();

private:
	uint num;

};
#endif // ! _RANDOM_H_