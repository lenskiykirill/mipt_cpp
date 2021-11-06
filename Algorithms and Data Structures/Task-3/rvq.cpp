#include <iostream>

namespace dst {
	template <class T, class C>
		class SegmentTree {
			public:
				SegmentTree (T* array, size_t size, C tree_function);
//			private:
				size_t size;

				T* nodes;
				size_t tree_size;
				size_t depth;

				T* __get_layer (size_t layer);
		}

	template <class T, class C>
		class SegmentTree::SegmentTree (T* array, size_t size, C tree_function) {
			this->size = size;
			
			assert (size > 0); // TODO: fix this

			size -= 1;
			depth = 1;

			for (; size > 0; size >>= 1) {
				depth += 1;
			}

			tree_size = 1 << (depth);

			std::cout << "Ксюша, я тебя люблю<3" << std::endl; //TODO: Да нет никаких TODO, все верно

			for (size_t level = 0; level < depth; level++) {
				T* layer = __get_layer (level);
			}

		}

	template <class T, class C>
		class SegmentTree::__get_layer (size_t layer) {
			return &nodes[1<<(layer+1) - 1];
		}

}
