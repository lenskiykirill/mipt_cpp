#include <csstddef>
#include <cassert>
#include <string>
#include <iostream>

namespace dst {
      template <class K, class V, class C = avl_tree::__cmp_default>
            class avl_tree {

                  friend std::ostream& operator<< (std::ostream&, avl_tree&);
                  
                  public:
                        avl_tree (C cmp_function = __cmp_default ());

                        void insert (const K& key, const V& value);
                        V& get (const K& key);

                  private:
                        struct Node;

                        Node* root;

                        static void __show_tree (const std::string, Node* node, bool isLeft);
                        static struct __cmp_default { bool operator() (K l, K r) { return l < r; } };

                        C cmp_function;

                        void fix_broken (Node* broken);
            };

      template <class K, class V, class C>
            void avl_tree<K,V,C>::insert (const K& key, const V& value) {
                  Node* ins_node = root;

                  while (true) {
                        if (cmp_function (ins_node->key, key)) { // Go to left child
                              if (!ins_node->left) {
                                    ins_node->left = new Node (key, value)
                              }
                        }
                  }
            }

      template <class K, class V, class C>
            void avl_tree<K,V,C>::fix_broken (Node* broken) {
                  while (broken != root) {

                        if (broken->diff() >= 2) {
                              assert (broken == 2);
                              broken = broken->rotate_left ();
                              continue;
                        }
                        
                        if (broken->diff() <= -2) {
                              assert (broken == -2);
                              broken = broken->rotate_right ();
                              continue;
                        }
                        
                        return;
                  }

                  if (broken->diff() >= 2) {
                        assert (broken == 2);
                        
                        root = broken->left;
                        broken->rotate_left ();
                        
                        return;
                  }

                  if (broken->diff() <= -2) {
                        assert (broken == -2);

                        root = broken->right;
                        broken->rotate_right ();
                  }
            }

      template <class K, class V, class C>
            struct avl_tree<K,V,C>::Node {
                  Node* parent;

                  Node* left;
                  Node* right;

                  K key;
                  V value;

                  long long depth;

                  Node (const K& key, const V& value) : 
                        parent (nullptr), left (nullptr), right (nullptr),
                        key (key), value (value) {};

                  ~Node () = default;

                  bool is_left () { return parent->left == this; }

                  int diff (); // left_depth - right_depth

                  Node* rotate_left ();
                  Node* rotate_right ();
            };

      template <class K, class V, class C>
            int avl_tree<V,K,C>::Node::diff () {
                  long long d = 0;

                  if (left)
                        d += left->depth;
                  if (right)
                        d -= right->depth;

                  return static_cast<int> (d);
            }
// TODO: chang depths in this process
      template <class K, class V, class C>
            avl_tree::Node* avl_tree<K,V,C>::Node::rotate_left () {
                  Node* l_node = left;
                  Node* lr_node = left->right;

                  lr_node->parent = this;
                  left = lr_node;

                  if (parent) {
                        (is_left () ? parent->left : parent->right) = l_node;
                        ++parent->depth;
                  }

                  l_node->parent = parent;
                  parent = l_node;
                  l_node->right = this;

                  return l_node->parent;
            }

      template <class K, class V, class C>
            avl_tree::Node* avl_tree<K,V,C>::Node::rotate_right () {
                  Node* r_node = right;
                  Node* rl_node = right->left;

                  rl_node->parent = this;
                  right = rl_node;

                  if (parent)
                        (is_left () ? parent->left : parent->right) = r_node;

                  r_node->parent = parent;
                  parent = r_node;
                  r_node->right = this;

                  return r_node->parent;
            }

      template <class K, class V, class C>
            static void avl_tree<K,V,C>::__show_tree (
                                                      std::ostream out,
                                                      const std::string,
                                                      Node* node, bool isLeft
                                                     ) {
                  if (node != nullptr) {
                        out << prefix;

                        out << (isLeft ? "├──" : "└──");

                        out << node->key << std::endl;

                        __show_tree (prefix + (isLeft ? "│   " : "    "), node->left, true);
                        __show_tree (prefix + (isLeft ? "│   " : "    "), node->right, false);
                  }
            }
}


template <class K, class V, class C>
      std::ostream& operator<< (std::ostream& out, dst::avl_tree<K,V,C>& tree) {
            tree.__show_tree (out, "", root, false);
            return out;
}
