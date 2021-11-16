#include <iostream>
#include <fstream>

class night_sky {
      public:
            night_sky (int x_max, int y_max, int z_max);
            ~night_sky ();
            
            void modify (int x1, int y1, int z1, long long value);
            long long get_sum (int x1, int y1, int z1);
            long long get_sum (int x1, int y1, int z1, int x2, int y2, int z2);

      private:
            int x_max;
            int y_max;
            int z_max;
            
            long long*** array;

            static int f(int x) { return x & (x+1); }
            static int g(int y) { return y | (y+1); }
};

night_sky::night_sky (int x_max, int y_max, int z_max) {
      
      this->x_max = x_max;
      this->y_max = y_max;
      this->z_max = z_max;
      
      array = new long long** [x_max];

      for (size_t i = 0; i < x_max; ++i) {
            array[i] = new long long* [y_max];
            
            for (size_t j = 0; j < y_max; ++j) {
                  array[i][j] = new long long [z_max];

                  for (size_t k = 0; k < z_max; ++k)
                        array[i][j][k] = 0;
            }
      }
}

night_sky::~night_sky () {
      for (size_t i = 0; i < x_max; ++i) {
            for (size_t j = 0; j < y_max; j++)
                  delete [] array[i][j];
            delete [] array[i];
      }

      delete [] array;
}

long long night_sky::get_sum (int x1, int y1, int z1) {
      
      long long res = 0;

      if ((x1 < 0) || (y1 < 0) || (z1 < 0))
            return 0;

      for (int x = x1; x >= 0; x = f(x)-1) {
            for (int y = y1; y >= 0; y = f(y)-1) {
                  for (int z = z1; z >= 0; z = f(z)-1) {

                        res += array[x][y][z];
                  }
            }
      }
      
      return res;
}

void night_sky::modify (int x1, int y1, int z1, long long value) {
      for (int x = x1; x < x_max; x = g(x)) {
            for (int y = y1; y < y_max; y = g(y)) {
                  for (int z = z1; z < z_max; z = g(z)) {
                        array[x][y][z] += value;
                  }
            }
      }
}

long long night_sky::get_sum (int x1, int y1, int z1, int x2, int y2, int z2) {
      long long res = get_sum (x2, y2, z2) - get_sum (x2, y2, z1);

      res -= (get_sum (x1, y2, z2) - get_sum (x1, y2, z1));
      res -= (get_sum (x2, y1, z2) - get_sum (x2, y1, z1));
      res += (get_sum (x1, y1, z2) - get_sum (x1, y1, z1));

      return res;
}

int main () {
      int n;

      std::cin >> n;

      night_sky ns(n, n, n);

      int request, x1, x2, y1, y2, z1, z2;
      long long value;

      while (true) {
            std::cin >> request;
            switch (request) {
                  case 1:
                        std::cin >> x1 >> y1 >> z1 >> value;
                        ns.modify (x1, y1, z1, value);
                        break;
                  case 2:
                        std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
                        value = ns.get_sum (x1-1, y1-1, z1-1, x2, y2, z2);
                        std::cout << value << '\n';
                        break;

                  case 3:
                        return 0;
            }
      }

      // Let me hope this task passes
}
