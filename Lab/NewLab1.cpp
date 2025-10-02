#include <iostream>
#include <cmath>

int main() {
	setlocale(LC_ALL, "RU");
	int unsigned a, b, h;
	std::cout << "Значение высоты h:";
	std::cin >> h;
	if (h > 255) {
		std::cout << "Ошибка, слишком большое значение";
	}
	else {
		std::cout << "Значение нижнего основания a:";
		std::cin >> a;
		std::cout << "Значение верхнего основания b:";
		std::cin >> b;
	}
	if (a > 1000 or b > 1000) {
		std::cout << "Ошибка, слишком большое значение";
	}
	else {
		int s = ((a + b) / 2) * h;
		std::cout << "Площадь трапеции:" << s << std::endl;
		std::cout << "int,занимает " <<  sizeof(int)*8 << " бит" << std::endl;
		std::cout << "min: " << pow(2,sizeof(int)*8) << "\n"
			<< "max: " << pow(2, sizeof(int) * 8-1);
	}
}

