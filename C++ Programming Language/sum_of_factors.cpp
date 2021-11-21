#include <iostream>

size_t s_to_size (const char* s) {
      size_t x = 0;

      while (s[0] != '\0') {
            x *= 10;
            x += (s[0] - '0');

            ++s;
      }

      return x;
}

bool check_eq (size_t* count, size_t size) {
      for (size_t i = 0; i < size; i++) {
            for (size_t j = i+1; j < size; j++)
                  if (count[i] == count[j])
                        return false;
      }

      return true;
}

long long sof (size_t size, size_t* sizes, long long** arrays) {
      
      size_t* count = new size_t [size];
      for (size_t i = 0; i < size; ++i)
            count[i] = 0;

      long long result = 0;

      while (true) {
            if (check_eq (count, size)) {
                  long long temp_res = 1;
                  for (size_t i = 0; i < size; i++) {
                        temp_res *= arrays[i][count[i]];
                  }
                  result += temp_res;
            }

            size_t x = 0;
            while (x < size) {
                  if (count[x] < sizes[x]-1) {
                        ++count[x];
                        break;
                  }
                  count[x] = 0;
                  ++x;
            }

            if (x == size) {
                  delete [] count;

                  return result;
            }
      }
}

int main (int argc, char** argv) {

      size_t* sizes = new size_t [argc-1];
      for (int i = 0; i < argc-1; i++)
            sizes[i] = s_to_size (argv[i+1]);

      long long** arrays = new long long* [argc-1];

      for (int i = 0; i < argc-1; ++i){

            arrays[i] = new long long [sizes[i]];
            
            for (size_t j = 0; j < sizes[i]; ++j)
                  std::cin >> arrays[i][j];
      }

      std::cout << sof (argc-1, sizes, arrays) << '\n';

      for (int i = 0; i < argc-1; ++i) {
            delete [] arrays[i];
      }

      delete [] arrays;
      delete [] sizes;
}
