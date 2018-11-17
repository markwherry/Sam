#include "AMath.h"

#include <math.h>

namespace Atom {

float OOshift23=1.0/(1<<23);

float AMath::log2(float i)
{
	// Too inprecise, even for tuning
	/*float LogBodge=0.346607f;
	float x;
	float y;
	x=*(int *)&i;
	x*= OOshift23; //1/pow(2,23);
	x=x-127;

	y=x-floorf(x);
	y=(y-y*y)*LogBodge;
	return x+y;*/

	return log(i)/log(2.0f);
}

float AMath::pow(float a, int b)
{
	// A simple, naive implementation (but still faster than math.h for this operation).
	// To be improved...
	float out = a;
	while(b>1) {
		out *= a;
		b--;
	}

	return out;
}

/*float AMath::pow(float a, float b)
{
	
	return AMath::pow(b*log2(a), 2);
}*/

}