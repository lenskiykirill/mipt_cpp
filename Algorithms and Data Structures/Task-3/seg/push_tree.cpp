#include <iostream>

class Tree {
      public:
            Tree (int* array, size_t size);
            ~Tree ();

            void update (size_t left, size_t right, int value);
            int get_min (size_t left, size_t right);
      private:
            int* nodes;
            int* pushed;
            bool* is_pushed;

            size_t size;
            size_t nodes_size;
            size_t nodes_depth;

            int __get_min (size_t idx);
            void __update (size_t idx, int value);

            size_t left_border; // changes during update and get_min
            size_t right_border;

            size_t left;
            size_t right;

            void push (size_t idx, int value);
            void pop (size_t idx);

            size_t layer_offset (size_t layer);
            void get_size (size_t size);
};

void Tree::get_size (size_t size) {
      nodes_depth = 1;

      for (--size; size > 0; size >>= 1) {
            ++nodes_depth;
      }

      this->size = 1 << (nodes_depth - 1);
      nodes_size = (1 << nodes_depth) - 1;
}

size_t Tree::layer_offset (size_t layer) {
      return (1 << layer) - 1;
}

Tree::Tree (int* array, size_t size) {
      get_size (size);
      nodes = new int[nodes_size];
      pushed = new int[nodes_size];
      is_pushed = new bool[nodes_size];

      for (size_t i = 0; i < size; ++i) {
            nodes[layer_offset (nodes_depth-1)+i] = array[i];
            is_pushed[layer_offset (nodes_depth-1)+i] = false;
      }

      for (size_t i = layer_offset (nodes_depth-1)+size; i < nodes_size; ++i) {
            nodes[i] = 0;//array[0];
            is_pushed[i] = false;
      }

      for (size_t i = layer_offset (nodes_depth-1); i > 0; --i) {
            size_t left_child = 2*i - 1;
            size_t right_child = 2*i;

            nodes[i-1] = std::min (nodes[left_child], nodes[right_child]);
            is_pushed[i-1] = false;
      }
}

Tree::~Tree () {
      delete [] nodes;
      delete [] pushed;
      delete [] is_pushed;
}

void Tree::pop (size_t idx) {
      if (is_pushed[idx]) {
            nodes[idx] = pushed[idx];
            is_pushed[idx] = false;

            if (2*idx + 1 < nodes_size) {
                  pushed[2*idx + 1] = pushed[idx];
                  pushed[2*idx + 2] = pushed[idx];

                  is_pushed[2*idx+1] = is_pushed[2*idx+2] = true;
            }
      }
}

void Tree::push (size_t idx, int value) {

      pushed[idx] = value;
      is_pushed[idx] = true;
}

void Tree::__update (size_t idx, int value) {

      pop (idx);

      if ((left == left_border) && (right == right_border)) {
            push (idx, value);
            pop (idx);
            return;
      }

      pop (idx);
      
      size_t old_left = left;
      size_t old_right = right;
      
      size_t old_left_border = left_border;
      size_t old_right_border = right_border;
      
      size_t middle = (left_border + right_border) / 2;

      if ((left >= left_border) && (right <= middle)) {
            left_border = left_border;
            right_border = middle;

            __update (2*idx+1, value);
            pop (2*idx+2);

            nodes[idx] = std::min(nodes[2*idx+1], nodes[2*idx+2]);

            return;
      }

      if ((left >= middle) && (right <= right_border)) {
            right_border = right_border;
            left_border = middle;

            __update (2*idx+2, value);

            pop (2*idx+1);
            nodes[idx] = std::min(nodes[2*idx+1], nodes[2*idx+2]);

            return;
      }

      left = left;
      right = middle;

      left_border = left_border;
      right_border = middle;

      __update (2*idx+1, value);

      left = middle;
      right = old_right;
      
      left_border = middle;
      right_border = old_right_border;

      __update (2*idx+2, value);
      
      nodes[idx] = std::min(nodes[2*idx+1], nodes[2*idx+2]);
}

int Tree::__get_min (size_t idx) {

      pop (idx);
      
      if ((left == left_border) && (right == right_border)) {
            return nodes[idx];
      }

      size_t old_left = left;
      size_t old_right = right;
      
      size_t old_left_border = left_border;
      size_t old_right_border = right_border;
      
      size_t middle = (left_border + right_border) / 2;

      if ((left >= left_border) && (right <= middle)) {
            left_border = left_border;
            right_border = middle;

            return __get_min (2*idx+1);
      }

      if ((left >= middle) && (right <= right_border)) {
            left_border = middle;
            right_border = right_border;

            return __get_min (2*idx+2);
      }

      int left_min;
      int right_min;

      left = left;
      right = middle;

      left_border = left_border;
      right_border = middle;

      left_min = __get_min (2*idx + 1);

      left = middle;
      right = old_right;

      left_border = middle;
      right_border = old_right_border;

      right_min = __get_min (idx*2 + 2);

      return std::min (left_min, right_min);
}

void Tree::update (size_t left, size_t right, int value) {
      this->left = left;
      this->right = right;

      left_border = 0;
      right_border = size;

      __update (0, value);
}

int Tree::get_min (size_t left, size_t right) {
      this->left = left;
      this->right = right;

      left_border = 0;
      right_border = size;

      return __get_min (0);
}

struct Dumb_Tree {
      Dumb_Tree (int* array, size_t size) {
            this->array = new int[size];
            for (size_t i=0; i < size; i++) {
                  this->array[i] = array[i];
            }
      }
      int get_min (size_t left, size_t right) {
            int min = array[left];
            for (size_t i=left; i < right; ++i) {
                  min = std::min (array[i], min);
            }

            return min;
      }
      
      void update (size_t left, size_t right, int value) {
            for (size_t i=left; i < right; ++i)
                  array[i] = value;
      }

      ~Dumb_Tree () {delete [] array;}

      int* array;
};

#include <random>

void stress_test (size_t N, size_t size=5) {
      int* array = new int[size];

      std::random_device rd;
      std::mt19937 rng(rd());
      std::uniform_int_distribution<int> uni (0, size-1);
      std::uniform_int_distribution<int> color (0, 255);
      std::cout << "array: ";
      for (size_t i=0; i < size; ++i) {
            array[i] = color(rng);
            std::cout << array[i] << ' ';
      }
      std::cout << "\n\n";

      Tree tr(array, size);
      Dumb_Tree dtr(array, size);

      delete [] array;

      for (int i=0; i < N; i++) {
            int left = uni(rng);
            int right = uni(rng);
            int c = color(rng);

            if (left > right) {
                  std::swap (left, right);
            }

            std::cout << "set ("<<left<<", "<<right<<") to "<<c<<"\n";

            ++right;
            tr.update (left, right, c);
            dtr.update (left, right, c);

            left = uni (rng);
            right = uni (rng);

            if (left > right)
                  std::swap (left, right);

            std::cout << "get ("<<left<<", "<<right<<")\n";

            ++right;

            int tr_result = tr.get_min (left, right);
            int dtr_result = dtr.get_min (left, right);

            if (tr_result != dtr_result) {
                  std::cerr << "ERROR!!!\n";
                  exit(0);
            }
      }
      std::cout << "OK!\n";
}

void stress_test_2 () {
      int array[] = {40, 144, 70, 123, 84};
      Tree t (array, 5);
      t.update(0, 5, 18);
      int res = t.get_min(1, 3);
      
      t.update (0, 3, 19);
      res = t.get_min(0, 4);

      std::cout << res << '\n';

      return;
}

int main () {
     int r, g, b;
     size_t N = 5;
     std::cin >> N;

     int* array = new int[N];

      for (size_t i = 0; i < N; ++i) {
            std::cin >> r >> g >> b;
            array[i] = r+g+b;
      }

      Tree tr (array, N);

      delete [] array;

      std::cin >> N;
      size_t left, right;
      
      for (size_t i=0; i < N; i++) {
            std::cin >> left >> right;
            std::cin >> r >> g >> b;

            tr.update (left, right+1, r+g+b);

            std::cin >> left >> right;
            r = tr.get_min (left, right+1);
            
            std::cout << r << ' ';
      }
      std::cout << '\n';
}

