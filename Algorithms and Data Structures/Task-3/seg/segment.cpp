#include <iostream>

namespace dst {
	template <class T>
		struct __quick_swp_default {
		       	
			void operator() (T* array, size_t left, size_t right) {
				T swp = array[left];
				array[left] = array[right];
				array[right] = swp;
			}
		};

	template <class T>
		struct __quick_cmp_default {

			bool operator() (T& left, T& right) {
				return left < right;
			}
		};

	template <class T, class C, class S>
		void __quick_partition (
					T* array, size_t size,
					size_t& middle, size_t& middle_size,
					C cmp_function, S swp_function
				       ) {
			
			swp_function (array, middle, size-1);

			size_t i = 0;
			size_t j = size - 1;

			size_t middle_array = size - 1;

			while (true) {
				while ( (i < size - 1) && (middle_array > i) &&
				        !cmp_function (array[size-1], array[i]) ) {
					
					if (!cmp_function (array[i], array[size-1])) {
						
						if (j == middle_array)
							--j;
						
						swp_function (array, i, --middle_array);
					} else {
						++i;
					}
				}

				while ( (j > 0) && // TODO: check this!
					!cmp_function (array[j-1], array[size-1]) ) {
					
					if (!cmp_function (array[size-1], array[j-1])) {
						swp_function (array, j-1, --middle_array);
					} else {
						--j;
					}
				}

				if (i == j) {
					middle = j;
					middle_size = 0;

					while (middle_array < size) {
						swp_function (array, j++, middle_array++);
						++middle_size;
					}

					return;
				}

				swp_function (array, i, j-1);
			}
		}

	template <class T, class C, class S>
		void __quick_s_sort (
				     T* array, size_t size,
				     C cmp_function, S swp_function
				    ) {
			
			for (size_t i = 0; i < size-1; ++i) {
				for (size_t j = i+1; j < size; j++) {
					if (cmp_function (array[j], array[i])) {
						swp_function (array, j, i);
					}
				}
			}
		}

	template <class T, class C, class S>
		size_t __quick_select (
				       T* array, size_t size,
				       size_t k,
				       C cmp_function, S swp_function
				      );
	
	template <class T, class C, class S>
		size_t __quick_middle (
				       T* array, size_t size,
				       C cmp_function, S swp_function,
				       const unsigned int part_size = 5
				      ) {
			
			if (size < part_size) {
				return 0;
			}

			for (size_t i = 0; i < size / part_size; ++i) {
				__quick_s_sort (
					        &(array[part_size * i]),
					        part_size,
					        cmp_function, swp_function
				   	       );

				swp_function (
					      array,
					      part_size*i + part_size / 2, i
					     );
			}

			return __quick_select (
					       array, size / part_size,
					       size / (2*part_size),
					       cmp_function, swp_function
					      );
		}

	template <class T, class C, class S>
		size_t __quick_select (
				       T* array, size_t size,
				       size_t k,
				       C cmp_function, S swp_function
				      ) {
			if (size == 1) { return 0; }

			size_t middle = __quick_middle (
							array, size,
							cmp_function, swp_function
						       );
			size_t middle_size;

			__quick_partition (
					   array, size, middle, middle_size,
					   cmp_function, swp_function
					  );
			if (k < middle) {
				return __quick_select (
						       array, middle, k,
						       cmp_function, swp_function
						      );
			}

			if (k < middle + middle_size) {
				return middle;
			}

			return middle + middle_size +
			       __quick_select (
					       &(array[middle + middle_size]),
					       size - (middle + middle_size),
					       k - (middle + middle_size),
					       cmp_function, swp_function
					      );
		}

	template <class T, class C = __quick_cmp_default<T>, class S = __quick_swp_default<T>>
		void quick_sort (
				 T* array, size_t size,
				 C cmp_function = __quick_cmp_default<T>(),
				 S swp_function = __quick_swp_default<T>()
				) {
			if (size <= 1) { return; }

			size_t middle = __quick_middle (
							array, size,
							cmp_function, swp_function
						       );
			size_t middle_size;

			__quick_partition (
					   array, size, middle, middle_size,
					   cmp_function, swp_function
					  );

			quick_sort (array, middle, cmp_function, swp_function);
			quick_sort (
				    &(array[middle + middle_size]),
				    size - (middle + middle_size),
				    cmp_function, swp_function
				   );
		}
}

struct nested_sort {
	size_t* unsorted;
	size_t* sorted;
	size_t* other_end;

	bool operator() (size_t left, size_t right) {
		return unsorted[left] < unsorted[right];
	}

	void swap (size_t* sorted, size_t left, size_t right) {
		size_t swp = sorted[right];
		sorted[right] = sorted[left];
		sorted[left] = swp;

		if (other_end[left] != right) {
			other_end[other_end[left]] = right;
			other_end[other_end[right]] = left;

			swp = other_end[right];
			other_end[right] = other_end[left];
			other_end[left] = swp;
		}
	}
};

struct nested_swap {
	nested_sort* ns;
	void operator () (size_t* array, size_t left, size_t right) {
		ns->swap(array, left, right);
	}
};

size_t get_nested_segments (size_t* left, size_t* right, size_t size) {
	size_t* unsorted = new size_t [2*size];
	size_t* sorted = new size_t [2*size];
	size_t* other_end = new size_t [2*size];

	for (size_t i = 0; i < size; ++i) {
		unsorted[2*i] = left[i];
		unsorted[2*i+1] = right[i];

		sorted[2*i] = 2*i;
		sorted[2*i+1] = 2*i+1;

		other_end[2*i] = 2*i + 1;
		other_end[2*i+1] = 2*i;
	}
	
	nested_sort ns;
	nested_swap ns_swp;

	ns.unsorted = unsorted;
	ns.sorted = sorted;
	ns.other_end = other_end;

	ns_swp.ns = &ns;

	dst::quick_sort<size_t, nested_sort, nested_swap> (
							   sorted, 2*size,
							   ns, ns_swp
							  );
	return 0;
}

#define SIZE 4

int main () {
	size_t left[] = {1, 5, 3, 9};  //= new size_t [SIZE];
	size_t right[] = {6, 2, 8, 4}; //= new size_t [SIZE];

	get_nested_segments (left, right, 4);

	return 0;
}
