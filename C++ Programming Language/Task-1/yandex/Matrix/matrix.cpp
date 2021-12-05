#include <cstddef>

namespace dst {
      template <class T>
            class vector {
                  public:
                        vector ();
                        vector (size_t vector_size);
                        vector (const T& value, size_t vector_size);
                        vector (const vector& other);

                        ~vector();

                        T& operator[] (size_t index);
                        const T& operator[] (size_t index) const;

                        vector& operator= (const vector other);

                        void push_back (const T& value);
                        void pop_back ();
                        T& back();

                        void resize (size_t new_size);

                        size_t size () const;

                  private:
                        void expand ();
                        void shrink ();

                        size_t vector_size;
                        size_t allocated_size;

                        T* array;

            };

      template <class T>
            vector<T>::vector () :
                  vector_size (0), allocated_size (4), array (new T [4]) {}

      template <class T>
            vector<T>::vector (size_t vector_size) :
                  vector_size (vector_size),
                  allocated_size ((vector_size*3+4) / 2),
                  array (new T[(vector_size*3+4) / 2]) {}

      template <class T>
            vector<T>::vector (const T& value, size_t vector_size) :
                  vector_size (vector_size),
                  allocated_size ((vector_size*3+4) / 2),
                  array (new T[(vector_size*3+4) / 2]) {

                        for (size_t i = 0; i < vector_size; ++i)
                              array[i] = value;
                  }

      template <class T>
            vector<T>::vector (const vector& other) :
                  vector_size (other.vector_size),
                  allocated_size ((other.vector_size*3+4) / 2),
                  array (new T[(other.vector_size*3+4) / 2]) {

                        for (size_t i = 0; i < other.vector_size; ++i)
                              array[i] = other.array[i];
                  }

      template <class T>
            vector<T>::~vector () {
                  delete [] array;
            }

      template <class T>
            T& vector<T>::operator[] (size_t index) {
                  return array[index];
            }

      template <class T>
            const T& vector<T>::operator[] (size_t index) const {
                  return array[index];
            }

      template <class T>
            vector<T>& vector<T>::operator= (const vector other) {
                  delete [] array;

                  allocated_size = (other.vector_size*3+4) / 2;
                  vector_size = other.vector_size;

                  array = new T [allocated_size];

                  for (size_t i = 0; i < other.vector_size; ++i)
                        array[i] = other.array[i];

                  return *this;
            }

      template <class T>
            size_t vector<T>::size () const {
                  return vector_size;
            }

      template <class T>
            void vector<T>::push_back (const T& value) {
                  expand ();
                  array[vector_size++] = value;
            }

      template <class T>
            void vector<T>::pop_back () {
                  --vector_size;
            }

      template <class T>
            T& vector<T>::back () {
                  return array[vector_size-1];
            }

      template <class T>
            void vector<T>::resize (size_t vector_size) {
                  if (vector_size > allocated_size) {
                        allocated_size = (vector_size*3+4) / 2;

                        T* new_array = new T [allocated_size];

                        for (size_t i = 0; i < this->vector_size; ++i)
                              new_array[i] = array[i];

                        delete [] array;
                        array = new_array;
                  }

                  this->vector_size = vector_size;
            }

      template <class T>
            void vector<T>::expand () {
                  if (vector_size == allocated_size) {
                        allocated_size = (allocated_size*3) / 2;

                        T* new_array = new T [allocated_size];

                        for (size_t i = 0; i < vector_size; ++i)
                              new_array[i] = array[i];

                        delete [] array;
                        array = new_array;
                  }
            }

      template <class T>
            void vector<T>::shrink () {}
}

namespace dst {
      template <class T>
            void swap (T& left, T& right) {
                  T swp = left;
                  left = right;
                  right = swp;
            }

      unsigned long long revert (unsigned long long a, unsigned long long p) {
            unsigned long long r, d, P = p;
            long long M[4] = {1, 0, 0, 1}, G[4];

            while (p > 0) {
                  r = a % p;
                  d = a / p;
                  a = p;
                  p = r;

                  G[0] = M[2];
                  G[1] = M[3];
                  G[2] = M[0] - d*M[2];
                  G[3] = M[1] - d*M[3];

                  M[0] = G[0];
                  M[1] = G[1];
                  M[2] = G[2];
                  M[3] = G[3];
            }
            if (M[0] < 0)
                  return M[0]+P;
            return M[0];
      }

      void fft (vector<unsigned long long>& a, bool invert) {
            static const long long mod = 469762049;
            static const long long root = 30;
            static const long long root_1 = revert (root, mod);
            static const long long root_pw = 1 << 26;

            size_t n = a.size();

            for (size_t i = 1, j = 0; i < n; ++i) {
                  size_t bit = n >> 1;
                  for (; j >= bit; bit >>= 1)
                        j -= bit;

                  j += bit;

                  if (i < j)
                        swap (a[i], a[j]);
            }

            for (size_t len = 2; len <= n; len <<= 1) {

                  unsigned long long wlen = invert ? root_1 : root;

                  for (size_t i = len; i < root_pw; i <<= 1)
                        wlen = (wlen*wlen) % mod;

                  for (size_t i = 0; i < n; i += len) {
                        unsigned long long w = 1;

                        for (size_t j = 0; j < len / 2; ++j) {

                              unsigned long long u = a[i+j], v = (a[i+j+len/2]*w) % mod;

                              a[i+j] = (u + v < mod) ? (u + v) : (u + v - mod);
                              a[i+j+len/2] = (u >= v) ? (u - v) : ((u + mod) - v);

                              w = (w*wlen) % mod;
                        }
                  }
            }

            if (invert) {
                  unsigned long long nrev = revert (n, mod);
                  for (size_t i = 0; i < n; ++i)
                        a[i] = (a[i] * nrev) % mod;
            }
            else
                  for (size_t i = 0; i < n; ++i)
                        a[i] %= mod;
      }
}
