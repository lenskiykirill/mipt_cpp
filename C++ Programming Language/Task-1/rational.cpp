#include "biginteger.h"

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
