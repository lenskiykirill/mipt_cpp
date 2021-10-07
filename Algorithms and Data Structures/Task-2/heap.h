namespace dst {
	template<class T>
		void swap (T& a, T&b) {
			T tmp = a;
			a = b;
			b = tmp;

			return;
		}
}

namespace dst {
	template<class T>
		class cmp_default {
			public:
				bool operator()( const T& left, const T& right) { return left < right; };
		};

	template<class T, class C = cmp_default<T>>
		class heap {
			public:
				heap ( C cmp_function = cmp_default<T>() );
				heap ( T *supplied_array, unsigned int size, C cmp_function = cmp_default<T>() );
				~heap ( void );

				void heapify ( void );
				T& extract ( void );
				void insert ( T& element );
				const T& top ( void );

				T* sort( void );

				void clear ( void );

				void display ( void );

				unsigned int size ( void );
			private:
				C is_less;
				T *heap_array;

				unsigned int heap_size;
				unsigned int array_size;

				bool is_external_array = false;
				
				unsigned int parent ( unsigned int idx );

				void sift_up ( unsigned int idx );
				void sift_down ( unsigned int idx );

				void expand ( void );
		};

	template<class T, class C>
		heap<T, C>::heap ( C cmp_function ) {
			this->is_less = cmp_function;
			this->heap_array = new T[2];
			this->heap_size = 0;
			this->array_size = 2;

			return;
		}
	
	template<class T, class C>
		heap<T, C>::heap ( T *supplied_array, unsigned int size, C cmp_function ) {
			this->is_less = cmp_function;
			this->heap_array = supplied_array;
			this->heap_size = size;
			this->array_size = size;

			this->is_external_array = true;

			return;
		}

	template<class T, class C>
		heap<T, C>::~heap ( void ) {
			if (!this->is_external_array) {
				delete this->heap_array;
			}
		}

	template<class T, class C>
		unsigned int heap<T, C>::parent ( unsigned int idx ) {
			return (idx - 1) / 2;
		}

	template<class T, class C>
		void heap<T, C>::sift_up ( unsigned int idx ) {
			while ( ( idx > 0 ) && ( this->is_less( heap_array[idx], heap_array[heap<T, C>::parent(idx)] ) ) ) {
				dst::swap ( heap_array[idx], heap_array[heap<T, C>::parent(idx)] );
				idx = heap<T, C>::parent(idx);
			}

			return;
		}

	template<class T, class C>
		void heap<T, C>::sift_down ( unsigned int idx ) {
			unsigned int first_child;
			unsigned int second_child;
			unsigned int min_child;
			bool found = false;
			
			while ( (second_child = 2*idx + 2) < heap_size ) {
				unsigned int first_child = 2*idx + 1;
				min_child = first_child;
				if ( this->is_less (this->heap_array[second_child], this->heap_array[first_child]) ) {
					min_child = second_child;
				}
				
				if ( this->is_less (this->heap_array[idx], this->heap_array[min_child]) ) {
					found = true;
					break;
				}

				dst::swap( this->heap_array[idx], this->heap_array[min_child] );
				idx = min_child;
			}

			first_child = 2*idx + 1;
			
			if ( (!found) && (first_child < heap_size) && (this->is_less( this->heap_array[first_child], this->heap_array[idx]) ) ) {
				dst::swap(this->heap_array[idx], this->heap_array[first_child]);
			}

			return;
		}

	template<class T, class C>
		void heap<T, C>::expand ( void ) {
			if (this->is_external_array) {
				return;
			}

			if (this->heap_size == this->array_size) {
				
				this->array_size = (this->array_size * 3 + 1) / 2;
				T* new_array = new T[this->array_size];
				
				for (int i=0; i < this->heap_size; i++) {
					new_array[i] = this->heap_array[i];
				}

				delete this->heap_array;
				this->heap_array = new_array;
			}

			return;
		}

	template<class T, class C>
		void heap<T, C>::heapify ( void ) {
			for (int i = this->heap_size; i >= 0; i--) {
				this->sift_down(i);
			}
			return;
		}

	template<class T, class C>
		T& heap<T, C>::extract ( void ) {
			dst::swap(this->heap_array[0], this->heap_array[this->heap_size-1]);

			--(this->heap_size);
			sift_down(0);

			return this->heap_array[this->heap_size];
		}

	template<class T, class C>
		void heap<T, C>::insert ( T& element ) {
			this->expand();

			this->heap_array[heap_size] = element;
			++(this->heap_size);

			this->sift_up(this->heap_size-1);

			return;
		}

	template<class T, class C>
		T* heap<T, C>::sort() {
			while (heap_size > 0) {
				this->extract();
			}

			return this->heap_array;
		}

	template<class T, class C>
		const T& heap<T, C>::top ( void ) {
			return heap_array[0];
		}
	
	template<class T, class C>
		unsigned int heap<T, C>::size ( void ) {
			return this->heap_size;
		}

	template<class T, class C>
		void heap<T, C>::clear ( void ) {
			if (!(this->is_external_array)) {
				delete this->heap_array;
				this->heap_array = new T[3];
				this->array_size = 3;
				this->heap_size = 0;
			}
			return;
		}

/*
	template<class T, class C>
		void heap<T, C>::display ( void ) {
			for (int n = 1; (n) < heap_size; n <<= 1) {
				std::cout << n << ": ";
				for (int i = n-1; (i < (n<<1)-1) && (i < heap_size); i++)
					std::cout << heap_array[i] << ' ';
				std::cout << '\n';
			}
			return;
		}
*/
}
