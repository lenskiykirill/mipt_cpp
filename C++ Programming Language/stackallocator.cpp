#include <cstdint>
#include <stddef.h>
#include <string>

namespace dst {
      template <size_t N>
            class StackStorage;

      template <size_t N>
            class StackStorage {
                  public:
                        StackStorage ();
                        ~StackStorage () = default;
                        StackStorage (const StackStorage&) = delete;

                        void* allocate (size_t size, size_t align);
                        void mark (void* ptr, size_t size);

                        struct exception;
                        struct exception_BADALLOC; // : public exception;
                  
                  private:

                        uint8_t __array[N];
                        void* __top;
                        void* __cap;

                        size_t __space;

                        void* __align (void* ptr, size_t align, size_t size);
                        void __clean ();
                        
                        // struct __frame;
            };

      /*
      template <size_t N>
            struct StackStorage<N>::__frame {
                  void* start;
                  bool empty;
            };
      */

      template <size_t N>
            StackStorage<N>::StackStorage () : __top (__array) {
                  __cap = __array + N;
                  
                  /*
                  void* void_ptr = __align (__top, alignof (__frame), sizeof (__frame));
                  __frame* frame_ptr = reinterpret_cast<__frame*> (void_ptr);

                  if (!frame_ptr)
                        throw exception_BADALLOC ("Cannot initialize StackStorage");
                  
                  __top = frame_ptr + 1;
                  frame_ptr->empty = false;
                  */
            };

      template <size_t N>
            void* StackStorage<N>::__align (void* ptr, size_t align, size_t size) {

                  uintptr_t int_ptr = reinterpret_cast<uintptr_t> (ptr);
                  uintptr_t res = (int_ptr - 1u + align) & -align;

                  void* res_ptr = reinterpret_cast<void*> (res);
                  uint8_t* bit_ptr = reinterpret_cast<uint8_t*> (res);

                  if ((bit_ptr + size) > __cap)
                        return nullptr;
                  
                  return res_ptr;
            }

      template <size_t N>
            void* StackStorage<N>::allocate (size_t size, size_t align) {
                  void* res = __align (__top, align, size);

                  //void* old_top = __top;
                  
                  if (!res)
                        throw exception_BADALLOC ("Not enough space");
                  
                  uint8_t* bit_ptr = reinterpret_cast<uint8_t*> (res);

                  __top = bit_ptr + size;
                  
                  /*
                  void* void_ptr = __align (__top, alignof (__frame), sizeof (__frame));
                  __frame* frame_ptr = reinterpret_cast<__frame*> (void_ptr);

                  if (!frame_ptr)
                        throw exception_BADALLOC ("Not enough space to fit frame");

                  __top = frame_ptr + 1;

                  frame_ptr->start = old_top;
                  frame_ptr->empty = false;
                  */

                  return res;
            }

      /*
      template <size_t N>
            void StackStorage<N>::mark (void* ptr, size_t size) {
                  
                  uint8_t* bit_ptr = reinterpret_cast<uint8_t*> (ptr);

                  void* void_ptr = __align (bit_ptr + size, alignof (__frame),
                                            sizeof (__frame));

                  __frame* frame_ptr = reinterpret_cast<__frame*> (void_ptr);
                  
                  frame_ptr->empty = true;
                  __clean ();
            }
      */

      template <size_t N>
            void StackStorage<N>::mark (void*, size_t) {
                  
            }

      /*
      template <size_t N>
            void StackStorage<N>::__clean () {
                  while (true) {
                        __frame* frame_ptr = reinterpret_cast<__frame*> (__top);
                        frame_ptr -= 1;

                        if (frame_ptr->empty)
                              __top = frame_ptr->start;
                        else
                              break;
                  }
            }
      */

      template <size_t N>
            void StackStorage<N>::__clean () {}

      template <size_t N>
            struct StackStorage<N>::exception {
                  unsigned int E_CODE;
            };

      template <size_t N>
            struct StackStorage<N>::exception_BADALLOC :
                        public StackStorage<N>::exception {
                  
                  exception_BADALLOC (const std::string& str) : message (str) {};
                  std::string what () { return message; }

                  std::string message;
            };
}

/***********************************************************************************************\
 *                                                                                             *
 *                                                                                             *
 *                                                                                             * 
\***********************************************************************************************/

namespace dst {
      namespace {
            template <size_t N>
                  class __StackAllocator{

                        protected:
                              __StackAllocator () = default;
                              __StackAllocator (const __StackAllocator& other) = default;
                              ~__StackAllocator () = default;


                              StackStorage<N>* S;
                              static StackStorage<N>* __get_storage
                                                      (const __StackAllocator& other);
                              
                              void __swap (__StackAllocator& other);
                  };

            template <size_t N>
                  StackStorage<N>* __StackAllocator<N>::__get_storage
                                                      (const __StackAllocator& other) {
                        return other.S;
                  }

            template <size_t N>
                  void __StackAllocator<N>::__swap (__StackAllocator<N>& other) {
                        StackStorage<N>* tmp = S;

                        S = other.S;
                        other.S = tmp;
                  }
      }

// Wild scheme of diamond inheritence. All for not making S public!

      template <typename T, size_t N>
            class StackAllocator;

      template <typename T, size_t N>
            bool operator== (const StackAllocator<T,N>& left, const StackAllocator<T,N>& right) {
                  return left.S == right.S;
            }

      template <typename T, size_t N>
            bool operator!= (const StackAllocator<T,N>& left, const StackAllocator<T,N>& right) {
                  return !(left == right);
            }

      template <typename T, size_t N>
            class StackAllocator : /*public std::allocator<T>,*/ public __StackAllocator<N> {
                  
                  friend bool operator==<T,N>
                              (const StackAllocator<T,N>& left, const StackAllocator<T,N>& right);

                  public:
                        typedef T value_type;

                        StackAllocator (StackStorage<N>& S);
                        ~StackAllocator () = default;

                        StackAllocator (const StackAllocator& other);

                        template <typename _T>
                              StackAllocator (const StackAllocator<_T,N>& other);

                        StackAllocator& operator= (const StackAllocator& other);

                        T* allocate (size_t n);
                        void deallocate (T* ptr, size_t n);

                        template <typename _T>
                              struct rebind {
                                    typedef StackAllocator<_T,N> other;
                              };

                  private:
//                        StackStorage<N>* S;

                        void swap (StackAllocator& other);
            };

      template <typename T, size_t N>
            StackAllocator<T,N>::StackAllocator (StackStorage<N>& S) {
                  __StackAllocator<N>::S = &S;
            }

      template <typename T, size_t N>
            StackAllocator<T,N>::StackAllocator (const StackAllocator<T,N>& other) : 
                  /*std::allocator<T> (other),*/ __StackAllocator<N> (other) {}

      template <typename T, size_t N>
      template <typename _T>
            StackAllocator<T,N>::StackAllocator (const StackAllocator<_T,N>& other) :
                  __StackAllocator<N> (other) {}

      template <typename T, size_t N>
            void StackAllocator<T,N>::swap (StackAllocator<T,N>& other) {

                  __StackAllocator<N>::__swap (other);
            }

      template <typename T, size_t N>
            T* StackAllocator<T,N>::allocate (size_t n) {
                  T* res;
                  StackStorage<N>* S = __StackAllocator<N>::S;

                  try {
                        res = reinterpret_cast<T*> (S->allocate (n*sizeof(T), alignof (T)));
                  }

                  catch (...) {
                        throw;
                  }

                  return res;
            }

      template <typename T, size_t N>
      StackAllocator<T,N>&
            StackAllocator<T,N>::operator= (const StackAllocator<T,N>& other) {
                  StackAllocator tmp (other);
                  swap (tmp);

                  return *this;
            }

      template <typename T, size_t N>
            void StackAllocator<T,N>::deallocate (T* ptr, size_t n) {

                  StackStorage<N>* S = __StackAllocator<N>::S;

                  S->mark (ptr, n);
            }
}

template <size_t N>
      using StackStorage = dst::StackStorage<N>;

template <typename T, size_t N>
      using StackAllocator = dst::StackAllocator<T,N>;
//--------------------------------------------------------------------------
// List goes here
//--------------------------------------------------------------------------

#include <memory>

namespace dst {
      template <bool expr, typename T1, typename T2>
            struct conditional {
                  typedef T1 type;
            };

      template <typename T1, typename T2>
            struct conditional<false, T1, T2> {
                  typedef T2 type;
            };

      template <bool expr, typename T1, typename T2>
            using conditional_t = typename conditional<expr, T1, T2>::type;
}

namespace dst {
      template <typename T, class A = std::allocator<T>>
            class List {
                  private:
                        template <bool is_const>
                              class base_iterator;
                        struct node;

                        typedef typename std::allocator_traits<A>::template rebind_alloc<node> _A;

                        void __construct (const List& other, _A& allocator);
                        List (const List& other, _A& allocator);

                  public:

                        using iterator = base_iterator<false>;
                        using const_iterator = base_iterator<true>;
                        using reverse_iterator = std::reverse_iterator<iterator>;
                        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

                        List ();
                        List (A allocator);
                        List (size_t size, A allocator = A());
                        List (size_t size, const T& value, A allocator = A());

                        _A get_allocator () const;

                        List (const List& other);
                        ~List ();

                        List& operator= (const List& other);

                        size_t size () const;

                        void push_back (const T& value);
                        void push_front (const T& value);

                        void pop_back ();
                        void pop_front ();

                        
                        iterator begin ();
                        const_iterator begin () const;

                        iterator end ();
                        const_iterator end () const;

                        const_iterator cbegin () const;
                        const_iterator cend () const;

                        reverse_iterator rbegin ();
                        reverse_iterator rend ();

                        const_reverse_iterator rbegin () const;
                        const_reverse_iterator rend () const;

                        const_reverse_iterator crbegin () const;
                        const_reverse_iterator crend () const;

                        void insert (const_iterator it, const T&);
                        void erase (const_iterator it);
                        
                        void swap (List<T,A>& other);
                        void clear ();

                        bool empty ();
                  private:
                        struct base_node {
                              base_node* next;
                              base_node* prev;
                        };

                        struct node : public base_node {
                              T value;
                              
                              node ();
                              node (const T& value);
                              ~node () = default;

                              node& operator= (const node& other) = delete;
                        };

                        _A allocator;

                        mutable base_node void_node; // This is the end of all the things. And the beginning
                        size_t list_size;

            };

      template <typename T, class A>
            List<T,A>::node::node (const T& value) : value (value) {}

      template <typename T, class A>
            List<T,A>::node::node () : value () {}

      template <typename T, class A>
            List<T,A>::List () : allocator () {
                  
                  list_size = 0;

                  void_node.prev = &void_node;
                  void_node.next = &void_node;
            }

      template <typename T, class A>
            List<T,A>::List (A allocator) : allocator (allocator) {

                  list_size = 0;

                  void_node.prev = &void_node;
                  void_node.next = &void_node;
            }

      template <typename T, class A>
            List<T,A>::List (size_t size, A allocator) : allocator (allocator) {

                  list_size = size;

                  base_node* first = &void_node;
                  void_node.prev = &void_node;
                  void_node.next = &void_node;

                  for (size_t i = 0; i < size; ++i) {
                        
                        node* Node;

                        try {
                              Node = std::allocator_traits<_A>::allocate (this->allocator, 1);
                        } catch (...) { clear (); throw; }
                        
                        try {
                              std::allocator_traits<_A>::construct (this->allocator, Node);
                        } catch (...) {
                              std::allocator_traits<_A>::deallocate (this->allocator, Node, 1);
                              clear (); throw; }

                        first->prev = Node;
                        Node->next = first;

                        void_node.next = Node;
                        Node->prev = &void_node;

                        first = Node;
                  }
            }

      template <typename T, class A>
            List<T,A>::List (size_t size, const T& value, A allocator) : allocator (allocator) {
                  
                  list_size = size;

                  base_node* first = &void_node;
                  void_node.prev = &void_node;
                  void_node.next = &void_node;

                  for (size_t i = 0; i < size; ++i) {

                        node* Node;
                        
                        try {
                              Node = std::allocator_traits<_A>::allocate (this->allocator, 1);
                        } catch (...) { clear (); throw; }

                        try {
                              std::allocator_traits<_A>::construct (this->allocator, Node, value);
                        } catch (...) {
                              std::allocator_traits<_A>::deallocate (this->allocator, Node, 1);
                              clear (); throw; }

                        first->prev = Node;
                        Node->next = first;

                        void_node.next = Node;
                        Node->prev = &void_node;

                        first = Node;
                  }
            }

      template <typename T, class A>
            typename List<T,A>::_A List<T,A>::get_allocator () const {
                  return allocator;
            }

      template <typename T, class A>
            void List<T,A>::__construct (const List<T,A>& other, List<T,A>::_A& allocator) {
                  
                  list_size = other.list_size;

                  void_node.prev = &void_node;
                  void_node.next = &void_node;

                  base_node* prev = &void_node;

                  base_node* src = other.void_node.next;

                  while (src != &other.void_node) {
                        node* Node;

                        try {
                              Node = std::allocator_traits<_A>::allocate (allocator, 1);
                        } catch (...) { clear (); throw; }

                        try {
                              std::allocator_traits<_A>::construct
                                      (allocator, Node, static_cast<node*>(src)->value);
                        } catch (...) {
                              std::allocator_traits<_A>::deallocate (allocator, Node, 1);
                              clear (); throw; }

                        Node->prev = prev;
                        Node->next = &void_node;

                        prev->next = Node;
                        void_node.prev = Node;

                        src = src->next;
                        prev = Node;
                  }
            }

      template <typename T, class A>
            List<T,A>::List (const List<T,A>& other) : allocator
                  (std::allocator_traits<_A>::
                        select_on_container_copy_construction (other.allocator))
            {
                  __construct (other, this->allocator);
            }

      template <typename T, class A>
            void List<T,A>::clear () {
                  base_node* PrevNode;
                  node* Node;

                  PrevNode = void_node.next;

                  while (PrevNode != &void_node) {
                        Node = static_cast<node*> (PrevNode);
                        PrevNode = Node->next;

                        std::allocator_traits<_A>::destroy (allocator, Node);
                        std::allocator_traits<_A>::deallocate (allocator, Node, 1);
                  }

                  list_size = 0;
            }

      template <typename T, class A>
            List<T,A>::~List () {
                  clear ();
            }

      template <typename T, class A>
            void List<T,A>::swap (List<T,A>& other) {

                  base_node* Node;
                  base_node* OtherNode;

                  Node = void_node.prev;
                  OtherNode = other.void_node.prev;

                  Node->next = &(other.void_node);
                  OtherNode->next = &void_node;

                  Node = void_node.next;
                  OtherNode = other.void_node.next;

                  Node->prev = &(other.void_node);
                  OtherNode->prev = &void_node;

                  std::swap (void_node.prev, other.void_node.prev);
                  std::swap (void_node.next, other.void_node.next);

                  std::swap (list_size, other.list_size);
                  
//                  std::swap (allocator, other.allocator);
            }

      template <typename T, class A>
            List<T,A>::List (const List<T,A>& other, _A& allocator) : allocator (allocator) {
                  __construct (other, allocator);
            }

      template <typename T, class A>
            List<T,A>& List<T,A>::operator= (const List<T,A>& other) {
                  
                  if (std::allocator_traits<_A>::
                      propagate_on_container_copy_assignment::value &&
                      (other.allocator != allocator)) {

                        allocator = other.allocator;

                        List tmp (other, allocator);
                        swap (tmp);
                  }

                  else {
                        List tmp (other);
                        swap (tmp);
                  }

                  return *this;
            }

      template <typename T, class A>
            size_t List<T,A>::size () const {
                  return list_size;
            }

      template <typename T, class A>
            void List<T,A>::push_back (const T& value) {

                  base_node* last = void_node.prev;
                  node* Node;
                  
                  try {
                        Node = std::allocator_traits<_A>::allocate (allocator, 1);
                  } catch (...) { throw; }

                  try {
                        std::allocator_traits<_A>::construct (allocator, Node, value);
                  } catch (...) {
                        std::allocator_traits<_A>::deallocate (allocator, Node, 1);
                        throw;
                  }

                  Node->next = &void_node;
                  void_node.prev = Node;

                  Node->prev = last;
                  last->next = Node;

                  ++list_size;
            }

      template <typename T, class A>
            void List<T,A>::push_front (const T& value) {
                  
                  base_node* first = void_node.next;
                  node* Node;
                  
                  try {
                        Node = std::allocator_traits<_A>::allocate (allocator, 1);
                  } catch (...) { throw; }

                  try {
                        std::allocator_traits<_A>::construct (allocator, Node, value);
                  } catch (...) {
                        std::allocator_traits<_A>::deallocate (allocator, Node, 1);
                        throw;
                  }

                  Node->prev = &void_node;
                  void_node.next = Node;

                  Node->next = first;
                  first->prev = Node;

                  ++list_size;
            }

      template <typename T, class A>
            void List<T,A>::pop_back () {
                  
                  node* Node = static_cast<node*> (void_node.prev);
                  base_node* last = Node->prev;

                  last->next = &void_node;
                  void_node.prev = last;

                  std::allocator_traits<_A>::destroy (allocator, Node);
                  std::allocator_traits<_A>::deallocate (allocator, Node, 1);

                  --list_size;
            }

      template <typename T, class A>
            void List<T,A>::pop_front () {
                  
                  node* Node = static_cast<node*> (void_node.next);
                  base_node* first = Node->next;

                  first->prev = &void_node;
                  void_node.next = first;
                  
                  std::allocator_traits<_A>::destroy (allocator, Node);
                  std::allocator_traits<_A>::deallocate (allocator, Node, 1);

                  --list_size;
            }

      template <typename T, class A>
            typename List<T,A>::iterator List<T,A>::begin () {
                  iterator ans (void_node.next);

                  return ans;
            }

      template <typename T, class A>
            typename List<T,A>::iterator List<T,A>::end () {
                  iterator ans (&void_node);

                  return ans;
            }
      
      template <typename T, class A>
            typename List<T,A>::const_iterator List<T,A>::begin () const {
                  const_iterator ans (void_node.next);

                  return ans;
            }

      template <typename T, class A>
            typename List<T,A>::const_iterator List<T,A>::end () const {
                  const_iterator ans (&void_node);

                  return ans;
            }

      template <typename T, class A>
            typename List<T,A>::const_iterator List<T,A>::cbegin () const{
                  const_iterator ans (void_node.next);

                  return ans;
            }

      template <typename T, class A>
            typename List<T,A>::const_iterator List<T,A>::cend () const{
                  const_iterator ans (&void_node);

                  return ans;
            }

      template <typename T, class A>
            typename List<T,A>::reverse_iterator List<T,A>::rbegin () {
                  
                  return std::reverse_iterator<iterator> (end());
            }

      template <typename T, class A>
            typename List<T,A>::reverse_iterator List<T,A>::rend () {
                  
                  return std::reverse_iterator<iterator> (begin());
            }

      template <typename T, class A>
            typename List<T,A>::const_reverse_iterator List<T,A>::rbegin () const {
                  
                  return std::reverse_iterator<const_iterator> (cend());
            }

      template <typename T, class A>
            typename List<T,A>::const_reverse_iterator List<T,A>::rend () const {
                  
                  return std::reverse_iterator<const_iterator> (cbegin());
            }
      
      template <typename T, class A>
            typename List<T,A>::const_reverse_iterator List<T,A>::crbegin () const {
                  
                  return std::reverse_iterator<const_iterator> (cend());
            }

      template <typename T, class A>
            typename List<T,A>::const_reverse_iterator List<T,A>::crend () const {
                  
                  return std::reverse_iterator<const_iterator> (cbegin());
            }

      template <typename T, class A>
            void List<T,A>::insert (typename List<T,A>::const_iterator it, const T& value) {
                  
                  base_node* next = it.Node;

                  base_node* prev = next->prev;
                  node* new_node;

                  try {
                        new_node = std::allocator_traits<_A>::allocate (allocator, 1);
                  } catch (...) { throw; }

                  try {
                        std::allocator_traits<_A>::construct (allocator, new_node, value);
                  } catch (...) {
                        std::allocator_traits<_A>::deallocate (allocator, new_node, 1);
                        throw;
                  }

                  new_node->prev = prev;
                  prev->next = new_node;

                  new_node->next = next;
                  next->prev = new_node;

                  ++list_size;
            }

      template <typename T, class A>
            void List<T,A>::erase (typename List<T,A>::const_iterator it) {
                  node* Node = reinterpret_cast<node*> (it.Node);

                  base_node* prev = Node->prev;
                  base_node* next = Node->next;

                  std::allocator_traits<_A>::destroy (allocator, Node);
                  std::allocator_traits<_A>::deallocate (allocator, Node, 1);

                  prev->next = next;
                  next->prev = prev;

                  --list_size;
            }

/*********************************************************************************************\
 *                                                                                           *
 *                                                                                           *
 *                                                                                           *
 *                                                                                           *
\*********************************************************************************************/

      template <typename T, class A>
      template <bool is_const>
            class List<T,A>::base_iterator {
                  friend class List;

                  friend class base_iterator<true>;

                  public:

                        using iterator_category = std::bidirectional_iterator_tag;
                        using difference_type = long long;

                        using value_type = conditional_t<is_const, const T, T>;
                        using pointer = value_type*;
                        using reference = value_type&;

                        base_iterator () = default;
                        base_iterator (base_node*);
                        base_iterator (const base_iterator& other);
                        ~base_iterator () = default;

                        base_iterator& operator= (const base_iterator& other);

                        reference operator* ();
                        pointer operator-> ();

                        base_iterator& operator++ ();
                        base_iterator  operator++ (int);

                        base_iterator& operator-- ();
                        base_iterator  operator-- (int);

                        bool operator== (const base_iterator& other) const;
                        bool operator!= (const base_iterator& other) const;
                        
                        operator base_iterator<true> ();
                  private:

                        base_node* Node;
            };

      template <typename T, class A>
      template <bool is_const>
            List<T,A>::base_iterator<is_const>::base_iterator (List<T,A>::base_node* Node) {

                  this->Node = Node;
            }

      template <typename T, class A>
      template <bool is_const>
            List<T,A>::base_iterator<is_const>::base_iterator
                                          (const List<T,A>::base_iterator<is_const>& other) {
                  Node = other.Node;
            }

      template <typename T, class A>
      template <bool is_const>
            List<T,A>::base_iterator<is_const>& List<T,A>::base_iterator<is_const>::operator=
                                          (const List<T,A>::base_iterator<is_const>& other) {
                  Node = other.Node;
                  return *this;
            }

      template <typename T, class A>
      template <bool is_const>
            typename List<T,A>::template base_iterator<is_const>::reference
                  List<T,A>::base_iterator<is_const>::operator* () {
                  
                  node* res_node = static_cast<node*> (Node);
                  return res_node->value;
            }

      template <typename T, class A>
      template <bool is_const>
            typename List<T,A>::template base_iterator<is_const>::pointer
                  List<T,A>::base_iterator<is_const>::operator-> () {
                  
                  node* res_node = static_cast<node*> (Node);

                  return &(res_node->value);
            }

      template <typename T, class A>
      template <bool is_const>
            List<T,A>::base_iterator<is_const>&
                  List<T,A>::base_iterator<is_const>::operator++ () {
                  
                  Node = Node->next;
                  return *this;
            }

      template <typename T, class A>
      template <bool is_const>
            List<T,A>::base_iterator<is_const>
                  List<T,A>::base_iterator<is_const>::operator++ (int) {
                  
                  base_iterator res (*this);
                  Node = Node->next;

                  return res;
            }

      template <typename T, class A>
      template <bool is_const>
            List<T,A>::base_iterator<is_const>&
                  List<T,A>::base_iterator<is_const>::operator-- () {
                  
                  Node = Node->prev;
                  return *this;
            }

      template <typename T, class A>
      template <bool is_const>
            List<T,A>::base_iterator<is_const>
                  List<T,A>::base_iterator<is_const>::operator-- (int) {
                  
                  base_iterator res (*this);
                  Node = Node->prev;

                  return res;
            }

      template <typename T, class A>
      template <bool is_const>
            bool List<T,A>::base_iterator<is_const>::operator==
                                          (const List<T,A>::base_iterator<is_const>& other) const {
                  
                  return (Node == other.Node);
            }

      template <typename T, class A>
      template <bool is_const>
            bool List<T,A>::base_iterator<is_const>::operator!=
                                          (const List<T,A>::base_iterator<is_const>& other) const {
                  
                  return (Node != other.Node);
            }

      template <typename T, class A>
      template <bool is_const>
            List<T,A>::base_iterator<is_const>::operator List<T,A>::base_iterator<true> () {
                  return base_iterator<true> (Node);
            }
}

template <typename T, class A = std::allocator<T>>
      using List = dst::List<T,A>;
