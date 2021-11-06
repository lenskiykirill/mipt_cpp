#include <iostream>

namespace dst {
	template <class __node, class C>
		class segment_tree {
			public:
				segment_tree (__node* array, size_t size, C tree_function);
				segment_tree (segment_tree& other);
				~segment_tree () = default;

				const __node& operator[] (size_t idx);

				void update_value (size_t idx, __node& value);
				__node get_value (size_t left, size_t right);

//			private:
				C tree_function;

				size_t size;
				size_t depth;

				__node* nodes;
				size_t nodes_size;

				__node* __get_layer (size_t layer);
		};

	template <class __node, class C>
		segment_tree<__node,C>::segment_tree (__node* array, size_t size, C tree_function) {
			
			this->tree_function = tree_function;

			// assert (size > 0); // Size must be > 0
			
			depth = 1;

			for (this->size = size-1; this->size > 0; this->size >>= 1) {
				++depth;
			}

			this->size = 1 << (depth - 1);
			nodes_size = (1 << depth) - 1;

			nodes = new __node[nodes_size];
			
			__node* layer = __get_layer (depth-1);
			
			for (size_t i = 0; i < size; i++) { // Fill last layer
				layer[i] = array[i];
			}

			for (size_t i = size; i < this->size; ++i) {
				layer[i] = array[0];
			}

			for (size_t node_idx = this->size-1; node_idx > 0; --node_idx) {
				
				size_t left_child = node_idx*2 - 1;
				size_t right_child = node_idx*2;

				nodes[node_idx-1] = this->tree_function (nodes[left_child], nodes[right_child]);
			}
		}

	template <class __node, class C>
		__node* segment_tree<__node,C>::__get_layer (size_t layer) {
			
			return &(nodes[(1 << layer) - 1]);
		}

	template <class __node, class C>
		void segment_tree<__node,C>::update_value (size_t idx, __node& value) {
			
			size_t node_idx = idx + size-1;

			nodes[node_idx] = value;

			while (node_idx > 0) {
				size_t parent = (node_idx - 1) / 2;

				size_t left_child = parent*2 + 1;
				size_t right_child = parent*2 + 2;

				nodes[parent] = tree_function (nodes[left_child], nodes[right_child]);

				node_idx = parent;
			}
		}

	template <class __node, class C>
		__node segment_tree<__node,C>::get_value (size_t left, size_t right) {
			
			return nodes[0];
		}
}

class cmp {
	public:
		int operator() (int& a, int& b) {
			if (a < b) {
				return a;
			}
			return b;
		}
};

int main () {
	int a[] = {3, 4, 1, 2, 5, 2};

	dst::segment_tree<int, cmp> T (a, 6, cmp());
	int new_value = 0;
	T.update_value(1, new_value);

	for (int i=0; i < T.depth; i++) {
		int* layer = T.__get_layer(i);
		for (int j=0; j < (1 << i); j++) {
			std::cout << layer[j] << ' ';
		}

		std::cout << std::endl;
	}

	return 0;
}

