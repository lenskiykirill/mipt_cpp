#include <iostream>

unsigned int find_min_max ( int *a, int *b, unsigned int size ) {
	unsigned int left = 0;
	unsigned int right = size;
	unsigned int middle;
	
	int min_max1, min_max2;

	while ( right-left > 1 ) {
		middle = (right + left)/2;

		if ( a[middle] < b[middle] ) {
			left = middle;
		}
		else {
			right = middle;
		}
	}

	if ( right == size ) {
		return left;
	}

	min_max1 = std::max ( a[left], b[left] );
	min_max2 = std::max ( a[right], b[right] );

	if (  min_max1 < min_max2  ) {
		return left;
	}
	
	return right;
}

int main () {
	unsigned int n, m, l, q, a_number, b_number;
	std::cin >> n >> m >> l;

	int **A = new int*[n];
	int **B = new int*[m];

	for (int i=0; i < n; i++) {
		
		A[i] = new int[l];
		
		for (int j=0; j < l; j++) {
			scanf("%d",  A[i]+j);
		}
	}

	for (int i=0; i < m; i++) {

		B[i] = new int[l];
		
		for (int j=0; j < l; j++) {
			scanf("%d", B[i]+j);
		}
	}
	
	std::cin >> q;
	for (int i=0; i < q; i++) {
//		std::cout << i << " > ";
		scanf("%u %u", &a_number, &b_number);
		printf("%u\n", find_min_max ( A[a_number-1], B[b_number-1], l) + 1);
	}

	for (int i = 0; i < n; i++) {
		delete A[i];
	}

	delete A;

	for (int i=0; i < m; i++) {
		delete B[i];
	}

	delete B;

	return 0;
}

