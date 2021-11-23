#include <iostream>
#include <algorithm>

struct dot {
      long long x;
      long long y;

      bool operator< (dot& other) {
            if (y == other.y)
                  return x < other.x;
            return y < other.y;
      }
};

long long f (long long x) { return x & (x+1); }
long long g (long long y) { return y | (y+1); }

class inner_fenwick {
      public:
            inner_fenwick ();
            ~inner_fenwick ();

            long long get (long long x);
            void change (long long x, long long z);
            void construct (long long* array, long long size);
      private:

            long long size;
            long long* x_array;
            long long* fenwick_array;
};

inner_fenwick::inner_fenwick () {}

void inner_fenwick::construct (long long* array, long long size) {
      
      this->size = size;
      x_array = new long long [size];
      fenwick_array = new long long [size];

      for (long long i = 0; i < size; ++i) {
            x_array[i] = array[i];
            fenwick_array[i] = 0;
      }
}

inner_fenwick::~inner_fenwick () {
      delete [] x_array;
      delete [] fenwick_array;
}

long long inner_fenwick::get (long long x) {
      long long res = 0;
      long long left = 0;
      long long right = size;

      if (x < x_array[0])
            return 0;

      while (right-left > 1) {
            long long middle = (left+right) / 2;
            
            if (x_array[middle] <= x)
                  left = middle;
            else
                  right = middle;
      }

      for (long long i = left; i >= 0; i = f(i)-1) {
            res += fenwick_array[i];
      }

      return res;
}

void inner_fenwick::change (long long x, long long z) {
      long long left = 0;
      long long right = size;

      while (right-left > 1) {
            long long middle = (left+right) / 2;

            if (x_array[middle] <= x)
                  left = middle;
            else
                  right = middle;
      }

      for (long long i = left; i < size; i = g(i)) {
            fenwick_array[i] += z;
      }
}

class outer_fenwick {
      public:
            outer_fenwick (dot* array, long long size);
            ~outer_fenwick ();

            long long get (long long x, long long y);
            void change (long long x, long long y, long long z);
      private:

            long long size;
            long long* y_array;

            inner_fenwick* fenwick_array;
};

outer_fenwick::outer_fenwick (dot* dot_array, long long size) {
      this->size = 1;
      
      dot* array = new dot[size];
      for (long long i = 0; i < size; ++i)
            array[i] = dot_array[i];

      std::sort (array, array+size);

      for (long long i = 1; i < size; ++i) {
            if (array[i-1].y != array[i].y)
                  ++(this->size);
      }

      y_array = new long long [this->size];
      fenwick_array = new inner_fenwick [this->size];

      long long* tmp_array = new long long [size];

      long long k = 0;
      long long* k_to_y = new long long [this->size];
      k_to_y[0] = 0;

      // need to make an array with different y's, each contains x's for y's from f(i) to i
      
      for (long long i = 1; i < size; ++i) {
            if (array[i].y != array[i-1].y) {
                  long long prev_k = f(k);
                  long long prev_y = k_to_y [prev_k];
                  
                  for (long long j = prev_y; j < i; ++j) {
                        tmp_array[j-prev_y] = array[j].x;
                  }

                  std::sort (tmp_array, tmp_array+(i-prev_y));

                  fenwick_array[k].construct (tmp_array, i-prev_y);

                  y_array[k] = array[i-1].y;
                  k_to_y [k++] = i;
            }
      }

      long long prev_k = f(k);
      
      long long prev_y = k_to_y [prev_k];

      for (long long j = prev_y; j < size; ++j)
            tmp_array[j-prev_y] = array[j].x;

      std::sort (tmp_array, tmp_array+(size-prev_y));

      fenwick_array[k].construct (tmp_array, size-prev_y);

      y_array[k] = array[size-1].y;

      delete [] k_to_y;
      delete [] tmp_array;
      delete [] array;
}

outer_fenwick::~outer_fenwick () {
      delete [] y_array;
      delete [] fenwick_array;
}

long long outer_fenwick::get (long long x, long long y) {
      long long left = 0;
      long long right = size;
      
      if (y < y_array[0])
            return 0;

      while (right-left > 1) {
            long long middle = (left+right) / 2;

            if (y_array[middle] <= y)
                  left = middle;
            else
                  right = middle;
      }

      long long res = 0;

      for (long long i = left; i >= 0; i = f(i)-1) {
            res += fenwick_array[i].get (x);
      }

      return res;
}

void outer_fenwick::change (long long x, long long y, long long z) {

      long long left = 0;
      long long right = size;

      while (right-left > 1) {
            long long middle = (left+right) / 2;

            if (y_array[middle] <= y)
                  left = middle;
            else
                  right =  middle;
      }

      for (long long i = left; i < size; i = g(i)) {
            fenwick_array[i].change (x, z);
      }
}

int main () {
      long long N;
      std::cin >> N;

      dot* dot_array = new dot[N];

      long long* weight = new long long[N];

      for (long long i = 0; i < N; ++i) {
            std::cin >> dot_array[i].x >> dot_array[i].y;
            std::cin >> weight[i];
      }

      outer_fenwick F (dot_array, N);

      for (long long i = 0; i < N; ++i) {
            F.change (dot_array[i].x, dot_array[i].y, weight[i]);
      }

      std::cin >> N;

      for (long long i = 0; i < N; ++i) {
            char c;
            std::cin >> c;
            long long x, y, z, d;

            switch (c) {
                  case 'c':
                        std::cin >> y >> z;
                        d = z - weight[y-1];
                        weight[y-1] = z;

                        x = dot_array[y-1].x;
                        y = dot_array[y-1].y;

                        F.change (x, y, d);
                        break;

                  case 'g':
                        std::cin >> x >> y;
                        z = F.get (x, y);
                        std::cout << z << '\n';
                        break;

                  default:
                        std::cout << "Unknown input '" << c << "'...\n";

                        delete [] dot_array;
                        delete [] weight;

                        return 0;
            }
      }

      delete [] dot_array;
      delete [] weight;

      return 0;
}
