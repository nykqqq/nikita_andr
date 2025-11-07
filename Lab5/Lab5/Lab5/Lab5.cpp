

#include <iostream>
#include <limits>
#include <cmath>


int sum = 0;
int sum1 = 0;
int SumNumber(int a, int b) {
    std::cout << "Использутся функция вычисления суммы цифр двух чисел" << std::endl;
    while (abs(a) > 0) {
        int d = abs(a) % 10;
        sum += d ;
        a /= 10;
        while (abs(b) > 0) {
            int k = abs(b) % 10;
            sum1 += k;
            b /= 10;
        }
    }
    return sum + sum1;

}
int Max3(int a, int b, int c) {
    std::cout << "Используется функция нахождения максимального из чисел" << std::endl;
    int max3 = INT_MIN;
    if (a > max3 && a > b && a > c) {
        max3 = a;
    }
    else if (b > max3 && b > a && b > c) {
        max3 = b;
    }
    else {
        max3 = c;
    }
    return max3;

   
}
void Lab1(double a, double b, unsigned int c) {
    std::cout << "Используется функция нахождения площади трапеции и нахождение памяти в битах каждого типа данного" << std::endl;
    double s = ((a + b) / 2) * c;
    std::cout << "Площадь трапеции:" << s << std::endl;
    std::cout << "int unsigned,занимает " << sizeof(int) * 8 << " бит" << std::endl;
    std::cout << "min: 0" << "\n"
        << "max: " << UINT_MAX << std::endl;
    std::cout << "double занимает " << sizeof(double) * 8 << " бит" << std::endl;
    std::cout << "min: " << std::numeric_limits<double>::min() << "\n"
        << "max: " << std::numeric_limits<double>::max() << std::endl;
}
int main()

{
    int x, z, q;
    unsigned int height;
    double footing1, footing2;
    setlocale(LC_ALL, "RU");
    std::cout << "Первое число: "; std::cin >> x;
    std::cout << "Второе число: "; std::cin >> z;
    std::cout << SumNumber(x, z) << std::endl;
    std::cout << "Первое число: "; std::cin >> x;
    std::cout << "Второе число: "; std::cin >> z;
    std::cout << "Третье число: "; std::cin >> q;
    std::cout << Max3(x, z, q) << std::endl;
    std::cout << "Высота "; std::cin >> height;
    std::cout << "Первое основание "; std::cin >> footing1;
    std::cout << "Второе основание"; std::cin >> footing2;
    Lab1(footing1, footing2, height);





}
