#include <cstddef>

namespace dst {
	template <class T>
		void swap (T& first, T& second) {
			T swp = first;
			first = second;
			second = swp;

			return;
		}

	template <class T>
		class __cmp_default {
			public:
				bool operator() (T& first, T& second) { return first < second; }
		};

	template <class T, class C>
		void __quick_partition (T* array, size_t size, size_t &middle, size_t& middle_size, C cmp_function) {
			
			swap (array[middle], array[size-1]);

			size_t i = 0;
			size_t j = size - 1;

			size_t middle_array = size - 1;
			
			while (true) {
				
				while ( (i < size - 1) &&
					 cmp_function (array[i], array[size-1]) ) {

					if (array[i] == array[size-1]) {
						swap (array[i], array[--middle_array]);
					
					} else {
						++i;
					}
				}

				while ( (j > 0) &&
					!cmp_function (array[j-1], array[size-1]) ) {
					
					if (array[j-1] == array[size-1]) {
						swap (array[j-1], array[--middle_array]);
					}

					--j;
				}

				if (i == j) {
					middle = j;
					middle_size = 0;

					while (middle_array < size) {
						swap (array[j++], array[middle_array++]);
						++middle_size;
					}

					return;
				}

				swap (array[i], array[j-1]);
			}

			return;
		}

	template <class T, class C>
		void __quick_s_sort (T* array, size_t size, C is_less) {
			
			for (size_t i = 0; i < size -1; i++) {
				for (size_t j = i+1; j < size; j++) {
					if ( is_less (array[j], array[i]) ) {

						swap (array[j], array[i]);
					}
				}
			}
			
			return;
		}

	template <class T, class C = __cmp_default<T>>
		size_t __quick_select ( T* array, size_t size,
				        size_t k, C cmp_function = __cmp_default<T>());

	template <class T, class C>
		size_t __quick_middle (T* array, size_t size,
				       C cmp_function,
				       const int part_size = 5) {

			if (size < part_size) {
				return 0;
			}

			for (size_t i=0; i < size / part_size; i++) {
				__quick_s_sort<T,C> ( &(array[part_size * i]),
						      part_size,
						      cmp_function );

				swap ( array[part_size*i + part_size/2],
				       array[i] );

			}

			return __quick_select<T,C> ( array, size / part_size,
						     size / (2*part_size),
						     cmp_function );
		}

	template <class T, class C>
		size_t __quick_select (T* array, size_t size, size_t k, C cmp_function) {
			
			if (size == 1) { return 0; }

			size_t middle = __quick_middle<T,C> (array, size, cmp_function);
			size_t middle_size;

			__quick_partition<T,C> (array, size, middle, middle_size, cmp_function);

			if (k < middle) {
				return __quick_select<T,C> (array, middle, k, cmp_function);
			}

			if (k < middle + middle_size) {
				return middle;
			}

			return (middle + middle_size) + 
				__quick_select<T,C> ( &(array[middle+middle_size]),
						     size - (middle + middle_size),
						     k - (middle + middle_size),
						     cmp_function);
		}

	template <class T, class C = __cmp_default<T>>
		void quick_sort (T* array, size_t size, C cmp_function = __cmp_default<T>()) {

			if (size <= 1) { return; }
			
			size_t middle = __quick_middle<T,C> (array, size, cmp_function);
			size_t middle_size;

			__quick_partition<T,C> (array, size, middle, middle_size, cmp_function);

			quick_sort<T,C> (array, middle, cmp_function);	

			quick_sort<T,C> (&(array[middle + middle_size]),
					 size - (middle + middle_size),
					 cmp_function);

			return;
		}

}

#include <iostream>
#include <bits/stdc++.h>

size_t circle_dist (size_t first, size_t second, size_t L) {
	if ( second > first ) {
		return second - first;
	}

	return L + (second - first);
}

size_t circle_add (size_t idx, size_t delta, size_t L) {

	if ( (L - idx) > delta ) {
		return idx + delta;
	}

	return idx - (L - delta);
}

size_t circle_sub (size_t idx, size_t delta, size_t L) {
	if (delta <= idx) {
		return idx - delta;
	}

	return L - (delta - idx);
}

size_t get_next_k (size_t idx, size_t k, size_t* circle, size_t N, size_t L) {
	size_t left = idx;
	size_t right = idx;

	while (circle_dist(left, right, N) > 1) {
		size_t middle = circle_add(left, circle_dist(left, right, N)/2, N);

		if (circle_dist(circle[idx], circle[middle], L) >= k) {
			right = middle;
		
		} else {
			left = middle;
		}
		
	}

	return right;
}

#include <iostream>

class my_cmp {
	private:
		size_t* bound;
	public:
		my_cmp (size_t* bound) {

			this->bound = bound;
		}

		bool operator() (size_t left, size_t right) {

			return bound[left] < bound[right];
		}
};

int main() {
//	std::ios_base::sync_with_stdio(false);
//	std::cin.tie(NULL);

	size_t N, L, k;
	std::cin >> L >> N >> k;
	
	size_t* circle = new size_t[N];

	for (size_t i = 0; i < N; i++) {
		std::cin >> circle[i];
	}

	dst::quick_sort(circle, N);

	size_t* next_k = new size_t[N];

	for (size_t i = 0; i < N; i++) {
		next_k[i] = get_next_k(i, k, circle, N, L);
	}

	for (size_t i=0; i < N; i++) {
		std::cout << next_k[i] << " ";
	}
	std::cout << "\n";
	
	size_t* bound = new size_t [2*N];
	size_t* values = new size_t [N];

	for (size_t i = 0; i < N; i++) {
		size_t right_bound = circle_dist(circle[circle_sub(next_k[i], 1, N)], circle_add(circle[i], k, L), L);
		right_bound = right_bound - 1;

		size_t left_bound = circle_dist(circle[circle_sub(i, 1, N)], circle[i], L);
		left_bound = left_bound - 1;

		size_t min_bound = right_bound;
		
		if (left_bound < right_bound) {
			min_bound = left_bound;
		}
		bound[2*i + 1] = circle_add(circle[i], 1, L);
		bound[2*i] = circle_sub (circle[i], min_bound, L);

		values[i] = circle_dist (i, next_k[i], N);
	}
	
	my_cmp cmp (bound);

	size_t* circle_split = new size_t [2*N];

	for (size_t i = 0; i < 2*N; i++) {
		circle_split[i] = i;
	}

	dst::quick_sort (circle_split, N, cmp);

	size_t* bound_split = new size_t [2*N];

	for (size_t i = 0; i < 2*N; i++) {
		bound_split[circle_split[i]] = i;
	}

		

	for (size_t i = 0; i < 2*N; i++) {
		std::cout << circle_split[i] << " ";
	}
	std::cout << "\n";

	for (size_t i = 0; i < 2*N; i++) {
		std::cout << bound[i] << " ";
	}

			

	std::cout << "\n";

	return 0;
}
