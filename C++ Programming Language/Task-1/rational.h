#include <cstddef>
#include <iostream>
#include <cassert>
#include <sstream>

#define __DEFAULT_BIGINTEGER_RADIX 1000000

class BigInteger {
      friend std::ostream& operator<< (std::ostream&, const BigInteger&);
      friend std::istream& operator>> (std::istream&, BigInteger&);

      friend BigInteger operator+ (const BigInteger& first, const BigInteger& second);
      friend BigInteger operator- (const BigInteger& first, const BigInteger& second);
      friend BigInteger operator* (const BigInteger& first, const BigInteger& second);
      friend BigInteger operator/ (const BigInteger& first, const BigInteger& second);
      friend BigInteger operator% (const BigInteger& first, const BigInteger& second);

      public:
            BigInteger (long long init_value, unsigned long long radix = 0);
            BigInteger (const BigInteger& other);
            BigInteger ();
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

            BigInteger  operator-  () const; 
            
            explicit operator bool () const;
            explicit operator long long () const;

            std::string toString () const;

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

BigInteger::BigInteger () :
      is_m_deceimal (true), power (0), is_negative (false),
      size (0), digits (nullptr), allocated_size (0)
{
      for (unsigned long r = radix; r > 0; r /= 10)
            ++power;
      --power;
}

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
      if ((size != 0) || digits)
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

//            delete [] digits;
//            digits = nullptr;
//            allocated_size = 0;
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

      ans.is_negative = is_negative;

      *this = ans;
}

void BigInteger::pos_div (const BigInteger& other)
{
      BigInteger ans (0);
      BigInteger tmp_ans (0);
      BigInteger bin_search (0);

      tmp_ans.resize (other.size+1);
      ans.resize (size - other.size + 1);
      ans.size = (size - other.size + 1);

      push_back (0);

      for (size_t i = size-other.size; i > 0; --i)
      {
            tmp_ans.size = 0;

            for (size_t j = i; j < size+1; ++j)
            {
                  tmp_ans.digits[j-i] = digits[j-1];
                  ++tmp_ans.size;
            }
            
            unsigned long long left = 0;
            unsigned long long right = radix;
            
            while ((tmp_ans.size > 0) && (tmp_ans.digits[tmp_ans.size-1] == 0))
                  --tmp_ans.size;

            while (right-left > 1)
            {
                  unsigned long long middle = (left+right) / 2;
                  
                  bin_search = middle;
                  bin_search *= other;

                  if (bin_search <= tmp_ans)
                        left = middle;
                  else
                        right = middle;
            }

            ans.digits[i-1] = left;
            
            bin_search = left;
            bin_search *= other;

            tmp_ans -= bin_search;
            if (tmp_ans.size > 0)
                  for (size_t j = i; j < size+1; ++j)
                        digits[j-1] = tmp_ans.digits[j-i];
            else
                  for (size_t j = i; j < size+1; ++j)
                        digits[j-1] = 0;

            --size;
      }

      while ((ans.size > 0) && (ans.digits[ans.size-1] == 0))
            --ans.size;
      if (ans.size == 0) {
            delete [] ans.digits;
            ans.digits = nullptr;
      }

      *this = ans;
}

void BigInteger::pos_rem (const BigInteger& other)
{
      BigInteger r (*this);
      r /= other;
      r *= other;

      (*this) -= r;
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

      if (size == 0)
            return *this;

      pos_mul (other);

      if (other.is_negative) {
            is_negative = !is_negative;
      }

      return *this;
}

BigInteger& BigInteger::operator/= (const BigInteger& other)
{
      if ((size > 0) && (other.size > 0))
            pos_div (other);

      return *this;
}

BigInteger& BigInteger::operator%= (const BigInteger& other)
{
      if ((size > 0) && (other.size > 0))
            pos_rem (other);

      return *this;
}

BigInteger BigInteger::operator- () const
{
      BigInteger ans (*this);
      if (size == 0)
            return ans;

      ans.is_negative = !is_negative;
      return ans;
}

BigInteger::operator long long () const
{
      long long pow = 1;
      long long res = 0;

      for (size_t i = 0; i < size; ++i)
      {
            res += digits[i]*pow;
            pow *= radix;
      }

      if (is_negative)
            res = -res;

      return res;
}

BigInteger::operator bool () const
{
      return size != 0;
}

BigInteger operator+ (const BigInteger& first, const BigInteger& second)
{
      BigInteger ans (first);
      ans += second;

      return ans;
}

BigInteger operator- (const BigInteger& first, const BigInteger& second)
{
      BigInteger ans (first);
      ans -= second;
      return ans;
}

BigInteger operator* (const BigInteger& first, const BigInteger& second)
{
      BigInteger ans (first);
      ans *= second;
      return ans;
}

BigInteger operator/ (const BigInteger& first, const BigInteger& second)
{
      BigInteger ans (first);
      ans /= second;
      return ans;
}

BigInteger operator% (const BigInteger& first, const BigInteger& second)
{
      BigInteger ans (first);
      ans %= second;
      return ans;
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

std::istream& operator>> (std::istream& in, BigInteger& big_integer)
{
      big_integer = BigInteger (0);
      std::string s;
      
      in >> s;

      size_t begin = 0;

      if (s[0] == '-')
      {
            big_integer.is_negative = true;
            ++begin;
      }

      if (s[begin] == '0')
            return in;

      size_t size = s.length ();
      big_integer.size = 0;

      std::string digit;
      big_integer.resize ((size-begin) / big_integer.power + 1);

      for (size_t i = 0; i < (size-begin) / big_integer.power; ++i)
      {
            digit = s.substr (size - (i+1) * big_integer.power, big_integer.power);
            unsigned long long dec_digit = std::stoi (digit);

            big_integer.digits[i] = dec_digit;
            ++big_integer.size;
      }

      size_t length = (size-begin) % big_integer.power;

      if (length)
      {
            digit = s.substr (begin, length);
            unsigned long long dec_digit = std::stoi (digit);

            big_integer.digits[big_integer.size++] = dec_digit;
      }

      return in;
}

std::string BigInteger::toString () const
{
      std::ostringstream s;
      s << *this;
      return s.str();
}

/*      DANGER!!! TESTING TERRITORY!!! NO CLASS CODE BEYOUND THIS POINT!!!         */

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
            
            std::string toString () const;
            std::string asDecimal (size_t precision=0) const;

            explicit operator double () const;
      private:
            BigInteger nom;
            BigInteger denom;

             void gcd ();
};

Rational::Rational (const BigInteger& init_value) :
      denom (1), nom (init_value)
{}

Rational::Rational (long long init_value) :
      denom (1), nom (init_value)
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

int main () {
      Rational R (-1, 6);
      Rational L (-12, 2);

      std::cout << static_cast<double> (R+L) << std::endl;
}
