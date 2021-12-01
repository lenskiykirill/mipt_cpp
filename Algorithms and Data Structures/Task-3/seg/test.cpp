#include <iostream>

int main () {
      int* array = new int [10];
      for (int i=0; i < 10; ++i) {
            array[i] = i;
      }

      size_t size = 10;
      delete [] array;
      
      return 0;
}
