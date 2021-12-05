//#include <cstddef>

template <unsigned long long n, unsigned long long squre>
      struct __is_less_than_square_root {`
            static const bool value = (n*n < square);
      };

template <unsigned long long d, unsigned long long p>
      struct __is_prime {
            static const bool value = (__is_prime<d-1>::value);
      };

template <unsigned long long N, bool is_prime>
