/*
 * BSD license by inclusion of servparse.cpp from NetBSD,
 * see the file servparse.cpp for details.  This file
 * assumes the most permissive available and applicable 
 * license given the restrictions therein.
 */

#include <stdio.h>
#include <sys/time.h>
#include "servparse.h"

long avg(long value = 0) {
	static long running = 0, count = 0, avg;

	if(value == 0 && count > 0)
	{
		avg = running / count;
		count = 0;
		running = 0;
		return avg * 2;
	} 
	else 
	{
		count ++;
		running += value;
		return 0;
	}
}

#define START  0
#define END    1
#define RESET  2

long sink(int action) {
	static long start = 0, end = 0, diff = 0;
	long ret = 0;

	if (action == RESET)
	{
		ret = diff;
		diff = 0;
		return ret;
	}

	timeval time;

	if(action == START)
	{
		gettimeofday(&time, NULL);
		start  = ((unsigned long long)time.tv_sec * 1000000 + time.tv_usec);
		return 0;
	}

	if(action == END)
	{
		gettimeofday(&time, NULL);
		end  = ((unsigned long long)time.tv_sec * 1000000 + time.tv_usec);
		diff +=	end - start;
		return 0;
	}
}

long timer()
{
	static long start = 0, end = 0;
	long diff;

	timeval time;
	gettimeofday(&time, NULL);
	end  = ((unsigned long long)time.tv_sec * 1000000 + time.tv_usec);

	diff = (end - start - sink(RESET));
	if(start > 0) {
		avg(diff);
	}

	start = end;
	return diff;
}


int main() 
{
	int ix, iy, res;
	int runs = 1200;
	int err = 0, succ = 0;
	string name;

	for(iy = 0; iy < runs; iy++) 
	{
		err = 0; succ = 0;
		timer();
		for(ix = 0; ix < 50000; ix++) 
		{
			name = service::findByPort(ix, "T");
			res = service::findByName(name);

/*
			if(res > 0) {
				if(res != ix) 
				{
					err++;
					//printf("Error: %d != %d (%s)\n", res, ix, name.c_str());
				} 
				else 
				{
					succ++;
				}
			}
			*/
		}
		//printf("Error: %d\nSuccess: %d\n", err, succ);
		timer();
	}
	printf("Direct Average: %ld\n", avg());

	string in;
	for(iy = 0; iy < runs; iy++) 
	{
		err = 0; succ = 0;
		timer();
		for(ix = 0; ix < 50000; ix++) 
		{
			sink(START);
			in = to_string(ix);
			sink(END);

			name = service::toName(in);
			res = service::toPort(name);
			/*
			if(!name.empty()) {
				res = service::toPort(name);

				if(res > 0) {
					if(res != ix) 
					{
						err++;
						//printf("Error: %d != %d (%s)\n", res, ix, name.c_str());
					} 
					else 
					{
						succ++;
					}
				}
			}
			*/
		}
		//printf("Error: %d Success: %d\n", err, succ);
		timer();
	}
	printf("Magic Average: %ld\n", avg());

	return (0);
}
