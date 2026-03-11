// car.h
#ifndef CAR_H
#define CAR_H

#include <iostream>
#include <string>
#include <vector>

class Car {
private:
    std::string brand;                 // марка
    std::string model;                  // модель
    std::string licensePlate;           // гос. номер
    std::vector<std::string> trunkItems; // вектор вещей в багажнике

protected:
    // Метод для генерации случайного госномера
    std::string generateRandomLicensePlate() const;

    // Проверка формата госномера
    bool isValidLicensePlate(const std::string& plate) const;

public:
    // Конструктор по умолчанию
    Car();

    // Конструктор полного заполнения
    Car(const std::string& br, const std::string& mdl,
        const std::string& plate, const std::vector<std::string>& items);

    // Конструктор копирования
    Car(const Car& other);

    // Деструктор
    ~Car();

    // Оператор присваивания
    Car& operator=(const Car& other);

    // Геттеры
    std::string getBrand() const;
    std::string getModel() const;
    std::string getLicensePlate() const;
    std::vector<std::string> getTrunkItems() const;

    // Сеттер для госномера
    bool setLicensePlate(const std::string& plate);

    // Метод для вывода информации
    void displayInfo() const;

    // Операторы
    Car operator+(const Car& other) const;
    Car operator-(const Car& other) const;
    Car operator/(const Car& other) const;
};

#endif // CAR_H
