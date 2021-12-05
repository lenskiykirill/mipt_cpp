#include <residue.h>

int main () {
      Residue<7> a = 6;
      Residue<7> b = 5;
      std::cout << static_cast<long long> (a*b);
}
