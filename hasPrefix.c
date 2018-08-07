#include "hasPrefix.h"

int hasPrefix(char const *p, char const *q)
{
	int i = 0;
	for(i = 0;q[i];i++)
	{
		if(p[i] != q[i])
			return -1;
	}
	return 0;
}
