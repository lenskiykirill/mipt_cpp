#include "biginteger.h"

int main () {
      BigInteger a, b;

      std::cin >> a >> b;

      BigInteger c, d;

      c = a % b;
      d = a / b;

      b = b*d + c;

      if (b != a)
            std::cout << "ERROR!\n";
      else
            std::cout << "OK\n";
}
