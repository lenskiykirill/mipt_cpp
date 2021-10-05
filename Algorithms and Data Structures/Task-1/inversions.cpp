#include <iostream>
#include <fstream>

unsigned long long merge (int *first_arr, unsigned int first_size, int *second_arr, unsigned int second_size, int *swap_arr) {
	
	unsigned long long invers_number = 0;

	unsigned int i = 0, j = 0;

/*---------------------------------------/
	std::cout << "Merging arrays:\n1: [";
	for (int k=0; k < first_size-1; k++)
		std::cout << first_arr[k] << ", ";
	std::cout << first_arr[first_size-1] << "]\n2: [";

	for (int k=0; k < second_size-1; k++)
		std::cout << second_arr[k] << ", ";
	std::cout << second_arr[second_size-1] << "]\n";

/---------------------------------------*/


	while ( (i < first_size) && (j < second_size) ) {
		if ( first_arr[i] < second_arr[j] ) {
			swap_arr[i+j] = first_arr[i];
			invers_number += j;

			++i;
		} else {
			swap_arr[i+j] = second_arr[j];
			++j;
		}
	}
	
	invers_number += (first_size - i)*second_size;

	while ( i < first_size ) {
		swap_arr[i+second_size] = first_arr[i];
		++i;
	}

	while ( j < second_size ) {
		swap_arr[j+first_size] = second_arr[j];
		++j;
	}


/*--------------------------------------/
	std::cout << "Number of inversions: " << invers_number << "\n";
	std::cout << "3: [";
	for (int i=0; i < first_size+second_size-1; i++)
		std::cout << swap_arr[i] << ", ";
	std::cout << swap_arr[first_size+second_size-1] << "]\n\n";
/--------------------------------------*/
	return invers_number;
}

unsigned long long get_inversion_number (int *arr, unsigned int size, int *swap_arr) {
	
	if (size < 2) {
		return 0;
	}

	int *first_arr = arr;
	unsigned int first_size = size/2;

	int *second_arr = arr + first_size;
	unsigned int second_size = size - first_size;

	unsigned long long invers_number = 0;

	invers_number += get_inversion_number (first_arr, first_size, swap_arr);
	invers_number += get_inversion_number (second_arr, second_size, swap_arr);

	invers_number += merge (first_arr, first_size, second_arr, second_size, swap_arr);

	for (int i=0; i < size; i++) {
		arr[i] = swap_arr[i];
	}

	return invers_number;
}

int main () {
	std::ifstream input;
	input.open("inverse.in");
	unsigned int N;

	input >> N;
	int *A = new int[N];
	int *swap = new int[N];

	for (int i=0; i < N; i++) {
		input >> A[i];
	}

	input.close();

	unsigned long long inversions = get_inversion_number (A, N, swap);

	std::ofstream output;
	output.open("inverse.out");
	
	output << inversions << '\n';

	output.close();

	delete A;
	delete swap;

	return 0;
}
