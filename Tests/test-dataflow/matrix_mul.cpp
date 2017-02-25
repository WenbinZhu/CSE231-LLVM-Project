#include <cstdlib>

void matrix_mul() {
	
	int a[10][10], b[10][10], mult[10][10];

	// Make sure they all get the same test
	srand(7);

	// Initialize arrays with random ints in (0..100)
	for (int i=0; i<10; i++) {
		for (int j=0; j<10; j++) {
			mult[i][j] = 0;
			a[i][j] = rand() % 100;
			b[i][j] = rand() % 100;
		}
	}

	// Perform naive multiplication
	for (int i=0; i<10; i++) {
		for (int j=0; j<10; j++) {
			for (int k=0; k<10; k++) {
				mult[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}