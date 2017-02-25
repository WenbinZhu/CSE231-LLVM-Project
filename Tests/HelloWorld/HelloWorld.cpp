#include <iostream>

using namespace std;

int main () {
	int var = 1;

	for (int i = 0; i < 5; i++) {
		if (var % 2 == 0)
			var++;
	}

	return 0;
}
