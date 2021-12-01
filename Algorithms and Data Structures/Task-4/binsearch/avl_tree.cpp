#include <cassert>
#include <string>
#include <iostream>

namespace dst {
      template <class T>
            T& max (T& l, T& r) {
                  return (l > r) ? l : r;
            }
}

namespace dst {
      template <class T>
            struct cmp_default  {
                  bool operator() (const T& l, const T& r) { return l < r; }
            };
}

namespace dst {
      template <class K, class V, class C = cmp_default<K>>
            class avl_tree {
                  
                  friend std::ostream& operator<< (std::ostream& out, avl_tree& tree) {
                        tree.display (out, "", tree.root, false);
                        return out;
                  }
                  
                  public:
                        avl_tree (C cmp_function = cmp_default<K>());
                        ~avl_tree () = default;

                        void insert (const K& key, const V& value);
                        V& get (const K& key);
                        void extract (const K& key);

                  private:
                        struct Node;
                        C cmp_function;

                        Node* root;

                        void fix_broken (Node* node);

                        void display (std::ostream&, const std::string, Node*, bool);

                        Node* get_min (Node* node);
                        Node* get_max (Node* node);
            };

      template <class K, class V, class C>
            avl_tree<K,V,C>::avl_tree (C cmp_function) :
                  root (nullptr)
            {
                  this->cmp_function = cmp_function;
            }
            
      template <class K, class V, class C>
            void avl_tree<K,V,C>::insert (const K& key, const V& value) {
                  Node* node = root;

                  while (root) {
                        if (cmp_function (key, node->key)) {
                              if (!node->left) {
                                    node->left = new Node (node, key, value);
                                    
                                    node->set_depth();

                                    node = node->left;
                                    break;
                              }
                              node = node->left;
                              continue;
                        }

                        if (cmp_function (node->key, key)) {
                              if (!node->right) {
                                    node->right = new Node (node, key, value);
                                    node->set_depth();
                                    
                                    node = node->right;
                                    break;
                              }

                              node = node->right;
                              continue;
                        }

                        node->value = value;
                        break;
                  }

                  if (!root) {
                        root = new Node (nullptr, key, value);
                        node = root;
                  }

                  if (node != root)
                        fix_broken (node->parent);
            }

      template <class K, class V, class C>
            void avl_tree<K,V,C>::extract (const K& key) {
                  Node* node = root;

                  while (true) {
                        if (cmp_function (key, node->key)) {
                              node = node->left;
                              continue;
                        }

                        if (cmp_function (node->key, key)) {
                              node = node->right;
                              continue;
                        }

                        break;
                  }
            }

      template <class K, class V, class C>
            typename avl_tree<K,V,C>::Node* avl_tree<K,V,C>::get_min (Node* node) {
                  Node* next = node->left;

                  while (next) {
                        node = next;
                        next = next->left;
                  }

                  return node;
            }

      template <class K, class V, class C>
            typename avl_tree<K,V,C>::Node* avl_tree<K,V,C>::get_max (Node* node) {
                  Node* next = node->right;

                  while (next) {
                        node = next;
                        next = next->right;
                  }

                  return node;
            }

      template <class K, class V, class C>
            struct avl_tree<K,V,C>::Node {
                  Node* parent;

                  Node* left;
                  Node* right;

                  K key;
                  V value;

                  unsigned long long depth;

                  Node (Node* parent, K key, V value);
                  ~Node ();

                  int diff ();
                  bool is_left ();
                  void set_depth ();

                  Node* rotate_left  ();
                  Node* rotate_right ();
            };

      template <class K, class V, class C>
            avl_tree<K,V,C>::Node::Node (Node* parent, K key, V value) :
                  parent (parent), left (nullptr), right (nullptr),
                  key (key), value (value), depth (1) {}

      template <class K, class V, class C>
            int avl_tree<K,V,C>::Node::diff () {

                  unsigned long long left_depth = (left ? left->depth : 0);
                  unsigned long long right_depth = (right ? right->depth : 0);

                  if (left_depth >= right_depth) {
                        assert (left_depth - right_depth <= 2);
                        return static_cast<int> (left_depth - right_depth);
                  }

                  assert (right_depth - left_depth <= 2);
                  return -static_cast<int> (right_depth - left_depth);
            }

      template <class K, class V, class C>
            void avl_tree<K,V,C>::Node::set_depth () {
                  unsigned long long l_depth = left ? left->depth : 0;
                  unsigned long long r_depth = right ? right->depth : 0;

                  depth = max (l_depth, r_depth) + 1;
            }

      template <class K, class V, class C>
            bool avl_tree<K,V,C>::Node::is_left () {
                  return parent->left == this;
            }

      template <class K, class V, class C>
            typename avl_tree<K,V,C>::Node* avl_tree<K,V,C>::Node::rotate_left () {
                  
                  Node* l_node = left;
                  Node* lr_node = l_node->right;
                  
                  if (lr_node)
                        lr_node->parent = this;
                  
                  left = lr_node;

                  if (parent) // It may not be needed...
                        (is_left() ? parent->left : parent->right) = l_node;

                  l_node->parent = parent;
                  parent = l_node;
                  l_node->right = this;

                  set_depth ();
                  l_node->set_depth ();

                  return l_node->parent;
            }

      template <class K, class V, class C>
            typename avl_tree<K,V,C>::Node* avl_tree<K,V,C>::Node::rotate_right () {
                  
                  Node* r_node = right;
                  Node* rl_node = r_node->left;
                  
                  if (rl_node)
                        rl_node->parent = this;

                  right = rl_node;

                  if (parent)
                        (is_left() ? parent->left : parent->right) = r_node;

                  r_node->parent = parent;
                  parent = r_node;
                  r_node->left = this;

                  set_depth ();
                  r_node->set_depth ();

                  return r_node->parent;
            }

      template <class K, class V, class C>
            void avl_tree<K,V,C>::fix_broken (Node* broken) {

                  while (broken != root) {
                        
                        broken->set_depth ();

                        if (broken->diff() >= 2) {
                              broken = broken->rotate_left ();
                              continue;
                        }

                        if (broken->diff() <= -2) {
                              broken = broken->rotate_right ();
                              continue;
                        }

                        broken = broken->parent;
                  }

                  root->set_depth ();

                  if (broken->diff() >= 2) {
                        root = broken->left;
                        broken->rotate_left ();
                        return;
                  }

                  if (broken->diff() <= -2) {
                        root = broken->right;
                        broken->rotate_right ();
                  }
            }

      template <class K, class V, class C>
            void avl_tree<K,V,C>::display (std::ostream& out,
                                           const std::string prefix,
                                           Node* node, bool isLeft)
            {
                  if (!node) {
//                        out << prefix;
//                        out << (isLeft ? "├" : "");
                        return;
                  }

                  out << prefix;
                  out << (isLeft? "├──" : "└──");

                  out << node->key << std::endl;

                  display (out, prefix + (isLeft ? "│   " : "    "), node->left, true); 
                  display (out, prefix + (isLeft ? "│   " : "    "), node->right, false);
            }
};

int main () {
      dst::avl_tree<unsigned long long, long long> T;

      std::string request;
      long long key;

      while (request != "exit") {
            std::cin >> request;

            if (request == "insert") {
                  std::cin >> key;
                  T.insert (key, 0);
                  continue;
            }

            if (request == "show") {
                  std::cout << std::endl;
                  std::cout << T << std::endl;
                  continue;
            }
      }
} 
