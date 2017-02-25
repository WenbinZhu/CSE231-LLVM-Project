int calc_fib(int f) {
	if (f==0) return 1;
	if (f==1) return 1;

	int fibo = 1;
	int prevFibo = 1;
	for (int i=2; i<=f; i++) {
		int temp = fibo;
		fibo += prevFibo;
		prevFibo = temp;
		
	}

	return fibo;
}