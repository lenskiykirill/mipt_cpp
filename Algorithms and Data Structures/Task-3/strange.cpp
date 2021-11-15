#include <iostream>

namespace dst {
      template <class T>
            class strange_struct {
                  public:
                        strange_struct (const T& left, const T& right);
                        ~strange_struct ();

                        void insert (const T& value);
                        const T& get (const T& value);
                  private:
                        struct node;
                        node* top_node;

                        T left;
                        T right;
            };

      template <class T>
            strange_struct<T>::strange_struct (const T& left, const T& right) {
                  this->left = left;
                  this->right = right;

                  top_node = new node ((left + right) / 2);

                  value = 0;
            }

      template <class T>
            strange_struct<T,C>::~strange_struct () {
                  delete top_node;
            }

      template <class T>
            struct strange_struct<T>::node {
                  node* left;
                  node* right;

                  T value;
                  T left_border;
                  T right_border;

                  node (const T& value);
                  ~node ();
            };

      template <class T>
            strange_struct<T>::node::node (const T& value) : left (nullptr), right (nullptr) {
            
            }

      template <class T>
            strange_struct<T>::node::~node () {
                  if (left != nullptr)
                        delete left;
                  if (right != nullptr)
                        delete right;
            }

      template <class T>
            void strange_struct<T>::insert (const T& value) {
                  node* current_node = top_node;

                  while (true) {
                        if (value < current_node->value) {
                              if (current_node->left != nullptr) {
                                    current_node = current_node->left;
                                    continue;
                              }
                        }
                  }
            }


}

int main() {
      dst::strange_struct<int> S;
      return 0;
}
