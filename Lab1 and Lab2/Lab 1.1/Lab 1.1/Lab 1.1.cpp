#include <iostream>
#include <string>
#include <regex>

class Car {
private:
    std::string brand;        // марка
    std::string model;        // модель
    std::string bodyNumber;   // номер кузова
    std::string licensePlate; // гос. номер
    int mileage;              // пробег

    // Вспомогательный метод для валидации гос. номера (формат: буква-3 цифры-2 буквы)
    bool isValidLicensePlate(const std::string& plate) const {
        std::regex pattern(R"([А-ЯA-Z]\d{3}[А-ЯA-Z]{2})");
        return std::regex_match(plate, pattern);
    }

    // Вспомогательный метод для валидации номера кузова (17 символов, буквы и цифры)
    bool isValidBodyNumber(const std::string& bodyNum) const {
        std::regex pattern(R"([A-HJ-NPR-Z0-9]{17})");
        return std::regex_match(bodyNum, pattern);
    }

public:
    // Конструктор по умолчанию
    Car() : brand("Неизвестно"), model("Неизвестно"),
        bodyNumber("Неизвестно"), licensePlate("Неизвестно"), mileage(0) {
        std::cout << "Конструктор по умолчанию вызван" << std::endl;
    }

    // Конструктор полного заполнения
    Car(const std::string& brand, const std::string& model,
        const std::string& bodyNumber, const std::string& licensePlate, int mileage)
        : brand(brand), model(model), mileage(mileage) {

        if (isValidBodyNumber(bodyNumber)) {
            this->bodyNumber = bodyNumber;
        }
        else {
            this->bodyNumber = "Invalid";
        }

        if (isValidLicensePlate(licensePlate)) {
            this->licensePlate = licensePlate;
        }
        else {
            this->licensePlate = "Invalid";
        }

        std::cout << "Конструктор полного заполнения" << std::endl;
    }

    // Конструктор копирования
    Car(const Car& other)
        : brand(other.brand), model(other.model),
        bodyNumber(other.bodyNumber), licensePlate(other.licensePlate),
        mileage(other.mileage) {
        std::cout << "Конструктор копирования" << std::endl;
    }

    // Деструктор
    ~Car() {
        std::cout << "Деструктор вызван для автомобиля " << brand << " " << model << std::endl;
    }

    // Геттеры для всех полей
    std::string getBrand() const {
        return brand;
    }

    std::string getModel() const {
        return model;
    }

    std::string getBodyNumber() const {
        return bodyNumber;
    }

    std::string getLicensePlate() const {
        return licensePlate;
    }

    int getMileage() const {
        return mileage;
    }

    // Сеттер для гос. номера с проверкой
    void setLicensePlate(const std::string& plate) {
        if (isValidLicensePlate(plate)) {
            licensePlate = plate;
            std::cout << "Гос. номер успешно обновлен" << std::endl;
        }
        else {
            std::cout << "Ошибка: неверный формат гос. номера. Ожидаемый формат: А123ББ" << std::endl;
        }
    }

    // Сеттер для номера кузова с проверкой
    void setBodyNumber(const std::string& bodyNum) {
        if (isValidBodyNumber(bodyNum)) {
            bodyNumber = bodyNum;
            std::cout << "Номер кузова успешно обновлен" << std::endl;
        }
        else {
            std::cout << "Ошибка: неверный формат номера кузова. Должно быть 17 символов (буквы и цифры)" << std::endl;
        }
    }

    // Метод для вывода всей информации
    void displayInfo() const {
        std::cout << "\n=== Информация об автомобиле ===" << std::endl;
        std::cout << "Марка: " << brand << std::endl;
        std::cout << "Модель: " << model << std::endl;
        std::cout << "Номер кузова: " << bodyNumber << std::endl;
        std::cout << "Гос. номер: " << licensePlate << std::endl;
        std::cout << "Пробег: " << mileage << " км" << std::endl;
        std::cout << "================================\n" << std::endl;
    }

    // Метод для скручивания пробега на X
    void rollbackMileage(int x) {
        if (x > 0) {
            if (mileage >= x) {
                mileage -= x;
                std::cout << "Пробег уменьшен на " << x << " км."
                    "Текущий пробег : " << mileage << " км" << std::endl;
            }
            else {
                std::cout << "Ошибка: пробег не может стать отрицательным!" << std::endl;
            }
        }
        else {
            std::cout << "Ошибка: значение для уменьшения должно быть положительным!" << std::endl;
        }
    }
};

// Пример использования
int main() {
    setlocale(LC_ALL, "RU");
    std::cout << "=== Тест Car ===" << std::endl;

    // Тест конструктора по умолчанию
    std::cout << "\n1. Создание объекта через конструктор по умолчанию:" << std::endl;
    Car car1;
    car1.displayInfo();

    // Тест конструктора полного заполнения
    std::cout << "\n2. Создание объекта через конструктор полного заполнения:" << std::endl;
    Car car2("Toyota", "Camry", "1HGCM82633A123456", "А123БВ", 150000);
    car2.displayInfo();

    // Тест конструктора копирования
    std::cout << "\n3. Создание объекта через конструктор копирования:" << std::endl;
    Car car3(car2);
    car3.displayInfo();

    // Тест сеттеров
    std::cout << "\n4. Тестирование сеттеров:" << std::endl;

    // Корректный гос. номер
    std::cout << "Попытка установить корректный гос. номер 'В456СД':" << std::endl;
    car1.setLicensePlate("В456СД");

    // Некорректный гос. номер
    std::cout << "\nПопытка установить некорректный гос. номер '12345':" << std::endl;
    car1.setLicensePlate("12345");

    // Корректный номер кузова
    std::cout << "\nПопытка установить корректный номер кузова '1HGBH41JXMN109186':" << std::endl;
    car1.setBodyNumber("1HGBH41JXMN109186");

    // Некорректный номер кузова
    std::cout << "\nПопытка установить некорректный номер кузова '123':" << std::endl;
    car1.setBodyNumber("123");

    car1.displayInfo();

    // Тест скручивания пробега
    std::cout << "\n5. Тестирование скручивания пробега:" << std::endl;
    std::cout << "Текущий пробег car2: " << car2.getMileage() << " км" << std::endl;

    std::cout << "Попытка скрутить пробег на 10000 км:" << std::endl;
    car2.rollbackMileage(10000);

    std::cout << "Попытка скрутить пробег на 50000 км (больше текущего):" << std::endl;
    car2.rollbackMileage(50000);

    std::cout << "Попытка скрутить пробег на отрицательное значение:" << std::endl;
    car2.rollbackMileage(-1000);

    

    return 0;
}