#include <iostream>

bool is_enough_hits (unsigned int N, unsigned long long *hp, unsigned int size, unsigned long long p, unsigned long long q) {
	unsigned long long direct_hit_sum = 0; // number to which the direct hits to any monster are added

	for (int i=0; i < size; i++) {
		if (hp[i] > q*N) {
			direct_hit_sum += ((hp[i] - q*N - 1) / (p - q)) + 1;
		}
	}

	return direct_hit_sum <= N;
}

unsigned int find_min_hits (unsigned long long *hp, unsigned int size, unsigned long long p, unsigned long long q) {
	unsigned int healthiest_monster = 0;
	
	for (int i=0; i < size; i++) {
		if (hp[i] > healthiest_monster) {
			healthiest_monster = hp[i];
		}
	}

	if (p == q) {
		return ((healthiest_monster - 1) / p) + 1;
	}

	unsigned int left = 0;
	unsigned int right = healthiest_monster;// / q + 1;

	while (right - left > 1) {
		unsigned int middle = (right + left) / 2;

		if (is_enough_hits (middle, hp, size, p, q)) {
			right = middle;
		} else {
			left = middle;
		}
	}
	
	return right;
}

int main() {
	unsigned int n, p, q;
	std::cin >> n >> p >> q;

	unsigned long long *hp = new unsigned long long [n];

	for (int i=0; i < n; i++) {
		std::cin >> hp[i];
	}

	std::cout << find_min_hits (hp, n, p, q) << std::endl;

	delete hp;

	return 0;
}
