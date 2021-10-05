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
				~heap ( void ) = default;
			private:
				C cmp_function;
		};

	template<class T, class C>
		heap<T, C>::heap ( C cmp_function) {
			this->cmp_function = cmp_function;

			return;
		}
}

bool is_less(int l, int r) {
	return l <= r;
}

#include <iostream>

int main() {
	dst::heap<int> heap = is_less;
	dst::heap<int> heap_int;

	std::cout << heap->cmp_function(5, 5) << ' ' << heap_int.cmp_function(5, 5) << '\n';	
	return 0;
}
