#include <iostream>
#include <cassert>
#include <string>
#include <sstream>

#define __BIGINTEGER_DEFAULT_RADIX 100
#define __BIGINTEGER_DEFAULT_RADIX_POWER 2

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

/* Well, I did my best to write a decent fft on my own. I failed. So I turned to the internet... https://e-maxx.ru/algo/fft_multiply */


class BigInteger {
      friend std::ostream& operator<< (std::ostream&, const BigInteger&);
      friend std::istream& operator>> (std::istream&, BigInteger&);

      friend BigInteger operator+ (const BigInteger&, const BigInteger&);
      friend BigInteger operator- (const BigInteger&, const BigInteger&);
      friend BigInteger operator* (const BigInteger&, const BigInteger&);
      friend BigInteger operator/ (const BigInteger&, const BigInteger&);
      friend BigInteger operator% (const BigInteger&, const BigInteger&);

      public:
            BigInteger ();
            BigInteger (long long init_value);
            BigInteger (const BigInteger& other);
            ~BigInteger ();

            BigInteger& operator= (const BigInteger& other);

            bool operator== (const BigInteger& other) const;
            bool operator!= (const BigInteger& other) const;
            bool operator<  (const BigInteger& other) const;
            bool operator>  (const BigInteger& other) const;
            bool operator<= (const BigInteger& other) const;
            bool operator>= (const BigInteger& other) const;

            BigInteger& operator+= (const BigInteger& other);
            BigInteger& operator-= (const BigInteger& other);
            BigInteger& operator*= (const BigInteger& other);
            BigInteger& operator/= (const BigInteger& other);
            BigInteger& operator%= (const BigInteger& other);
            BigInteger& operator++ ();
            BigInteger  operator++ (int);
            BigInteger& operator-- ();
            BigInteger  operator-- (int);

            BigInteger  operator- () const;

            explicit operator bool () const;
            explicit operator long long () const;

            std::string toString () const;

      private:
            static const unsigned long long default_radix = __BIGINTEGER_DEFAULT_RADIX;

            unsigned int radix_power = __BIGINTEGER_DEFAULT_RADIX_POWER;

            unsigned long long radix;

            bool is_negative;

            dst::vector<unsigned long long> digits;

            void pos_add (const BigInteger& other);
            void pos_sub (const BigInteger& other);
            void pos_inc ();
            void pos_dec ();
            
            bool pos_cmp (const BigInteger& other) const;
            bool pos_eq  (const BigInteger& other) const;

            void pos_mul (const BigInteger& other);
            void pos_div (const BigInteger& other);
            void pos_rem (const BigInteger& other);

            void digit_mul (unsigned long long digit);
            
            static const unsigned int mod = 469762049;
};

BigInteger::BigInteger () :
      radix (BigInteger::default_radix),
      is_negative (false), digits (0) {}

BigInteger::BigInteger (long long init_value) :
      radix (BigInteger::default_radix),
      is_negative (false), digits (0) {
      
            if (init_value < 0) {
                  is_negative = true;
                  init_value = -init_value;
            }

            while (init_value > 0) {
                  digits.push_back (init_value % radix);
                  init_value /= radix;
            }
      }

BigInteger::BigInteger (const BigInteger& other) :
      radix (BigInteger::default_radix),
      is_negative (other.is_negative),
      digits (other.digits) {}

BigInteger::~BigInteger () {}

BigInteger& BigInteger::operator= (const BigInteger& other) {
      is_negative = other.is_negative;
      
      radix = other.radix;
      
      digits = other.digits;

      return *this;
}

bool BigInteger::pos_eq (const BigInteger& other) const {
      if (digits.size() != other.digits.size())
            return false;

      for (size_t i = digits.size(); i > 0; --i)
            if (digits[i-1] != other.digits[i-1])
                  return false;

      return true;
}

bool BigInteger::pos_cmp (const BigInteger& other) const {
      if (digits.size() != other.digits.size())
            return digits.size() < other.digits.size();

      for (size_t i = digits.size(); i > 0; --i)
            if (digits[i-1] != other.digits[i-1])
                  return digits[i-1] < other.digits[i-1];

      return false;
}

void BigInteger::pos_add (const BigInteger& other) {
      if (other.digits.size() == 0)
            return;
      
      unsigned long long carry = 0;
      unsigned long long result;

      if (pos_cmp (other)) {
            size_t this_size = digits.size();

            digits.resize (other.digits.size() + 1);
            
            for (size_t i = 0; i < this_size; ++i) {
                  result = digits[i] + other.digits[i] + carry;
                  carry = result / radix;

                  digits[i] = result % radix;
            }

            for (size_t i = this_size; i < other.digits.size(); ++i) {
                  result = other.digits[i] + carry;
                  carry = result / radix;

                  digits[i] = result % radix;
            }
      }
      
      else {
            size_t this_size = digits.size();

            digits.resize (digits.size()+1);

            for (size_t i = 0; i < other.digits.size(); ++i) {
                  result = digits[i] + other.digits[i] + carry;
                  carry = result / radix;
                  
                  digits[i] = result % radix;
            }

            for (size_t i = other.digits.size(); i < this_size; ++i) {
                  result = digits[i] + carry;
                  carry = result / radix;

                  digits[i] = result % radix;
            }
      }

      if (carry)
            digits.back() = carry;
      else
            digits.resize (digits.size() - 1);
}

void BigInteger::pos_sub (const BigInteger& other) {
      unsigned long long carry = 0;

      for (size_t i = 0; i < other.digits.size(); ++i) {
            if (digits[i] < other.digits[i] + carry) {
                  digits[i] += radix;
                  digits[i] -= (other.digits[i] + carry);

                  carry = 1;
            } else {
                  digits[i] -= (other.digits[i] + carry);

                  carry = 0;
            }
      }

      for (size_t i = other.digits.size(); i < digits.size(); ++i) {
            if (digits[i] >= carry) {
                  digits[i] -= carry;

                  break;
            }

            digits[i] += (radix - carry);
      }

      while ((digits.size() > 0) && (digits.back() == 0))
            digits.pop_back ();
}

void BigInteger::pos_inc () {
      unsigned long long carry = 1;
      unsigned long long result;

      for (size_t i = 0; i < digits.size(); ++i) {
            result = digits[i] + carry;
            carry = result / radix;

            digits[i] = result % radix;
      }

      if (carry)
            digits.push_back (carry);
}

void BigInteger::pos_dec () {
      unsigned long long carry = 1;

      for (size_t i = 0; i < digits.size(); ++i) {
            if (digits[i] >= carry) {
                  digits[i] -= carry;

                  break;
            }

            digits[i] += (radix-carry);
      }

      while ((digits.size() > 0) && (digits.back() == 0))
            digits.pop_back();
}

void BigInteger::pos_mul (const BigInteger& other) {
      dst::vector<unsigned long long> fa (digits), fb (other.digits);
      size_t n = 1;

      while ((n < digits.size()) || (n < other.digits.size()))
            n <<= 1;
      n <<= 1;

      fa.resize (n);
      fb.resize (n);

      for (size_t i = digits.size(); i < n; ++i)
            fa[i] = 0;
      for (size_t i = other.digits.size(); i < n; ++i)
            fb[i] = 0;

      dst::fft (fa, false);
      dst::fft (fb, false);

      for (size_t i = 0; i < n; ++i)
            fa[i] = (fa[i] * fb[i]) % BigInteger::mod;


      dst::fft (fa, true);
      
      digits = fa;

      unsigned long long carry = 0;

      for (size_t i = 0; i < n; ++i) {
            digits[i] += carry;
            carry = digits[i] / radix;

            digits[i] %= radix;
      }

      for (size_t i = digits.size(); i > 0; --i) {
            if (digits[i-1] != 0)
                  break;
            digits.resize (i-1);
      }
}

void BigInteger::digit_mul (unsigned long long digit) {
      //if (digit == 0)
      //      digits.resize(0);

      digits.resize (digits.size() + 1);

      unsigned long long carry = 0;
      unsigned long long result;

      for (size_t i = 0; i < digits.size()-1; i++) {
            result = digits[i] * digit + carry;
            carry = result / radix;

            digits[i] = result % radix;
      }

      digits.back() = carry;
}

void BigInteger::pos_div (const BigInteger& other) {
      BigInteger res (0);

      if (digits.size() < other.digits.size()) {
            *this = res;
            return;
      }

      digits.push_back (0);

      res.digits.resize (digits.size() - other.digits.size());

      BigInteger tmp, subst;

      for (size_t i = digits.size() - other.digits.size(); i > 0; --i) {
            size_t left = 0, right = radix, middle;

            tmp.digits.resize (other.digits.size() + 1);
            
            for (size_t j = 0; j < other.digits.size()+1; ++j)
                  tmp.digits[j] = digits[i+j-1];
            
            while (right - left > 1) {
                  middle = (right + left) / 2;

                  subst = other;
                  subst.digit_mul (middle);

                  if (!tmp.pos_cmp(subst))
                        left = middle;
                  else
                        right = middle;
            }

            subst = other;
            subst.digit_mul (left);

            tmp.pos_sub (subst);

            for (size_t j = 0; j <= other.digits.size(); ++j)
                  digits[i+j-1] = tmp.digits[j];

            digits.resize (digits.size() - 1);
            
            res.digits[i-1] = left;;
      }

      *this = res;
      pos_sub (0);
}

void BigInteger::pos_rem (const BigInteger& other) {
      if (digits.size() < other.digits.size())
            return;

      digits.push_back (0);
      BigInteger tmp, subst;

      for (size_t i = digits.size() - other.digits.size(); i > 0; --i) {
            size_t left = 0, right = radix, middle;

            tmp.digits.resize (other.digits.size() + 1);
            
            for (size_t j = 0; j < other.digits.size()+1; ++j)
                  tmp.digits[j] = digits[i+j-1];
            
            while (right - left > 1) {
                  middle = (right + left) / 2;

                  subst = other;
                  subst.digit_mul (middle);

                  if (!tmp.pos_cmp (subst))
                        left = middle;
                  else
                        right = middle;
            }

            subst = other;
            subst.digit_mul (left);

            tmp.pos_sub (subst);

            for (size_t j = 0; j <= other.digits.size(); ++j)
                  digits[i+j-1] = tmp.digits[j];

            digits.resize (digits.size() - 1);
      }
      
      pos_sub (0);
}

/* Operator definitions */

bool BigInteger::operator== (const BigInteger& other) const {
      if (is_negative != other.is_negative)
            return false;
      return pos_eq (other);
}

bool BigInteger::operator!= (const BigInteger& other) const {
      return !(*this == other);
}

bool BigInteger::operator<  (const BigInteger& other) const {
      if (is_negative != other.is_negative)
            return is_negative;

      return is_negative != pos_cmp (other);
}

bool BigInteger::operator>  (const BigInteger& other) const {
      return other < *this;
}

bool BigInteger::operator<= (const BigInteger& other) const {
      return !(other < *this);
}

bool BigInteger::operator>= (const BigInteger& other) const {
      return !(*this < other);
}

BigInteger& BigInteger::operator+= (const BigInteger& other) {
      
      if (this == &other) {
            BigInteger x (other);
            pos_add (x);
            return *this;
      }

      if (is_negative == other.is_negative) {
            pos_add (other);
            return *this;
      }

      if (!pos_cmp (other))
            pos_sub (other);
      else {
            BigInteger tmp (other);
            tmp.pos_sub (*this);
            
            *this = tmp;
            //is_negative = !is_negative;
      }

      return *this;
}

BigInteger& BigInteger::operator-= (const BigInteger& other) {
      if (this == &other) {
            *this = BigInteger (0);
            return *this;
      }

      if (is_negative == other.is_negative) {
            if (!pos_cmp (other))
                  pos_sub (other);
            else {
                  BigInteger tmp (other);
                  tmp.pos_sub (*this);

                  *this = tmp;
                  is_negative = !is_negative;
            }

            return *this;
      }

      pos_add (other);
      return *this;
}

BigInteger& BigInteger::operator++ () {
      if (!is_negative)
            pos_inc ();
      else
            pos_dec ();

      return *this;
}

BigInteger& BigInteger::operator-- () {
      
      if (digits.size() == 0)
            is_negative = true;
      
      if (!is_negative)
            pos_dec ();
      else
            pos_inc ();

      return *this;
}

BigInteger BigInteger::operator++ (int) {
      BigInteger res (*this);

      ++(*this);

      return res;
}

BigInteger BigInteger::operator-- (int) {
      BigInteger res (*this);

      --(*this);

      return res;
}

BigInteger& BigInteger::operator*= (const BigInteger& other) {

      if (this == &other) {
            BigInteger x (other);
            pos_mul (x);

            is_negative = false;
            return *this;
      }

      if ((digits.size() == 0) || (other.digits.size() == 0)) {
            *this = BigInteger (0);
            return *this;
      }

      pos_mul (other);

      is_negative = (is_negative != other.is_negative);

      return *this;
}

BigInteger& BigInteger::operator/= (const BigInteger& other) {
      
      if (this == &other) {
            *this = BigInteger (1);
            return *this;
      }

      bool sign = is_negative;

      pos_div (other);

      is_negative = (sign != other.is_negative);

      return *this;
}

BigInteger& BigInteger::operator%= (const BigInteger& other) {
      if (this == &other) {
            *this = BigInteger (0);
            return *this;
      }

      bool sign = is_negative;

      pos_rem (other);

      is_negative = (sign != other.is_negative);

      return *this;
}

BigInteger BigInteger::operator- () const {
      if (digits.size() == 0)
            return *this;
      
      BigInteger res (*this);
      res.is_negative = !is_negative;

      return res;
}

BigInteger operator+ (const BigInteger& left, const BigInteger& right) {
      BigInteger res = left;
      res += right;

      return res;
}

BigInteger operator- (const BigInteger& left, const BigInteger& right) {
      BigInteger res = left;
      res -= right;

      return res;
}

BigInteger operator* (const BigInteger& left, const BigInteger& right) {
      BigInteger res = left;
      res *= right;

      return res;
}

BigInteger operator/ (const BigInteger& left, const BigInteger& right) {
      BigInteger res = left;
      res /= right;

      return res;
}

BigInteger operator% (const BigInteger& left, const BigInteger& right) {
      BigInteger res = left;
      res %= right;

      return res;
}

std::string BigInteger::toString() const {
      std::ostringstream oss;
      oss << *this;

      return oss.str();
}

std::ostream& operator<< (std::ostream& out, const BigInteger& big_int) {
      if (big_int.digits.size() == 0) {
            out << 0;
            return out;
      }

      if (big_int.is_negative)
            out << '-';

      for (size_t i = big_int.digits.size(); i > 0; --i) {
            unsigned int unused = big_int.radix_power;
            
            for (unsigned long long p = big_int.digits[i-1]; p > 0; p /= 10)
                  --unused;

            if (i != big_int.digits.size())
                  for (unsigned int i = 0; i < unused; ++i)
                        out << 0;
            if (big_int.digits[i-1])
                  out << big_int.digits[i-1];
      }

      return out;
}

std::istream& operator>> (std::istream& in, BigInteger& big_int) {
      big_int = BigInteger (0);
      std::string s;

      in >> s;

      size_t begin = 0;

      if (s[0] == '-') {
            big_int.is_negative = true;
            ++begin;
      }

      if ((s.length() - begin == 1) && s[begin] == '0')
            return in;

      assert (s[begin] != '0');

      size_t size = s.length ();
      big_int.digits.resize ((size-begin) / big_int.radix_power + 1);
      big_int.digits.resize (0);

      for (size_t i = 0; i < (size-begin) / big_int.radix_power; ++i) {
            unsigned long long digit = std::stoi (s.substr (size - (i+1)*big_int.radix_power,
                                                            big_int.radix_power
                                                 ));

            big_int.digits.push_back (digit);
      }

      size_t length = (size-begin) % big_int.radix_power;

      if (length) {
            unsigned long long digit = std::stoi (s.substr (begin, length));
            big_int.digits.push_back (digit);
      }

      return in;
}

BigInteger::operator bool () const {
      return digits.size() != 0;
}

BigInteger::operator long long () const {
      long long pow = 1;
      long long res = 0;

      for (size_t i = 0; i < digits.size(); ++i) {
            res += digits[i]*pow;
            pow *= radix;
      }

      res = is_negative ? -res : res;

      return res;
}

/*--------- END OF THE CLASS CODE ---------*/

class Rational {
            friend std::ostream& operator<< (std::ostream& out, const Rational R);
            friend std::istream& operator>> (std::istream& in, Rational R);

            friend Rational operator+ (const Rational& left, const Rational& right);
            friend Rational operator- (const Rational& left, const Rational& right);
            friend Rational operator* (const Rational& left, const Rational& right);
            friend Rational operator/ (const Rational& left, const Rational& right);
      public:
            Rational (long long init_value);
            Rational (const BigInteger& init_value);
            Rational (const BigInteger& nom, const BigInteger& denom);
            Rational (const Rational& other);
            Rational ();

            ~Rational ();

            Rational& operator= (const Rational& other);

            bool operator== (const Rational& other) const;
            bool operator!= (const Rational& other) const;
            bool operator<  (const Rational& other) const;
            bool operator>  (const Rational& other) const;
            bool operator<= (const Rational& other) const;
            bool operator>= (const Rational& other) const;

            Rational& operator+= (const Rational& other);
            Rational& operator-= (const Rational& other);
            Rational& operator*= (const Rational& other);
            Rational& operator/= (const Rational& other);

            Rational  operator- ();
            
            std::string toString () const;
            std::string asDecimal (size_t precision=0) const;

            explicit operator double () const;
      private:
            BigInteger nom;
            BigInteger denom;

             void gcd ();
};

Rational::Rational () :
      nom (0), denom (1)
{}

Rational::Rational (const BigInteger& init_value) :
      nom (init_value), denom (1)
{}

Rational::Rational (long long init_value) :
      nom (init_value), denom (1)
{}

Rational::Rational (const BigInteger& nom, const BigInteger& denom) :
      nom (nom), denom (denom)
{
      gcd ();
}

Rational::Rational (const Rational& other) :
      nom (other.nom), denom (other.denom)
{}

Rational::~Rational () {}

void Rational::gcd ()
{
      bool sign = (nom < BigInteger (0));
      if (sign)
            nom = -nom;

      BigInteger left (nom);
      BigInteger right (denom);
      BigInteger rem (0);

      while (right)
      {
            rem = left % right;
            left = right;
            right = rem;
      }

      nom /= left;
      denom /= left;

      if (sign)
            nom = -nom;
}

Rational& Rational::operator= (const Rational& other)
{
      nom = other.nom;
      denom = other.denom;

      return *this;
}

bool Rational::operator== (const Rational& other) const
{
      if (nom == BigInteger (0))
            return other.nom == BigInteger (0);

      if (nom != other.nom)
            return false;
      return denom == other.denom;
}

bool Rational::operator!= (const Rational& other) const
{
      return !(*this == other);
}

bool Rational::operator< (const Rational& other) const
{
      return nom * other.denom < denom * other.nom;
}

bool Rational::operator> (const Rational& other) const
{
      return other < *this;
}

bool Rational::operator<= (const Rational& other) const
{
      return !(other < *this);
}

bool Rational::operator>= (const Rational& other) const
{
      return !(*this < other);
}

std::ostream& operator<< (std::ostream& out, Rational R)
{
      out << R.nom;
      
      if ((R.nom != 0) && (R.denom != 1))
            out << '/' << R.denom;

      return out;
}

Rational& Rational::operator+= (const Rational& other)
{
      BigInteger new_nom = nom * other.denom + denom * other.nom;
      BigInteger new_denom = denom * other.denom;
      
      nom = new_nom;
      denom = new_denom;

      gcd();

      return *this;
}

Rational& Rational::operator-= (const Rational& other)
{
      BigInteger new_nom = nom * other.denom - denom * other.nom;
      BigInteger new_denom = denom * other.denom;

      nom = new_nom;
      denom = new_denom;

      gcd ();

      return *this;
}

Rational& Rational::operator*= (const Rational& other)
{
      nom *= other.nom;
      denom *= other.denom;

      gcd ();

      return *this;
}

Rational& Rational::operator/= (const Rational& other)
{
      if (other.nom < 0)
      {
            nom = -nom;
            denom = -denom;
      }

      nom *= other.denom;
      denom *= other.nom;

      gcd ();

      return *this;
}

Rational operator+ (const Rational& left, const Rational& right)
{
      Rational R = left;
      R += right;

      return R;
}

Rational operator- (const Rational& left, const Rational& right)
{
      Rational R = left;
      R -= right;

      return R;
}

Rational operator* (const Rational& left, const Rational& right)
{
      Rational R = left;
      R *= right;

      return R;
}

Rational operator/ (const Rational& left, const Rational& right)
{
      Rational R = left;
      R /= right;

      return R;
}

Rational Rational::operator- ()
{
      Rational other (*this);
      other.nom = - other.nom;

      return other;
}

std::string Rational::toString () const
{
      std::ostringstream s;
      s << *this;
      return s.str();
}

std::string Rational::asDecimal (size_t precision) const
{
      std::string s;

      Rational other (*this);
      if (other.nom < 0)
      {
            s += '-';
            other.nom = -other.nom;
      }

      s += (other.nom / other.denom).toString();

      if (precision == 0)
            return s;

      other.nom %= other.denom;
      s += '.';

      for (size_t i = 0; i < precision; ++i)
      {
            other.nom *= 10;
            s += (other.nom / other.denom).toString();
            other.nom %= other.denom;
      }

      return s;
}

Rational::operator double () const
{
      double x = 0;
      BigInteger BI (nom);
      
      bool is_negative = (BI < 0);
      if (is_negative)
            BI = -BI;

      BI *= (2 << 27);
      BI /= denom;

      x = static_cast<double> (static_cast<long long> (BI));
      x /= (2 << 27);

      if (is_negative)
            x = -x;

      return x;
}

