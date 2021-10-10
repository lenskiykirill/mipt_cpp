#include "new_heap.h"
#include <iostream>

bool is_less (int a, int b) {
	return a < b;
}

int main() {
	char cmd = '\0';
	int value;

	dst::heap<int> heap(is_less);

	std::cout << "\033[1;31m.:Amazing heap v0.01:.\033[0m\n";
	std::cout << "\nAvailable commands:\n\n";
	std::cout << "1) \033[0;32mi\033[0m 12345 : insert number '12345'\n";
	std::cout << "2) \033[0;32me\033[0m       : extract minimal number from heap\n";
	std::cout << "3) \033[0;32ms\033[0m       : print heap size\n";
	std::cout << "4) \033[0;32mc\033[0m       : clear heap (unavailable for now)\n";
	std::cout << "5) \033[0;32mt\033[0m       : get top (minimal) element\n";
	std::cout << "6) \033[1;31mx\033[0m       : exit\n";
	std::cout << "\n";

	while ( true ) {
		std::cout << "> ";
		std::cin >> cmd;

		switch (cmd) {
			case 'i':
				std::cin >> value;
				heap.insert ( value );
				std::cout << "Ststus: 'ok'\n";
				break;
			case 'e':
				if ( heap.size() > 0 ) {
					std::cout << "\033[1;32m" << heap.extract() << "\033[0m\n";
				} else {
					std::cout << "Heap size is \033[1;31m0\033[0m!\n";
				}
				break;
					
			case 's':
				std::cout << heap.size() << '\n';
				break;
			case 't':
				std::cout << heap.top() << '\n';
				break;
			case 'x':
				std::cout << "Goodbye...\n";
				return 0;
		}
	}
	
	return 0;
}
