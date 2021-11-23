#include <cstddef>
#include <iostream>
#include <cassert>

class BigInteger {
      friend std::ostream& operator<< (std::ostream&, const BigInteger&);
      friend std::istream& operator>> (std::istream&, BigInteger&);

      public:
            BigInteger (long long init_value, unsigned long long radix = 0);
            ~BigInteger ();

            bool operator== (const BigInteger& other) const;
            bool operator!= (const BigInteger& other) const;
            bool operator<  (const BigInteger& other) const;
            bool operator>  (const BigInteger& other) const;
            bool operator<= (const BigInteger& other) const;
            bool operator>= (const BigInteger& other) const;

            BigInteger& operator+= (const BigInteger& other);
            BigInteger& operator-= (const BigInteger& other);
//            BigInteger& operator^= (const BigInteger& other);
//            BigInteger& operator&= (const BigInteger& other);
//            BigInteger& operator|= (const BigInteger& other);
            BigInteger& operator*= (const BigInteger& other);
            BigInteger& operator/= (const BigInteger& other);
            BigInteger& operator%= (const BigInteger& other);
            BigInteger& operator++ ();
            BigInteger  operator++ (int postfix);

      private:
            static const unsigned long long default_radix = 100;
            
            bool is_m_deceimal;           // is radix a power of 10?
            unsigned int power;           // if (is_m_deceimal), what is the power?

            unsigned long long radix;
            
            bool is_negative;             // Holds true if BigInteger is < 0. false if = 0.

            size_t size;                  // Holds number of digits. 0 if BigInteger = 0
            size_t allocated_size;        // Holds allocated size. Easy-peasy
            unsigned long long* digits;   // The array of digits. Simple

            /* Hidden magic goes here */

            void rebase (unsigned long long new_radix); // Change radix. Disabled for now

            void push_back (unsigned long long digit);  // Add a new digit to the end of array
            void resize ();                             // Change allocated size and copy bytes

            void pos_add (BigInteger& other); // Addition but it discards minuses
            void pos_sub (BigInteger& other); // Substraction but -/~/-

            bool pos_cmp (BigInteger& other); // Operator < but -/~/-
            bool pos_eq  (BigInteger& other); // Operator == but -/~/-
};

BigInteger::BigInteger (long long init_value, unsigned long long radix) :
      is_m_deceimal (true), power (0), is_negative (false),
      size (0), digits (nullptr), allocated_size (0)

{
      if (radix == 0)
      {
            this->radix = radix = default_radix;
            for (unsigned long long r = radix; r > 0; r /= 10)
                  ++power;
            --power;
      }
      else
            is_m_deceimal = false;

      if (init_value == 0)
            return;

      if (init_value < 0)
      {
            is_negative = true;
            init_value = -init_value;
      }

      while (init_value > 0)
      {
            push_back (init_value % radix);
            init_value /= radix;
      }
}

BigInteger::~BigInteger () {
      if ((size != 0) && digits)
            delete [] digits;
}

/* Hidden magic happens here */

void BigInteger::push_back (unsigned long long digit)
{
      if (size != allocated_size)
      {
            digits[size++] = digit;
            return;
      }

      allocated_size = (allocated_size*3 + 3) / 2;

      unsigned long long* digit_array = new unsigned long long [allocated_size];
      for (size_t i = 0; i < size; ++i)
            digit_array[i] = digits[i];

      if (digits)
            delete [] digits;

      digits = digit_array;
      digits[size++] = digit;
}

bool BigInteger::pos_eq (const BigInteger& other) const
{
      if (size != other.size)
            return false;
      for (size_t i = 0
}

std::ostream& operator<< (std::ostream& out, const BigInteger& big_integer)
{
      assert (big_integer.is_m_deceimal);

      if (big_integer.is_negative)
            out << '-';

      for (size_t i = big_integer.size; i > 0; --i)
      {
            unsigned int unused = big_integer.power;
            for (unsigned long long p = big_integer.digits[i-1]; p > 0; p /= 10)
                  --unused;

            if (i != big_integer.size)
                  for (unsigned int i = 0; i < unused; ++i)
                        out << 0;
            
            out << big_integer.digits[i-1];
      }

      return out;
}

/*      DANGER!!! TESTING TERRITORY!!! NO CLASS CODE BEYOUND THIS POINT!!!         */

int main () {
      BigInteger a(-203);
      std::cout << a << '\n';
}
