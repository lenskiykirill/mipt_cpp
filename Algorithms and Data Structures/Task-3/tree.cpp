namespace dst {
	
	template <class T, class C>
		class persistent_tree {
			public:

			private:
				struct __node {
					T value;

					struct __node* parent;
					struct __node* left_child;
					struct __node* right_child;
				};

				struct __node* top_nodes;
				size_t top_nodes_size;


		};
}
