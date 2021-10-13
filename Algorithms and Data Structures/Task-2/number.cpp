#include <fstream>
#include <iostream>
#include "new_heap.h"
#include <string>

bool is_less(const std::string a, const std::string b) {
	unsigned int i=0;

	while ((i < a.size()) && (i < b.size())) {
		
		if (a[i] < b[i]) {
			return true;
		}
		
		if (a[i] > b[i]) {
			return false;
		}

		++i;
	}
	
	if (a.size() > b.size()) {
		return a[i] < b[0];
	}

	if (a.size() < b.size()) {
		return a[0] < b[i];
	}

	return false;
}

int main() {
	dst::heap<std::string> heap(is_less);

	std::ifstream istream("number.in");

	std::string a;
	unsigned int N = 0;

	while (istream >> a) {

		heap.insert(a);
		++N;
	}
	
	std::string *ans = heap.sort();

	for (unsigned int i=0; i < N; i++) {
		std::cout << ans[i];
	}

	return 0;
}
