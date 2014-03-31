/*
 * BSD license by inclusion of servparse.cpp from NetBSD,
 * see the file servparse.cpp for details.  This file
 * assumes the most permissive available and applicable 
 * license given the restrictions therein.
 */

#include <stdio.h>
#include <sys/time.h>
#include "servparse.h"

long timer()
{
	static long start = 0, end = 0;
	long diff;
	timeval time;

	gettimeofday(&time, NULL);
	end  = ((unsigned long long)time.tv_sec * 1000000 + time.tv_usec);
	diff = end - start;
	start = end;
	return diff;
}


int main() 
{
	int ix, iy;

	for(iy = 1; iy < 10; iy++) 
	{
		timer();
		for(ix = 0; ix < 50000; ix++) 
		{
			findByName(findByPort(ix, "U"));
		}
		printf("Lookup Time: %ld \n", timer());
	}
	return (0);
}
