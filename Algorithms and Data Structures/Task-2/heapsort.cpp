#include <iostream>
#include "heap.h"

bool is_less(int a, int b) {return a > b;}

int main() {
	int N;
	std::cin >> N;
	int *a = new int[N];
	for (int i=0; i < N; i++)
		std::cin >> a[i];

	dst::heap<int, bool (*)(int, int)> heap(a, N, is_less);

	heap.heapify();
	int *b = heap.sort();

	for (int i=0; i < N; i++)
		std::cout << b[i] << " ";
	std::cout << std::endl;

	return 0;
}
