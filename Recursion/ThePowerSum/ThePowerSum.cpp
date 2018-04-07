#include <iostream>
#include <cmath>

int powerSum(int x, int n, int num) {
	// New x value for the first recursive call.
	int newX = x - pow(num, n);
	// Case for no solutions.
	if (newX < 0) {
		return 0;
	}
	// The solution case.
	else if (newX == 0) {
		return 1;
	}
	// Otherwise make the sum of recursive calls for the newX and old x values.
	else {
		return powerSum(newX, n, num + 1) + powerSum(x, n, num + 1);
	}
}

int main() {
	int x{}, n{};

	// Read input from std::cin.
	std::cin >> x >> n;
	// Get the powerSum() result.
	int result = powerSum(x, n, 1);
	// Output a single integer result.
	std::cout << result << std::endl;
}