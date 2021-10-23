#include <iostream>
#include <bits/stdc++.h>

namespace dst {

	template <class T>
		void swap(T& left, T& right) {

			T swp = left;

			left = right;
			right = swp;

			return;
		}

	template <class T>
		class __cmp_default {
			public:
				bool operator() (const T& left, const T& right) {
					return left < right;
				}
		};

	template <class T, class C = __cmp_default<T>>
                size_t __quick_select (T* array, size_t size, size_t k, C is_less = __cmp_default<T>());

	template <class T, class C>
		size_t __quick_partition (T* array, size_t size, size_t mid_index, C is_less) {

			size_t i = 0;
			size_t j = size - 1;

			swap (array[mid_index], array[size - 1]);

			while (true) {
				while ( (i < size - 1) && (is_less (array[i], array[size-1]) ) ) { ++i; }

				while ( (j > 0) && !(is_less (array[j - 1], array[size-1]) ) ) { --j; }

				if (i == j) {
					swap (array[j], array[size-1]);
					return j;
				}

				swap (array[i], array[j-1]);
			}

			return 0; // This must mean error
		}

	template <class T, class C>
		size_t __quick_s_middle (T* array, size_t size, C is_less = __cmp_default<T>()) {

			for (size_t i = 0; i <= size / 2; i++) {
				for (size_t j=i+1; j < size; j++) {
					if (array[j] < array[i]) {
						swap(array[i], array[j]);
					}
				}
			}
			return size / 2;
		}

	template <class T, class C>
		size_t __quick_middle (T* array, size_t size, C is_less, const int part_size = 5) {

			if (size < part_size) {
				return 0;
			}

			for (int i=0; i < size / part_size; i++) {
				__quick_s_middle<T, C> ( &array [part_size * i], part_size, is_less );

				swap(array[part_size * i + (part_size / 2)], array[i]);
			}

			return __quick_select<T, C> (array, size / part_size, size / (2*part_size), is_less);
		}

	template <class T, class C>
		size_t __quick_r_middle (T* array, size_t size, C is_less) {
			return size / 2;
		}

	template <class T, class C>
		size_t __quick_select (T* array, size_t size, size_t k, C is_less) {

			if (size == 1) {
				return 0;
			}

			size_t middle = __quick_middle (array, size, is_less);

			middle = __quick_partition<T, C> (array, size, middle, is_less);

			if (k <= middle) {
				return __quick_select (array, middle+1, k, is_less);
			}

			return __quick_select (&array[middle+1], size-middle-1, k-middle-1, is_less) + middle + 1;
		}

	template <class T, class C = __cmp_default<T>>
		void quick_sort (T* array, size_t size, C is_less = __cmp_default<T>()) {
			if (size < 1) {
				return;
			}

			size_t middle = __quick_middle (array, size, is_less);

			middle = __quick_partition<T, C> (array, size, middle, is_less);
			
			quick_sort(&(array[0]), middle, is_less);
			quick_sort(&(array[middle+1]), size-(middle+1), is_less);

			return;
		}

	template <class T, class C = __cmp_default<T>>
		const T& quick_select (T* array, size_t size, size_t k, C is_less = __cmp_default<T>()) {

			size_t pos = __quick_select (array, size, k, is_less);

			return array[pos];
		}
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	size_t n;
	std::cin >> n;
	
	int* array = new int[n];

	for (size_t i=0; i < n; i++) {
		std::cin >> array[i];
	}

	dst::quick_sort(array, n);

	for (size_t i=0; i < n; i++) {
		std::cout << array[i] << " ";
	}
	std::cout << "\n";

	return 0;
}
