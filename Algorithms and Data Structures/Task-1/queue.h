namespace dst {
	template<class T>
		class queue;

	template<class T>
		class queue_node {
			friend class queue<T>;

			private:
				queue_node ( void ) = default;
				~queue_node ( void ) = default;

				queue_node *next_node;
				T value;
		};

	template<class T>
		class queue {
			public:
				queue ( void );
				~queue ( void );

				void push_back ( T value );
				void push_middle ( T value );

				T pop_front ( void );

				unsigned int size ( void );
			private:
				queue_node<T> *first, *last, *middle; // Middle must always point to the central node or to the next to central if n is even
				unsigned int queue_size;
		};

	template<class T>
		queue<T>::queue ( void ) {
			this->first = nullptr;
			this->middle = nullptr;
			this->last = nullptr;

			this->queue_size = 0;
		}

	template<class T>
		queue<T>::~queue ( void ) {
			queue_node<T> *tmp_node;

			while ( ( this->queue_size > 0 ) && ( this->first != nullptr ) ) {
				
				tmp_node = this->first;

				this->first = tmp_node->next_node;

				delete tmp_node;
				--(this->queue_size);
			}
		}

	template<class T>
		void queue<T>::push_back ( T value ) {
			
			queue_node<T> *new_node = new queue_node<T>;
			new_node->value = value;

			if ( this->queue_size > 0 ) {
				(this->last)->next_node = new_node;
			}

			this->last = new_node;

			if ( this->queue_size == 0 ) {
				this->first = new_node;
				this->middle = new_node;
			}

			else if ( this->queue_size % 2 == 0 ){
				this->middle = (this->middle)->next_node;
			}


			++(this->queue_size);

			return;

		}

	template<class T>
		T queue<T>::pop_front ( void ) {
			
			if ( this->queue_size == 1 ) {
				T value = (this->first)->value;
				
				delete this->first;
				--(this->queue_size);

				return value;
			}
			
			if ( this->queue_size % 2 == 0 ) {
				this->middle = this->middle->next_node;
			}
			
			queue_node<T> *old_node = this->first;
			this->first = old_node->next_node;

			T value = old_node->value;
			delete old_node;

			--(this->queue_size);

			return value;
		}

	template<class T>
		void queue<T>::push_middle ( T value ) {
				
			queue_node<T> *new_node = new queue_node<T>;
			new_node->value = value;

			if ( this->queue_size == 0 ) {
				this->first = new_node;
				this->middle = new_node;
				this->last = new_node;

				++(this->queue_size);

				return;
			}

			if ( this->queue_size == 1 ) {
				this->last = new_node;
			}

			new_node->next_node = (this->middle)->next_node;

			(this->middle)->next_node = new_node;

			if ( this->queue_size % 2 == 0 ) {
				this->middle = (this->middle)->next_node;
			}

			++(this->queue_size);

			return;
		}

	template<class T>
		unsigned int queue<T>::size ( void ) {
			return this->queue_size;
		}
}
