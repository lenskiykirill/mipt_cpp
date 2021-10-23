#include <iostream>

template <class T>
	void swap (T& a, T& b) {
		T tmp = a;
		a = b;
		b = tmp;

		return;
	}

template <class T>
	size_t quick_partition (T* array, size_t size, size_t middle) {

		swap (array[middle], array[size-1]);
		
		size_t i = 0;
		size_t j = size-1;

		while (j > i) {
			if (array[i] < array[size-1]) {
				++i;
			
			} else if (array[j] >= array[size-1]) {
				--j;
			
			} else {
				swap(array[i], array[j]);
			}
		}

		swap (array[j], array[size]);

		return j;
	}

int main() {
	int a[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

	quick_partition(a, 10, 4);

	for (int i=0; i < 10; i++) {
		std::cout << a[i] << " ";
	}
	std::cout << "\n";

	return 0;
}
