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
                                    result = tree_function (result, nodes_array[l]);
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
                                                      result,
                                                      nodes_array[__get_left_child (l) + 1]
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

class min {
      public:
            long long operator() (long long a, long long b) {
                  if (a < b) { return a; }
                  return b;
            }
};

class max {
      public:
            long long operator() (long long a, long long b) {
                  if (a > b) { return a; }
                  return b;
            }
};

#define ARRAY_SIZE 1000000

int main () {
      
      long long* a = new long long [ARRAY_SIZE];

      for (size_t i=0; i < ARRAY_SIZE; i++) {
            long long x = (i % 12345);
            x = (x*x) % 12345;

            long long y = (i % 23456);
            long long z = (y*y) % 23456;
            y = (y*z) % 23456;

            a[i] = x+y;
      }

      dst::segment_tree<long long, min> T_min (a, ARRAY_SIZE, min());
      dst::segment_tree<long long, max> T_max (a, ARRAY_SIZE, max());

      size_t N;
      std::cin >> N;

      for (size_t i = 0; i < N; ++i) {
            long long x, y;
            std::cin >> x >> y;
            
            if (x > 0) {
                  ++y;
                  std::cout << T_max.get(x, y) - T_min.get(x, y) << '\n';
            
            } else {
                  x = -x;
                  T_min.insert(y, x);
                  T_max.insert(y, x);
            }
      }

      return 0;
}
