#include <cstddef>
#include <iostream>

class BigInt {
      friend std::ostream& operator<< (std::ostream& out, BigInt& big_integer);

      public:
            // BigInt (unsigned long long init_value);

            BigInt (long long init_value);
            ~BigInt ();

            BigInt& operator+= (BigInt& other);
            BigInt& operator-= (BigInt& other);

            bool operator< (BigInt& other);
            bool operator> (BigInt& other);
            bool operator== (BigInt& other);
            bool operator<= (BigInt& other);
            bool operator>= (BigInt& other);
            bool operator!= (BigInt& other);

      private:
            static const unsigned long long __default_radix = 10;
            bool is_m_decimal;

            unsigned long long radix;
            
            bool is_negative;

            size_t size;
            size_t allocated_size;
            unsigned long long* number_array;

            void push_back (unsigned long long digit);
            void resize (size_t new_size);

            void pos_add (BigInt& other);
            void pos_sub (BigInt& other);
};

BigInt::~BigInt () {
      if (number_array)
            delete [] number_array;
}

void BigInt::resize (size_t new_size) {
      allocated_size = new_size;

      unsigned long long* new_number_array = new unsigned long long [allocated_size];

      for (size_t i = 0; i < size; ++i)
            new_number_array[i] = number_array[i];

      delete [] number_array;
      number_array = new_number_array;
}

void BigInt::push_back (unsigned long long digit) {

      if (!number_array) {
            allocated_size = 3;
            size = 0;
            number_array = new unsigned long long [allocated_size];
      }

      if (size >= allocated_size) {
            resize ((allocated_size*3 + 2)/2);
      }

      number_array[size] = digit;
      ++size;
}

/*BigInt::BigInt (unsigned long long init_value) {
     
      this->radix = radix;

      number_array = nullptr;
      size = 0;
      allocated_size = 0;

      while (init_value > 0) {
            push_back (init_value % radix);
            init_value /= radix;
      }

      is_negative = false;
}*/

BigInt::BigInt (long long init_value) : radix (__default_radix), size (0) {
      
      is_negative = false;

      if (init_value < 0) {
            is_negative = true;
            init_value = -init_value;
      }

      number_array = nullptr;

      for (size_t i = 0; init_value > 0; ++i) {
            push_back (init_value % radix);
            init_value /= radix;
      }
}

BigInt& BigInt::operator+= (BigInt& other) {
     if (is_negative == other.is_negative)
           pos_add (other);

     return *this;
}

BigInt& BigInt::operator-= (BigInt& other) {
      if (is_negative == other.is_negative) {
            if (*this >= other)
                  pos_sub (other);
      }

      return *this;
}

void BigInt::pos_add (BigInt& other) {
      size_t this_size = size;
      size_t other_size = other.size;

      if (other_size > this_size) {
            resize (other_size+1);
            size = other_size;
      }
      
      else {
            resize (size+1);
      }

      unsigned long long carry = 0;

      size_t i = 0;

      for (i = 0; (i < this_size) && (i < other_size); ++i) {
            unsigned long long result = number_array[i] + other.number_array[i];
            result += carry;

            carry = result / radix;
            result %= radix;

            number_array[i] = result;
      }

      for (; i < this_size; ++i) {
            unsigned long long result = number_array[i]+carry;
            carry = result / radix;
            result %= radix;

            number_array[i] = result;
      }

      for (; i < other_size; ++i) {
            unsigned long long result = other.number_array[i];
            result += carry;

            carry = result / radix;
            result %= radix;

            number_array[i] = result;
      }

      if (carry != 0) {
            number_array[i] = carry;
            ++size;
      }
}

void BigInt::pos_sub (BigInt& other) { // we assume *this > other
      size_t i;
      for (i = 0; i < other.size; ++i) {
            if (number_array[i] < other.number_array[i]) {
                  --number_array[i+1];
                  number_array[i] += radix;
            }

            number_array[i] -= other.number_array[i];
      }

      if (i == size) {
            while ((size > 0) && (number_array[size-1] == 0))
                  --size;
      }
}

bool BigInt::operator< (BigInt& other) {
      if (is_negative && !other.is_negative)
            return true;

      if (!is_negative && other.is_negative) {
            return false;
      }

      if (size != other.size)
            return size < other.size;

      for (size_t i = size; i > 0; --i) {
            if (number_array[i-1] != other.number_array[i-1])
                  return number_array[i-1] < other.number_array[i-1];
      }

      return false;
}

bool BigInt::operator> (BigInt& other) {
      return other < *this;
}

bool BigInt::operator== (BigInt& other) {
      if (size != other.size)
            return false;

      for (size_t i = size; i > 0; --i) {
            if (number_array[i-1] != other.number_array[i-1])
                  return false;
      }

      return true;
}

bool BigInt::operator<= (BigInt& other) {
      return !(*this > other);
}

bool BigInt::operator>= (BigInt& other) {
      return !(*this < other);
}

bool BigInt::operator!= (BigInt& other) {
      return !(*this == other);
}

std::ostream& operator<< (std::ostream& out, BigInt& big_int) {
      if (big_int.size == 0) {
            out << 0;
            return out;
      }

      for (size_t i = big_int.size; i > 0; --i)
            out << big_int.number_array[i-1];

      return out;
}

int main () {
      BigInt a = 1000;
      BigInt b = 0;
      a += a;

      std::cout << (a) << '\n';
      return 0;
}
