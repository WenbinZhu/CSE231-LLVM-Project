#include <stdbool.h>

int f(int v) {
	int a, b;
	
	a = b = 7;

	if (v < 14) {
		a  = 9;
		b = 5 / v;
	}

//	else  {
//		b = 5 / v;
//	}

	return a + b;
}
