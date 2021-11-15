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

#ifndef __HEADER__SEGMENT_TREE
#define __HEADER__SEGMENT_TREE

#include <cstddef>

namespace dst {
      template <class T, class C>
            class segment_tree {
                  public:
                        segment_tree (T* array, size_t size, C tree_function);
                        segment_tree (T& f, size_t size, C tree_function);
                        ~segment_tree ();

                        void insert (T& value, size_t position);
                        T get (size_t first, size_t last);
                  private:

                        C tree_function;

                        size_t size;

                        T* nodes_array;
                        size_t nodes_size;
                        size_t nodes_depth;

                        T* __get_layer (size_t layer);
                        size_t __get_layer_offset (size_t layer);
                        void __get_size (size_t size);
                        size_t __get_parent (size_t index);
                        size_t __get_left_child (size_t index);
            };

      template <class T, class C>
            segment_tree<T,C>::segment_tree (T& f, size_t size, C tree_function) {
                  
                  this->tree_function = tree_function;

                  __get_size (size);
                  nodes_array = new T[nodes_size];

                  for (size_t i = __get_layer_offset (nodes_depth - 1); i < nodes_size; ++i) {
                        nodes_array[i] = f;
                  }

                  for (size_t i = __get_layer_offset (nodes_depth - 1); i > 0; --i) {

                        size_t left_child = __get_left_child (i-1);
                        size_t right_child = left_child + 1;

                        nodes_array[i-1] = tree_function (
                                                          nodes_array[left_child],
                                                          nodes_array[right_child]
                                                         );
                  }
            }

      template <class T, class C>
            segment_tree<T,C>::segment_tree (T* array, size_t size, C tree_function) {
                  
                  this->tree_function = tree_function;

                  __get_size (size);
                  nodes_array = new T[nodes_size];

                  size_t i;
                  
                  for (i = 0; i < size; ++i) {
                        nodes_array[__get_layer_offset (nodes_depth - 1) + i] = array[i];
                  }

                  for (i = __get_layer_offset (nodes_depth-1) + size; i < nodes_size; ++i) {
                        nodes_array[i] = array[0];
                  }

                  for (size_t i = __get_layer_offset (nodes_depth - 1); i > 0; --i) {

                        size_t left_child = __get_left_child (i-1);
                        size_t right_child = left_child + 1;

                        nodes_array[i-1] = tree_function (
                                                          nodes_array[left_child],
                                                          nodes_array[right_child]
                                                         );
                  }

            }

      template <class T, class C>
            segment_tree<T,C>::~segment_tree () {
                  delete [] nodes_array;
            }

      template <class T, class C>
            void segment_tree<T,C>::__get_size (size_t size) {
                  nodes_depth = 1;

                  for (--size; size > 0; size >>= 1) {
                        ++nodes_depth;
                  }

                  this->size = 1 << (nodes_depth - 1);
                  nodes_size = (1 << nodes_depth) - 1;
            }

      template <class T, class C>
            size_t segment_tree<T,C>::__get_layer_offset (size_t layer) {
                  return (1 << layer) - 1;
            }

      template <class T, class C>
            size_t segment_tree<T,C>::__get_parent (size_t index) {
                  return (index - 1) / 2;
            }

      template <class T, class C>
            size_t segment_tree<T,C>::__get_left_child (size_t index) {
                  return index*2 + 1;
            }

      template <class T, class C>
            void segment_tree<T,C>::insert (T& value, size_t position) {
                  position = __get_layer_offset (nodes_depth - 1) + position;

                  nodes_array[position] = value;

                  while (position > 0) {

                        size_t parent = __get_parent (position);
                        size_t left_child = __get_left_child (parent);
                        size_t right_child = left_child + 1;

                        nodes_array[parent] = tree_function (
                                                             nodes_array[left_child],
                                                             nodes_array[right_child]
                                                            );

                        position = parent;
                  }
            }

      template <class T, class C>
            T segment_tree<T,C>::get (size_t left, size_t right) {
                  size_t l = 0;
                  size_t r = 0;

                  size_t l_begin = 0;
                  size_t l_end = size;

                  size_t r_begin = 0;
                  size_t r_end = size;

                  while (true) {

                        if ((l_begin == left) && (l_end == right)) {
                              return nodes_array[l];
                        }

                        r_end = l_end;
                        
                        r_begin = l_end = (l_begin + l_end) / 2;

                        if ((l_begin <= left) && (l_end >= right)) {
                              
                              l = __get_left_child (l);
                              
                              continue;
                        }

                        if ((r_begin <= left) && (r_end >= right)) {

                              l_begin = r_begin;
                              l_end = r_end;
                              l = __get_left_child (l) + 1;
                              
                              continue;
                        }
                        
                        l = __get_left_child (l);
                        r = l + 1;

                        break;
                  }

                  T result;
                  bool is_inited = false;

                  while (true) {
                        
                        if (l_begin == left) {
                              if (!is_inited) {
                                    result = nodes_array[l];
                              
                              } else {
                                    result = tree_function (nodes_array[l], result);
                              }
                              
                              break;
                        }

                        size_t middle = (l_begin + l_end) / 2;
                        
                        if (middle <= left) {
                              l_begin = middle;
                              l = __get_left_child (l) + 1;

                              continue;
                        }

                        l_end = middle; 

                        if (!is_inited) {
                              is_inited = true;
                              
                              result = nodes_array[__get_left_child (l) + 1];
                        } else {
                              
                              result = tree_function (
                                                      nodes_array[__get_left_child (l) + 1],
                                                      result
                                                     );
                        }

                        l = __get_left_child (l);
                  }

                  while (true) {
                        
                        if (r_end == right) {
                              result = tree_function (result, nodes_array[r]);
                              break;
                        }

                        size_t middle = (r_begin + r_end) / 2;

                        if (middle >= right) {
                              r = __get_left_child (r);
                              r_end = middle;

                              continue;
                        }

                        r_begin = middle;

                        result = tree_function (
                                                result,
                                                nodes_array[__get_left_child (r)]
                                               );

                        r = __get_left_child (r) + 1;
                  }

                  return result;
            }
}

#endif

#include <iostream>
#include <bits/stdc++.h>


struct segment_end {
      size_t* array;
      size_t idx;

      bool operator==(segment_end& a) {
            return (array[idx] == array[a.idx]) && (array[idx+1] == array[a.idx+1]);
      }

      bool operator< (segment_end& a) {
            if (array[idx] != array[a.idx]) { return array[idx] < array[a.idx]; }
            if (idx % 2) { return array[idx - 1] > array[a.idx - 1]; }
            return array[idx + 1] > array[a.idx + 1];
      }
};

struct tree_fcn {
      size_t operator() (size_t a, size_t b) {return a+b;}
};

int main () {
      std::ios_base::sync_with_stdio(false);
      std::cin.tie(NULL);

      size_t N;
      std::cin >> N;

      size_t* end_array = new size_t[2*N];
      bool* is_left = new bool[2*N];

      segment_end* sorted = new segment_end[2*N];

      for (size_t i=0; i < N; i++) {
            std::cin >> end_array[2*i] >> end_array[2*i+1];

            sorted[2*i].idx = 2*i;
            sorted[2*i + 1].idx = 2*i+1;

            sorted[2*i].array = sorted[2*i + 1].array = end_array;
      }

      dst::quick_sort<segment_end> (sorted, 2*N);
      size_t* line = new size_t[2*N];
      size_t* tree = new size_t[2*N];
      size_t* reverse_ptr = new size_t[2*N];

      for (size_t i = 0; i < 2*N; ++i) {
            line[i] = sorted[i].idx;
            reverse_ptr[sorted[i].idx] = i;
            tree[i] = 0;
      }
      
      dst::segment_tree<size_t, tree_fcn> T (tree[0], 2*N, tree_fcn());
      size_t s = 0;

      for (size_t i=0; i < 2*N; i++) {
            if (line[i] % 2) {
                  size_t pair_idx = line[i]-1;
                  size_t x;
            
                  s += T.get (reverse_ptr[pair_idx], i);
                  x = T.get (reverse_ptr[pair_idx], reverse_ptr[pair_idx]+1);
                  ++x;

                  T.insert(x, reverse_ptr[pair_idx]);
                  
            }
      }

      std::cout << s << '\n';

      return 0;
}
