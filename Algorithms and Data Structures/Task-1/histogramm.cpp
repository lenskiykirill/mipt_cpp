#include <iostream>
#include <stdio.h>

namespace dst {
	template<class T>
		class stack;

	template<class T>
		class stack_node {
			friend class stack<T>;

			public:
				stack_node ( void ) = default;
				~stack_node ( void ) = default;

				stack_node *next_node;
				T value;
		};

	template<class T>
		class stack {
			public:
				stack ( void );
				~stack ( void );

				void push ( T value );
				T pop (void);
				T back ( void );

				unsigned int size ( void );
				bool empty ( void );

				void clear ( void );

			private:
				stack_node<T> *top_node;
				unsigned int stack_size;

		};

	template<class T>
		stack<T>::stack ( void ) {
			this->top_node = nullptr;
			this->stack_size = 0;
		}

	template<class T>
		stack<T>::~stack ( void ) {
			this->clear ();
		}

	template<class T>
		void stack<T>::push ( T value ) {
			stack_node<T> *new_node = new stack_node<T>;

			new_node->next_node = this->top_node;
			new_node->value = value;

			this->top_node = new_node;
			++(this->stack_size);

			return;
		}

	template<class T>
		T stack<T>::pop ( void ) {
			stack_node<T> *old_node = this->top_node;
			T value;

			this->top_node = old_node->next_node;
			value = old_node->value;

			delete old_node;
			--(this->stack_size);

			return value;
		}

	template<class T>
		T stack<T>::back ( void ) {
			return (this->top_node)->value;
		}

	template<class T>
		unsigned int stack<T>::size ( void ) {
			return this->stack_size;
		}

	template<class T>
		bool stack<T>::empty ( void ) {
			return this->stack_size == 0;
		}

	template<class T>
		void stack<T>::clear ( void ) {
			stack_node<T> *tmp_node;
			while ( this->stack_size > 0 ) {
				tmp_node = this->top_node;
				this->top_node = tmp_node->next_node;
				
				delete tmp_node;
				--(this->stack_size);
			}
			return;
		}
}

long long get_max_rect ( long long *hist, unsigned int size) {

	dst::stack<long long> stk; // contains decreasing sequence
	unsigned int left, middle, right;
	long long tmp_max, all_max = 0;

	for (unsigned int i=0; i <= size; ++i) {
		right = i;

		while ( ( stk.size() > 0 ) && ( hist[stk.back()] > hist[i] ) ) {
		
			middle = stk.pop();

			left = -1;
			if ( stk.size() > 0 )
				left = stk.back();

			tmp_max = (right - left - 1)*hist[middle];

			if (tmp_max > all_max)
				all_max = tmp_max;

//			std::cout << "hist[" << i << "]: left: " << (int) left << " middle: "<< middle << " right: " << right << " tmp_max: " << tmp_max << std::endl << std::endl;

		}

		stk.push(i);
	}
	
	return all_max;
}

int main () {
	unsigned int N;
	
	std::cin >> N;

	long long *hist = new long long[N+1];

	for (int i=0; i < N; i++)
		scanf("%lld", hist+i);

	hist[N] = 0;

	printf("%lld\n", get_max_rect (hist, N));

	delete hist;

	return 0;
}
