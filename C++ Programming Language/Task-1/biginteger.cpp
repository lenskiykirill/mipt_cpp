#include <cstddef>
#include <iostream>
#include <cassert>

#define __DEFAULT_BIGINTEGER_RADIX 10

class BigInteger {
      friend std::ostream& operator<< (std::ostream&, const BigInteger&);
      friend std::istream& operator>> (std::istream&, BigInteger&);

      public:
            BigInteger (long long init_value, unsigned long long radix = 0);
            BigInteger (const BigInteger& other);
            ~BigInteger ();

            const BigInteger& operator= (const BigInteger& other);

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
            BigInteger& operator-- ();
            BigInteger  operator-- (int postfix);

      private:
            static const unsigned long long default_radix = __DEFAULT_BIGINTEGER_RADIX;
            
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
            void resize (size_t new_size);                             // Change allocated size and copy bytes

            void pos_add (const BigInteger& other); // Addition but it discards minuses
            void pos_sub (const BigInteger& other); // Substraction but -/~/-
            void pos_inc ();
            void pos_dec ();

            bool pos_cmp (const BigInteger& other) const; // Operator < but -/~/-
            bool pos_eq  (const BigInteger& other) const; // Operator == but -/~/-

            void pos_mul (const BigInteger& other);
            void pos_div (const BigInteger& other);
            void pos_rem (const BigInteger& other);
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

BigInteger::BigInteger (const BigInteger& other)
{
      radix = other.radix;
      is_m_deceimal = other.is_m_deceimal;
      power = other.power;

      if (other.size == 0) {
            digits = nullptr;
            allocated_size = 0;
            size = 0;

            return;
      }

      digits = new unsigned long long [other.allocated_size];
      
      for (size_t i = 0; i < other.size; ++i)
            digits[i] = other.digits[i];

      allocated_size = other.allocated_size;
      size = other.size;

      is_negative = other.is_negative;
}

BigInteger::~BigInteger () {
      if ((size != 0) && digits)
            delete [] digits;
}

const BigInteger& BigInteger::operator= (const BigInteger& other)
{
      if (this == &other)
            return *this;

      radix = other.radix;
      is_m_deceimal = other.is_m_deceimal;
      power = other.power;

      if ((size != 0) && digits)
      {
            delete [] digits;
      }

      if (other.size == 0)
      {
            digits = nullptr;
            allocated_size = 0;
            size = 0;

            return *this;
      }

      digits = new unsigned long long [other.allocated_size];
      for (size_t i = 0; i < other.size; ++i)
            digits[i] = other.digits[i];

      allocated_size = other.allocated_size;
      size = other.size;

      is_negative = other.is_negative;

      return *this;
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

void BigInteger::resize (size_t new_size) {
      allocated_size = new_size;

      unsigned long long* digit_array = new unsigned long long [allocated_size];
      for (size_t i = 0; i < size; ++i)
            digit_array[i] = digits[i];

      if (digits)
            delete [] digits;

      digits = digit_array;
}

bool BigInteger::pos_eq (const BigInteger& other) const
{
      if (size != other.size)
            return false;

      for (size_t i = size; i > 0; --i)
            if (other.digits[i-1] != digits[i-1])
                  return false;
      return true;
}

bool BigInteger::pos_cmp (const BigInteger& other) const
{
      if (size != other.size)
            return size < other.size;

      for (size_t i = size; i > 0; --i)
            if (digits[i-1] != other.digits[i-1])
                  return digits[i-1] < other.digits[i-1];

      return false;
}

bool BigInteger::operator== (const BigInteger& other) const
{
      if (is_negative != other.is_negative)
            return false;
      return pos_eq (other);
}

bool BigInteger::operator!= (const BigInteger& other) const
{
      return !(*this == other);
}

bool BigInteger::operator< (const BigInteger& other) const
{
      if (is_negative != other.is_negative)
            return is_negative;

      if (is_negative)
            return !pos_cmp (other);
      return pos_cmp (other);
}

bool BigInteger::operator> (const BigInteger& other) const
{
      return other < *this;
}

bool BigInteger::operator<= (const BigInteger& other) const
{
      return !(other < *this);
}

bool BigInteger::operator>= (const BigInteger& other) const
{
      return !(*this < other);
}

void BigInteger::pos_add (const BigInteger& other) {
      if (other.size == 0)
            return;

      unsigned long long carry = 0;
      unsigned long long result;

      if (pos_cmp (other)) // if abs(*this) < abs(other)
      {
            if (allocated_size < other.size+1)
                  resize (other.size+1);

            for (size_t i = 0; i < size; ++i)
            {
                  result = digits[i] + other.digits[i] + carry;
                  carry = result / radix;
                  digits[i] = result % radix;
            }

            for (size_t i = size; i < other.size; ++i) {
                  result = other.digits[i] + carry;
                  carry = result / radix;
                  digits[i] = result % radix;
            }

            size = other.size;

            if (carry)
                  digits[size++] = carry;

            return;
      }

      if (allocated_size == size)
            resize ((size*3 + 3) / 2);

      for (size_t i = 0; i < other.size; ++i)
      {
            result = digits[i] + other.digits[i] + carry;
            carry = result / radix;
            digits[i] = result % radix;
      }

      for (size_t i = other.size; i < size; ++i)
      {
            result = digits[i] + carry;
            carry = result / radix;
            digits[i] = result % radix;
      }

      if (carry)
            digits[size++] = carry;
}

void BigInteger::pos_sub (const BigInteger& other)
{
      unsigned long long carry = 0;

      for (size_t i = 0; i < other.size; ++i)
      {
            if (digits[i] < other.digits[i]+carry)
            {
                  digits[i] += radix;
                  digits[i] -= (other.digits[i] + carry);
                  carry = 1;
            }

            else
            {
                  digits[i] -= (other.digits[i] + carry);
                  carry = 0;
            }
      }
      
      for (size_t i = other.size; i < size; ++i)
      {
            if (digits[i] >= carry)
            {
                  digits[i] -= carry;
                  break;
            }

            digits[i] += (radix - carry);
      }

      size_t i = size;
      while ((i > 0) && (digits[i-1] == 0))
      {
            --size;
            --i;
      }

      if (size == 0)
      {
            is_negative = false;

            delete [] digits;
            digits = nullptr;
            allocated_size = 0;
      }
}

void BigInteger::pos_inc ()
{
      unsigned long long carry = 1;

      for (size_t i = 0; i < size; ++i)
      {
            unsigned long long result = digits[i] + carry;
            carry = result / radix;
            digits[i] = result % carry;
      }

      if (carry)
            push_back (carry);
}

void BigInteger::pos_dec ()
{
      unsigned long long carry = 1;

      for (size_t i = 0; i < size; ++i) {
            if (digits[i] >= carry)
            {
                  digits[i] -= carry;
                  break;
            }

            digits[i] += (radix-carry);
      }

      size_t i = size;
      while ((i > 0) && (digits[i-1] == 0))
      {
            --size;
            --i;
      }

      if (size == 0)
      {
            is_negative = false;

            delete [] digits;
            digits = nullptr;
            allocated_size = 0;
      }
}

/*      MUL, DIV, REM                                */

void BigInteger::pos_mul (const BigInteger& other)
{
      BigInteger ans (0);
      BigInteger tmp_ans (0);

      tmp_ans.resize (size + other.size + 1);
      ans.resize (size + other.size + 1);

      for (size_t shift = 0; shift < other.size; ++shift)
      {
            unsigned long long mult = other.digits[shift];
            unsigned long long carry = 0;
            tmp_ans.size = shift;

            for (size_t i = 0; i < size; ++i)
            {
                  unsigned long long result = digits[i]*mult + carry;
                  carry = result / radix;

                  tmp_ans.digits[i+shift] = result % radix;
                  ++tmp_ans.size;
            }

            if (carry)
            {
                  tmp_ans.digits[size+shift] = carry;
                  ++tmp_ans.size;
            }

            if (shift > 0)
                  tmp_ans.digits[shift-1] = 0;

            ans += tmp_ans;
      }

      *this = ans;
}

/*      Magic ends here and routine starts:(         */


BigInteger& BigInteger::operator+= (const BigInteger& other)
{
      if (is_negative == other.is_negative)
      {
            pos_add (other);
            return *this;
      }

      if (!pos_cmp (other))
            pos_sub (other);
      else
      {
            BigInteger tmp (other);
            tmp.pos_sub (*this);
            *this = tmp;

            is_negative = !is_negative;
      }
      
      return *this;
}

BigInteger& BigInteger::operator-= (const BigInteger& other)
{
      if (is_negative == other.is_negative)
      {
            if (!pos_cmp (other))
                  pos_sub (other);
            else
            {
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

BigInteger& BigInteger::operator++ ()
{
      if (!is_negative)
            pos_inc ();
      else
            pos_dec ();

      return *this;
}

BigInteger BigInteger::operator++ (int postfix)
{
      BigInteger result (*this);

      ++(*this);

      return result;
}

BigInteger& BigInteger::operator-- ()
{
      if (size == 0)
            is_negative = true;

      if (is_negative)
            pos_inc ();
      else
            pos_dec ();

      return *this;
}

BigInteger BigInteger::operator-- (int postfix)
{
      BigInteger result (*this);

      --(*this);

      return result;
}

BigInteger& BigInteger::operator*= (const BigInteger& other)
{
      if (other.size == 0)
      {
            *this = other;
            return *this;
      }

      pos_mul (other);

      if (other.is_negative) {
            is_negative = !is_negative;
      }

      return *this;
}

std::ostream& operator<< (std::ostream& out, const BigInteger& big_integer)
{
      assert (big_integer.is_m_deceimal);

      if (big_integer.size == 0)
      {
            out << 0;
            return out;
      }

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
            if (big_integer.digits[i-1])
                  out << big_integer.digits[i-1];
      }

      return out;
}

/*      DANGER!!! TESTING TERRITORY!!! NO CLASS CODE BEYOUND THIS POINT!!!         */

int main () {
      long long x, y;
      std::cin >> x >> y;

      BigInteger a (x);
      BigInteger b (y);

      a *= b;

      std::cout << (a) << '\n';
}
