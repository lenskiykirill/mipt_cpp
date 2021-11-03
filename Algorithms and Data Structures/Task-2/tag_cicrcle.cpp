#include "quick_sort.h"

namespace dst {

	template <class T, class C = __cmp_default<T> >
		class remove_heap {
			public:
				remove_heap (C cmp_function = __cmp_default<T>());
				~remove_heap (void);

				const T& top (void);

				void insert (const T& value);
				void remove (const T& value);
//			private:
				T* heap_array;
				T* remove_array;

				size_t heap_size;
				size_t remove_size;
				
				size_t heap_allocated;
				size_t remove_allocated;

				C cmp_function;

				void sift_up (size_t idx, T* array, size_t size);
				void sift_down (size_t idx, T* array, size_t size);
				
				void insert (const T& value, T* array, size_t &size);
				void extract (T* array, size_t &size);

				void heap_swap (size_t first, size_t second, T* array);

				void expand (void);
		};

	template <class T, class C>
		remove_heap<T,C>::remove_heap (C cmp_function) {
			
			heap_array = new T [1];
			remove_array = new T [1];

			heap_size = 0;
			remove_size = 0;

			heap_allocated = 1;
			remove_allocated = 1;

			this->cmp_function = cmp_function;

			return;
		}

	template <class T, class C>
		remove_heap<T,C>::~remove_heap (void) {
			
			delete [] heap_array;
			delete [] remove_array;
		}

	template <class T, class C>
		void remove_heap<T,C>::heap_swap (size_t first, size_t second, T* array) {
			
			T swp = array[first];
			array[first] = array[second];
			array[second] = swp;

			return;
		}

	template <class T, class C>
		void remove_heap<T,C>::sift_up (size_t idx, T* array, size_t size) {
			
			while ((idx > 0) && cmp_function (array[idx], array[(idx - 1) / 2])) {

				heap_swap (idx, (idx - 1) / 2, array);

				idx = (idx - 1) / 2;
			}

			return;
		}

	template <class T, class C>
		void remove_heap<T,C>::sift_down (size_t idx, T* array, size_t size) {
			
			size_t left = 2*idx + 1;
			size_t right = left + 1;

			while (right < size) {
				
				size_t top_idx = left;

				if ( cmp_function (array[right], array[top_idx]) ) {
					top_idx = right;
				}

				if ( cmp_function (array[idx], array[top_idx]) ) {
					break;
				}

				heap_swap (idx, top_idx, array);
				idx = top_idx;

				left = 2*idx + 1;
				right = left + 1;
			}

			if ( (left < size) && cmp_function (array[left], array[idx]) ) {

				heap_swap (idx, left, array);
			}

			return;
		}

	template <class T, class C>
		void remove_heap<T,C>::expand () {
			
			if (heap_size == heap_allocated) {
				
				heap_allocated *= 2;

				T* new_heap = new T [heap_allocated];

				for (size_t i = 0; i < heap_size; i++) {

					new_heap[i] = heap_array[i];
				}

				delete [] heap_array;

				heap_array = new_heap;
			}

			if (remove_size == remove_allocated) {

				remove_allocated *= 2;

				T* new_remove = new T [remove_allocated];

				for (size_t i = 0; i < remove_size; i++) {

					new_remove[i] = remove_array[i];
				}

				delete [] remove_array;

				remove_array = new_remove;
			}

			return;
		}

	template <class T, class C>
		void remove_heap<T,C>::insert (const T& value, T* array, size_t& size) {
			
			array[size] = value;

			sift_up (size, array, size);

			++size;
		}

	template <class T, class C>
		void remove_heap<T,C>::extract (T* array, size_t& size) {
			
			--size;
			
			heap_swap (0, size, array);

			sift_down (0, array, size);

			return;
		}

	template <class T, class C>
		void remove_heap<T,C>::insert (const T& value) {

			expand();
			
			insert (value, heap_array, heap_size);

			return;
		}

	template <class T, class C>
		void remove_heap<T,C>::remove (const T& value) {

			expand();

			insert (value, remove_array, remove_size);

			return;
		}

	template <class T, class C>
		const T& remove_heap<T,C>::top (void) {
	
			while ((remove_size > 0) && (heap_array[0] == remove_array[0])) {

				extract (heap_array, heap_size);
				extract (remove_array, remove_size);
			}

			return heap_array[0];
		}
}

#include <iostream>

namespace dst {
	template <class T1, class T2>
		class pair {
			public:
				pair (T1 first, T2 second);
				pair () = default;
				
				T1 first;
				T2 second;
		};

	template <class T1, class T2>
		pair<T1,T2>::pair (T1 first, T2 second) {
			this->first = first;
			this->second = second;

			return;
		}
}

namespace crcl {
	class circle {
		public:
			circle (size_t* marked, size_t marked_size, size_t circle_size, size_t part_size);
			~circle () = default;

//		private:
			size_t* marked;
			size_t marked_size;

			size_t circle_size;

			size_t part_size;

			size_t add (size_t left, size_t right, size_t size);
			size_t sub (size_t left, size_t right, size_t size);
			size_t dist (size_t left, size_t right, size_t size);

			size_t* next_part; // points to first element of marked, which belongs to next part
			size_t* prev_part; // -/~/- same for prev part

			void get_next_part ();
			void get_prev_part ();

			dst::pair<size_t, size_t>* circle_split;

			void fill_circle_split ();
	};

	circle::circle (size_t* marked, size_t marked_size, size_t circle_size, size_t part_size) {
		
		this->marked = marked;
		this->marked_size = marked_size;

		dst::quick_sort (this->marked, this->marked_size);

		this->circle_size = circle_size;

		this->part_size = part_size;

		next_part = new size_t [marked_size];
		prev_part = new size_t [marked_size];

		get_next_part ();
		get_prev_part ();

		circle_split = new dst::pair<size_t, size_t> [2*marked_size];

		fill_circle_split();

		return;
	}

	size_t circle::add (size_t left, size_t right, size_t size) {
		
		if ( (size - left) > right ) {
			return left + right;
		}

		return left - (size - right);
	}
	
	size_t circle::sub (size_t left, size_t right, size_t size) {
		
		if (right <= left) {
			return left - right;
		}

		return size - (right - left);
	}

	size_t circle::dist (size_t left, size_t right, size_t size) {
		
		if ( right > left ) {
			return right - left;
		}

		return right + (size - left);
	}

	void circle::get_next_part () {

		for (size_t i = 0; i < marked_size; i++) {
			
			size_t left = i;
			size_t right = i;

			while (dist (left, right, marked_size) > 1) {
				size_t middle = add (left, dist (left, right, marked_size) / 2, marked_size);

				if (dist (marked[i], marked[middle], circle_size) >= part_size) {

					right = middle;

				} else {

					left = middle;
				}
			}

			next_part[i] = left;
		}

		return;
	}

	void circle::get_prev_part () {
		
		for (size_t i = 0; i < marked_size; i++) {
			
			size_t left = i;
			size_t right = i;

			while (dist (left, right, marked_size) > 1) {
				size_t middle = add (left, dist (left, right, marked_size) / 2, marked_size);

				if (dist (marked[middle], marked[i], circle_size) >= part_size) {
					
					left = middle;

				} else {
					
					right = middle;
				}
			}

			prev_part[i] = left;
		}

		return;
	}

	void circle::fill_circle_split () {

		for (size_t i = 0; i < marked_size; i++) {

			circle_split[i].second = add (marked[i], 1, circle_size);

			size_t left_shift = dist (marked[sub (i, 1, marked_size)], marked[i], circle_size);

			size_t right_shift = sub ( marked[next_part[i]],
						   add (marked[i], part_size-1, circle_size),
						   circle_size);

			if (left_shift < right_shift) {
				circle_split[i].first = sub (circle_split[i].second, left_shift, circle_size);
			}

			else {
				circle_split[i].second = sub (circle_split[i].second, right_shift, circle_size);
			}
		}

		size_t k = marked_size;

		for (size_t i = 0; i < marked_size; i++) {
			
			
		}
	}

}

#include <iostream>

class my_cmp {
	public:
		bool operator() (int& left, int& right) { return left < right; }
};

int main() {
	my_cmp cmp;
	dst::remove_heap<int, my_cmp> heap (cmp);
	size_t array[] = {0, 1, 3};

	crcl::circle circle (array, 3, 10, 2);
	
	for (size_t i = 0; i < circle.marked_size; i++) {
		std::cout << circle.circle_split[i].first << " " << circle.circle_split[i].second << "\n";
	}

	std::cout << "\n";

	return 0;
}
