#include <iostream>
#include <string.h>

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

int main () {
	dst::stack<unsigned int> stk;

	char *s = (char *) malloc(30);
        int d;

        scanf("%29s", s);

        while ( strncmp(s, "exit", 30) ) {

                if ( !strncmp(s, "push", 4) ) {
                        scanf("%d", &d);
                        stk.push(d);
                        printf("ok\n");
                }

                else if ( !strncmp(s, "pop", 30) )
			if ( stk.size() > 0 ) {
                        	printf("%d\n", stk.pop());
			}
			else {
				printf("error\n");
			}

                else if ( !strncmp(s, "back", 30) )
			if ( stk.size() > 0 ) {
                        	printf("%d\n", stk.back());
			}
			else {
				printf("error\n");
			}

                else if ( !strncmp(s, "clear", 30) ) {
                        stk.clear();
                        printf("ok\n");
                }

                else if ( !strncmp(s, "size", 30) ) {
                        printf("%d\n", stk.size());
                }

                else
                        printf("Uncnown command\n");

                scanf("%29s", s);
        }

        printf("bye\n");
}

unsigned int max_rect (unsigned int *a) {

