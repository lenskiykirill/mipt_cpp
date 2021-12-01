#include <iostream>

namespace dst {
      unsigned long long gcd (unsigned long long a, unsigned long long p) {
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
}

int main () {
      unsigned long long a, p;
      std::cin >> a >> p;
      std::cout << dst::gcd (a, p) << '\n';

      return 0;
}
