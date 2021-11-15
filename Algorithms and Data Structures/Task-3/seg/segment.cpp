#include <iostream>

namespace dst {
	template <class T>
		struct __quick_swp_default {
		       	
			void operator() (T& left, T& right) {
				T swp = left;
				left = right;
				right = swp;
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
			
			swp_function (array[middle], array[size-1]);

			size_t i = 0;
			size_t j = size - 1;

			size_t middle_array = size - 1;

			while (true) {
				while ( (i < size - 1) && (middle_array > i) &&
				        !cmp_function (array[size-1], array[i]) ) {
					
					if (!cmp_function (array[i], array[size-1])) {
						
						if (j == middle_array)
							--j;
						
						swp_function (array[i], array[--middle_array]);
					} else {
						++i;
					}
				}

				while ( (j > 0) && // TODO: check this!
					!cmp_function (array[j-1], array[size-1]) ) {
					
					if (!cmp_function (array[size-1], array[j-1])) {
						swp_function (array[j-1], array[--middle_array]);
					} else {
						--j;
					}
				}

				if (i == j) {
					middle = j;
					middle_size = 0;

					while (middle_array < size) {
						swp_function (array[j++], array[middle_array++]);
						++middle_size;
					}

					return;
				}

				swp_function (array[i], array[j-1]);
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
						swp_function (array[j], array[i]);
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
					      array[part_size*i + part_size / 2],
                                    array[i]
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

namespace dst {
      template <class T>
            struct __fenwick_default {
                  T operator() (const T& left, const T& right) {
                        return left + right;
                  }
            };

      template <class T, class F = __fenwick_default<T>>
            class fenwick {

                  public:
                        fenwick (T* array, size_t size, F fenwick_function);
                        fenwick (T zero, size_t size, F fenwick_function = __fenwick_default<T>());
                        ~fenwick ();
                        
                        void modify (size_t idx, const T& value);
                        T count (size_t idx);

                  private:
                        T* fenwick_array;
                        F fenwick_function;

                        size_t size;

                        size_t f (size_t idx);
                        size_t g (size_t idx);
            };

      template <class T, class F>
            size_t fenwick<T,F>::f (size_t idx) {
                  return idx & (idx+1);
            }

      template <class T, class F>
            size_t fenwick<T,F>::g (size_t idx) {
                  return idx | (idx+1);
            }

      template <class T, class F>
            T fenwick<T,F>::count (size_t idx) {
                  
                  if (idx == 0) {
                        return fenwick_array[0];
                  }

                  T res = fenwick_array[idx];
                  idx = f(idx)-1;

                  for (; f(idx) > 0; idx = f(idx)-1) {
                        res = fenwick_function (fenwick_array[idx], res);
                  }

                  res = fenwick_function (fenwick_array[idx], res);

                  return res;
            }

      template <class T, class F>
            void fenwick<T,F>::modify (size_t idx, const T& value) {
                  for (; idx < size; idx = g(idx)) {
                        fenwick_array[idx] = fenwick_function (fenwick_array[idx], value);
                  }
            }

      template <class T, class F>
            fenwick<T,F>::fenwick (T zero, size_t size, F fenwick_function) {
                  fenwick_array = new T[size];
                  this->size = size;

                  this->fenwick_function = fenwick_function;

                  for (size_t i = 0; i < size; i++) {
                        this->fenwick_array[i] = zero;
                  }
            }

      template <class T, class F>
            fenwick<T,F>::~fenwick () {
                  delete [] fenwick_array;
            }
}

struct nested_sort {
	size_t* unsorted;
	size_t* sorted;
	size_t* other_end;

	bool operator() (size_t left, size_t right) {
            if (sorted[left] == sorted[right]) {
                  if (sorted[other_end[left]] != sorted[other_end[right]])
                        return sorted[other_end[left]] > sorted[other_end[right]];

                  std::cerr << "Error!\n";
                  exit(-1);
            }

		return sorted[left] < sorted[right];
	}

	void swap (size_t& left, size_t& right) {
		size_t swp = sorted[left];
		sorted[left] = sorted[right];
            sorted[right] = swp;

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
	void operator () (size_t& left, size_t& right) {
		ns->swap(left, right);
	}
};

size_t get_nested_segments (size_t* left, size_t* right, size_t size) {
	size_t* unsorted = new size_t [2*size];
	size_t* sorted = new size_t [2*size];
	size_t* other_end = new size_t [2*size];

	for (size_t i = 0; i < size; ++i) {
		sorted[2*i] = left[i];
		sorted[2*i+1] = right[i];

		unsorted[2*i] = 2*i;
		unsorted[2*i+1] = 2*i+1;

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
							   unsorted, 2*size,
							   ns, ns_swp
							  );
      size_t* reps new size_t[size];
      reps[0] = 1;
      
      size_t new_size = 1;

      for (size_t i = 1; i < size; ++i) {
            if (
                (sorted[i] == sorted[i-1]) &&
                (sorted[other_end[i]] == sorted[other_end[i-1]])
               ) {
                  ++reps[new_size-1];
            }
      }

	return 0;
}

#define SIZE 4

int main () {
	size_t left[] = {1, 5, 3, 9};  //= new size_t [SIZE];
	size_t right[] = {6, 2, 8, 4}; //= new size_t [SIZE];

	//get_nested_segments (left, right, 4);

      dst::fenwick<int> F (0, 10);
      F.modify(5, 1);
      std::cout << F.count(8)-F.count(4) << '\n';

	return 0;
}
