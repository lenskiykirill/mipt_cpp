#include <cstddef>

namespace dst {

	template <class T>
		class __cmp_default {
			public:
				bool operator() (const T& left, const T& right) { return left < right; }
		};

	template <class T, class C = __cmp_default<T>>
		class double_heap {
			public:
				double_heap (C cmp_function = __cmp_default<T>());
				~double_heap ( void );

				// double_heap (T* min_heap, size_t min_heap_size, T* max_heap, size_t max_heap_size);

				 const T& get_min ( void );
				 const T& get_max ( void );

				const T extract_min ( void );
				const T extract_max ( void );
				
				void insert ( const T& value );

				void clear ( void );

			private:
				T* min_heap;
				T* max_heap;
				
				size_t heap_size;
				size_t heap_allocated;

				size_t* ptr_min_max;
				size_t* ptr_max_min;

				C cmp_function;
				
				bool min_cmp (const T& left, const T& right) { return cmp_function (left, right); }
				bool max_cmp (const T& left, const T& right) { return !cmp_function (left, right); }

				void sift_up ( size_t idx, T* array, size_t size, size_t* ptr_array_aux,
					       size_t* ptr_aux_array, bool (double_heap<T,C>::*cmp_function) (const T&, const T&) );

				void sift_down ( size_t idx, T* array, size_t size, size_t* ptr_array_aux,
						 size_t* ptr_aux_array, bool (double_heap<T,C>::*cmp_function) (const T&, const T&));
				
				void heap_swap (size_t first, size_t second, T* array, size_t* ptr_array_aux, size_t* ptr_aux_array);

				void expand (void);
		};

	template <class T, class C>
		void double_heap<T,C>::heap_swap ( size_t first, size_t second, T* array,
				              size_t* ptr_array_aux, size_t* ptr_aux_array ) {

			T swp = array[first];
			array[first] = array[second];
			array[second] = swp;

			size_t aux_first = ptr_array_aux[first];
			size_t aux_second = ptr_array_aux[second];

			ptr_array_aux[first] = aux_second;
			ptr_array_aux[second] = aux_first;

			ptr_aux_array[aux_first] = second;
			ptr_aux_array[aux_second] = first;

			return;
		}

	template <class T, class C>
		void double_heap<T,C>::sift_up ( size_t idx, T* array, size_t size,
			                         size_t* ptr_array_aux, size_t* ptr_aux_array,
			                         bool (double_heap<T,C>::*cmp_function) (const T&, const T&)) {

			while ((idx > 0) && (this->*cmp_function) (array[idx], array[(idx - 1)/2])) {
			
				heap_swap (idx, (idx - 1)/2, array, ptr_array_aux, ptr_aux_array);

				idx = (idx - 1)/2;
			}

			return;
		}

	template <class T, class C>
		void double_heap<T,C>::sift_down ( size_t idx, T* array, size_t size,
				 		   size_t* ptr_array_aux, size_t* ptr_aux_array,
				                   bool (double_heap<T,C>::*cmp_function) (const T&, const T&) ) {

			size_t left = 2*idx + 1;
			size_t right = left + 1;

			while (right < size) {
				
				size_t top_idx = left;

				if ((this->*cmp_function) (array[right], array[top_idx])) {
					top_idx = right;
				}

				if ((this->*cmp_function) (array[idx], array[top_idx])) {
					break;
				}

				heap_swap (idx, top_idx, array, ptr_array_aux, ptr_aux_array);
				idx = top_idx;

				left = 2*idx + 1;
				right = left + 1;		
			}

			if ((left < size) && (this->*cmp_function) (array[left], array[idx])) {
				heap_swap (idx, left, array, ptr_array_aux, ptr_aux_array);
			}

			return;
		}

	template <class T, class C>
		double_heap<T,C>::double_heap (C cmp_function) {
			max_heap = new T [1];
			min_heap = new T [1];

			ptr_max_min = new size_t [1];
			ptr_min_max = new size_t [1];

			heap_size = 0;
			heap_allocated = 1;

			this->cmp_function = cmp_function;
		}

	template <class T, class C>
		double_heap<T,C>::~double_heap (void) {
			
			delete [] max_heap;
			delete [] min_heap;

			delete [] ptr_max_min;
			delete [] ptr_min_max;
		}

	template <class T, class C>
		void double_heap<T,C>::expand (void) {
			if (heap_size == heap_allocated) {

				heap_allocated *= 2;

				T* new_min = new T [heap_allocated];
				T* new_max = new T [heap_allocated];
				
				size_t* new_min_max = new size_t [heap_allocated];
				size_t* new_max_min = new size_t [heap_allocated];
				
				for (size_t i = 0; i < heap_size; i++) {
					new_min[i] = min_heap[i];
					new_max[i] = max_heap[i];

					new_min_max[i] = ptr_min_max[i];
					new_max_min[i] = ptr_max_min[i];
				}

				delete [] min_heap;
				delete [] max_heap;

				min_heap = new_min;
				max_heap = new_max;

				delete [] ptr_min_max;
				delete [] ptr_max_min;

				ptr_min_max = new_min_max;
				ptr_max_min = new_max_min;
			}

			return;
		}

	template <class T, class C>
		void double_heap<T,C>::insert (const T& value) {
			expand();

			min_heap[heap_size] = value;
			max_heap[heap_size] = value;
			
			ptr_min_max[heap_size] = heap_size;
			ptr_max_min[heap_size] = heap_size;

			sift_up (heap_size, min_heap, heap_size, ptr_min_max, ptr_max_min, &double_heap<T,C>::min_cmp);
			sift_up (heap_size, max_heap, heap_size, ptr_max_min, ptr_min_max, &double_heap<T,C>::max_cmp);

			++heap_size;

			return;
		}

	template <class T, class C>
		const T& double_heap<T,C>::get_min (void) {
			return min_heap[0];
		}

	template <class T, class C>
		const T& double_heap<T,C>::get_max (void) {
			return max_heap[0];
		}
	
	template <class T, class C>
		const T double_heap<T,C>::extract_min (void) {

			--heap_size;

			size_t max_heap_idx = ptr_min_max[0];

			T result = min_heap[0];

			heap_swap (0, heap_size, min_heap, ptr_min_max, ptr_max_min);
			heap_swap (max_heap_idx, heap_size, max_heap, ptr_max_min, ptr_min_max);

			sift_down (0, min_heap, heap_size, ptr_min_max, ptr_max_min, &double_heap<T,C>::min_cmp);
			sift_down (max_heap_idx, max_heap, heap_size, ptr_max_min, ptr_min_max, &double_heap<T,C>::max_cmp);
			sift_up (max_heap_idx, max_heap, heap_size, ptr_max_min, ptr_min_max, &double_heap<T,C>::max_cmp);

			return result;
		}

	template <class T, class C>
		const T double_heap<T,C>::extract_max (void) {
			
			--heap_size;

			size_t min_heap_idx = ptr_max_min[0];

			T result = max_heap[0];

			heap_swap (0, heap_size, max_heap, ptr_max_min, ptr_min_max);
			heap_swap (min_heap_idx, heap_size, min_heap, ptr_min_max, ptr_max_min);

			sift_down (0, max_heap, heap_size, ptr_max_min, ptr_min_max, &double_heap<T,C>::max_cmp);
			sift_down (min_heap_idx, min_heap, heap_size, ptr_min_max, ptr_max_min, &double_heap<T,C>::min_cmp);
			sift_up (min_heap_idx, min_heap, heap_size, ptr_min_max, ptr_max_min, &double_heap<T,C>::min_cmp);

			return result;
		}

	template <class T, class C>
		void double_heap<T,C>::clear (void) {
			heap_size = 0;

			return;
		}
}

