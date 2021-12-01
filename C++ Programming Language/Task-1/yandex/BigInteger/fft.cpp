#include <vector>
#include <iostream>

using namespace std;

      unsigned long long reverse (unsigned long long a, unsigned long long p) {
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


const int mod = 7340033;
const int root = 5;
const int root_1 = 4404020;
const int root_pw = 1<<20;

void fft (vector<int> & a, bool invert) {
	int n = (int) a.size();

	for (int i=1, j=0; i<n; ++i) {
		int bit = n >> 1;
		for (; j>=bit; bit>>=1)
			j -= bit;
		j += bit;
		if (i < j)
			swap (a[i], a[j]);
	}

	for (int len=2; len<=n; len<<=1) {
		int wlen = invert ? root_1 : root;
		for (int i=len; i<root_pw; i<<=1)
			wlen = int (wlen * 1ll * wlen % mod);
		for (int i=0; i<n; i+=len) {
			int w = 1;
			for (int j=0; j<len/2; ++j) {
				int u = a[i+j],  v = int (a[i+j+len/2] * 1ll * w % mod);
				a[i+j] = u+v < mod ? u+v : u+v-mod;
				a[i+j+len/2] = u-v >= 0 ? u-v : u-v+mod;
				w = int (w * 1ll * wlen % mod);
			}
		}
	}
	if (invert) {
		int nrev = reverse (n, mod);
		for (int i=0; i<n; ++i)
			a[i] = int (a[i] * 1ll * nrev % mod);
	}
}

int main() {
      vector<int> a = {0, 1};
      vector<int> b = {0, 1};
      
      vector<int> fa (a.begin(), a.end()), fb (b.begin(), b.end());
      size_t n = 1;

      while (n < max (a.size(), b.size())) n <<= 1;
      n <<= 1;
      fa.resize(n), fb.resize(n);

/*      for (size_t i = a.size(); i < fa.size(); i++)
            fa[i] = 0;
      for (size_t i = b.size(); i < fb.size(); i++)
            fb[i] = 0;
*/
      fft (fa, false);
      fft (fb, false);

      for (size_t i=0; i < n; ++i)
            fa[i] = (fa[i]*1ll*fb[i]) % 7340033;;
      fft (fa, true);

      for (size_t i = 0; i < fa.size(); ++i)
            std::cout << fa[i] << ' ';
      std::cout << '\n';
}
