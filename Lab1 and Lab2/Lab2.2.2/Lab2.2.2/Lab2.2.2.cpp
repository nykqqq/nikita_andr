
#include "car.h"
#include <iostream>

int main() {
    setlocale(LC_ALL, "RU");
    std::cout << "=== Тестирование конструкторов и деструкторов ===" << std::endl;

    // Создание объектов с разными конструкторами
    Car defaultCar;
    defaultCar.displayInfo();

    std::cout << "\n";

    // Создание первой машины
    std::vector<std::string> items1 = { "a", "b", "v" };
    Car car1("A", "ModelX", "A777AA", items1);
    car1.displayInfo();

    std::cout << "\n";

    // Создание второй машины
    std::vector<std::string> items2 = { "b", "t", "k" };
    Car car2("B", "ModelY", "B77766", items2);
    car2.displayInfo();

    std::cout << "\n=== Тестирование операторов ===" << std::endl;

    // Тест оператора +
    std::cout << "Оператор +:" << std::endl;
    Car resultPlus = car1 + car2;
    resultPlus.displayInfo();

    std::cout << "\n";

    // Тест оператора -
    std::cout << "Оператор -:" << std::endl;
    Car resultMinus = car1 - car2;
    resultMinus.displayInfo();

    std::cout << "\n";

    // Тест оператора /
    std::cout << "Оператор /:" << std::endl;
    Car resultDivide = car1 / car2;
    resultDivide.displayInfo();

    std::cout << "\n=== Тестирование оператора присваивания ===" << std::endl;
    Car car3;
    car3 = car1;
    car3.displayInfo();

    std::cout << "\n=== Тестирование сеттера с проверкой ===" << std::endl;
    Car testCar;
    testCar.setLicensePlate("INVALID");  // Неправильный формат
    testCar.setLicensePlate("A123BC");   // Правильный формат
    testCar.displayInfo();


    return 0;
}