#include <iostream>
#include <bits/stdc++.h>

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
				
				if (
                              (!cmp_function (array[size-1], array[j-1])) &&
                              (j < middle_array)
                             ) {
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
                    fenwick (const T& zero, size_t size, F fenwick_function = __fenwick_default<T>());
                    ~fenwick ();
                    
                    void modify (size_t idx, const T& value);
                    T count (size_t idx);
                    T count (size_t left, size_t right);

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
              
              if (f(idx) == 0) {
                    return fenwick_array[idx];
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
        T fenwick<T,F>::count (size_t left, size_t right) {
              if (left == 0) {
                    return count(right);
              }

              return count(right) - count(left-1);
        }

  template <class T, class F>
        void fenwick<T,F>::modify (size_t idx, const T& value) {
              for (; idx < size; idx = g(idx)) {
                    fenwick_array[idx] = fenwick_function (fenwick_array[idx], value);
              }
        }

  template <class T, class F>
        fenwick<T,F>::fenwick (const T& zero, size_t size, F fenwick_function) {
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

struct segment {
  size_t left;
  size_t right;

  bool operator< (segment& other) {
        if (right != other.right)
              return right < other.right;
        return left > other.left;
  }

  bool operator== (segment& other) {
        return (left == other.left) && (right == other.right);
  }
};

long long get_nested (segment* s, size_t N) {

  dst::quick_sort<struct segment> (s, N);

  size_t* sorted = new size_t [N];

  for (size_t i = 0; i < N; i++) {
        sorted[i] = s[i].left;
  }

  size_t length = 1;

  dst::quick_sort<size_t> (sorted, N);
  
  for (size_t i=1; i < N; ++i) {
        if (sorted[i] != sorted[i-1]) {
              sorted[length++] = sorted[i];
        }
  }
  
  dst::fenwick<size_t> f(0, length);
  long long res = 0;
  long long correction;

  for (size_t i=0; i < N; ++i) {
        
        if ((i > 0) && (s[i] == s[i-1]))
              correction += 1;
        else
              correction = 0;


        size_t left = 0;
        size_t right = length;

        while (right-left > 1) {
              size_t middle = (left + right) / 2;

              if (sorted[middle] <= s[i].left)
                    left = middle;
              else
                    right = middle;
        }
        
        size_t left_end = left;
        
        left = 0;
        right = length;

        while (right-left > 1) {
              size_t middle  =(left+right)/2;

              if (sorted[middle] <= s[i].right)
                    left = middle;
              else
                    right = middle;
        }

        size_t right_end = left;

        res += (f.count (left_end, right_end) - correction);
        f.modify (left_end, 1);
  }

  delete [] sorted;

  return res;
}

int main () {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie (NULL);

  size_t N;
  std::cin >> N;
  
  segment* array = new segment[N];

  for (size_t i=0; i < N; ++i) {
        std::cin >> array[i].left >> array[i].right;
  }
  

  std::cout << get_nested (array, N) << '\n';

  delete [] array;
}
