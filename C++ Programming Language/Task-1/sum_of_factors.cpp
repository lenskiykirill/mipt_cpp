#include <iostream>

long long f_sum (long long** A, size_t* n, size_t size) {

      long long s = 0;

      size_t* idx = new size_t[size];

      for (size_t i = 0; i < size; i++) {
            idx[i] = 0;
      }

      while (true) {
            
            long long mul = 1;
            
            for (size_t i=0; i < size; ++i) {
                  mul *= A[i][idx[i]];
            }

            std::cout << "[";
            for (size_t i=0; i < size-1; ++i) {
                  std::cout << idx[i] << ", ";
            }
            std::cout << idx[size-1] << "]\n";

            s += mul;
            
            size_t j = 0;

            ++idx[j];
            
            while ((idx[j] == n[j]) && (j != size)) {
                  idx[j] = 0;
                  ++j;
                  ++idx[j];
            }

            if (j == size) {
                  break;
            }
      }

      return s;
}

int main (int argc, const char** argv) {
      
      size_t* A = new size_t [argc-1];

      for (int i = 0; i < argc-1; ++i) { A[i] = atoi (argv[i+1]); }
      
      long long** B = new long long* [argc-1];

      for (int i=0; i < argc-1; ++i) {

            B[i] = new long long[A[i]];

            for (size_t j = 0; j < A[i]; ++j) {
                  std::cin >> B[i][j];
            }
      }

      std::cout << f_sum (B, A, static_cast<size_t> (argc-1)) << '\n';

      for (int i = 0; i < argc-1; ++i) {
            delete [] B[i];
      }

      delete [] A;
      delete [] B;

      return 0;
}
