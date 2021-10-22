#include <iostream>
#include <bits/stdc++.h>

template <class T>
	void swap(T& a, T& b) {
		T tmp = a;
		a = b;
		b = tmp;

		return;
	}

template <class T>
	T& max (T& a, T& b) {
		if (a > b) {
			return a;
		}
		return b;
	}

template <class T>
	void sift_up (T* array, size_t idx) {

		while ((idx > 0) && (array[idx] > array[(idx-1) / 2])) {
			swap<T>(array[idx], array[(idx-1) / 2]);
			idx = (idx-1) / 2;
		}
		return;
	}



template <class T>
	void sift_down (T* array, size_t array_size, size_t idx) {
		size_t left = 2*idx + 1;
		size_t right = left + 1;

		while ((right < array_size)) {

			size_t max_idx = left;

			if (array[max_idx] < array[right]) {
				max_idx = right;
			}

			if (array[idx] > array[max_idx]) {
				break;
			}

			swap(array[idx], array[max_idx]);
			idx = max_idx;

			left = 2*idx + 1;
			right = left + 1;
		}

		if (left < array_size) {
			if (array[idx] < array[left]) {
				swap(array[idx], array[left]);
			}
		}

		return;
	}

void get_k_min(size_t n, size_t k, unsigned long int a, unsigned long int x, unsigned long int y, unsigned long int* heap) {

	for (size_t i=0; i < k; i++) {
		a = (a*x + y) % (1 << 30);
		heap[i] = a;
		sift_up(heap, i);
	}

	for (size_t i=k; i < n; i++) {
		a = (a*x + y) % (1 << 30);
		heap[k] = a;
		sift_up(heap, k);

		swap(heap[0], heap[k]);

		sift_down(heap, k, 0);
	}

	while (k > 0) {
		swap(heap[0], heap[k-1]);
		sift_down(heap, k-1, 0);
		--k;
	}

	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	size_t n, k;
	std::cin >> n >> k;

	unsigned long int a, x, y;
	std::cin >> a >> x >> y;

	unsigned long int* heap = new unsigned long int [k+1];

	get_k_min(n, k, a, x, y, heap);

	for (size_t i=0; i < k; i++) {
		std::cout << heap[i] << "\n";
	}

	delete [] heap;

	return 0;
}
