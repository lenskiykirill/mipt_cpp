#include "segment_tree.h"
#include "../Task-2/quick_sort.h"

#include <iostream>

struct segment {
	size_t left;
	size_t right;

	bool operator==(segment& b) {
		return (left == b.left);
	}

	bool operator< (segment& b) {
		return left < b.left;
	}
};

struct tree_function {
	segment operator()(const segment& a, const segment& b) {
		
	}
};

int main () {
	size_t N;

	std::cin >> N;
	segment* segment_array = new segment[N];

	for (size_t i = 0; i < N; i++) {
		std::cin >> segment_array[i].left >> segment_array[i].right;
	}

	dst::quick_sort<segment> (segment_array, N);

	dst::segment_tree<segment, tree_function> T (segment_array, N, tree_function());


	
	for (size_t i = 0; i < N; i++) {
		std::cout << segment_array[i].left << ' ' << segment_array[i].right << '\n';
	}

	return 0;
}
