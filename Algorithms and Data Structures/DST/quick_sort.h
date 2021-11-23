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
