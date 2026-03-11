
#include "car.h"
#include <algorithm>
#include <random>
#include <regex>
#include <iostream>

// Защищенный метод для генерации случайного госномера
std::string Car::generateRandomLicensePlate() const {
    static const char letters[] = "ABCEHKMOPTXY";
    static const char digits[] = "0123456789";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> letterDist(0, 11);
    static std::uniform_int_distribution<> digitDist(0, 9);

    std::string plate;
    plate += letters[letterDist(gen)];
    plate += digits[digitDist(gen)];
    plate += digits[digitDist(gen)];
    plate += digits[digitDist(gen)];
    plate += letters[letterDist(gen)];
    plate += letters[letterDist(gen)];

    return plate;
}

// Защищенный метод проверки формата госномера
bool Car::isValidLicensePlate(const std::string& plate) const {
    std::regex pattern(R"([ABCEHKMOPTXY]\d{3}[ABCEHKMOPTXY]{2})");
    return std::regex_match(plate, pattern);
}

// Конструктор по умолчанию
Car::Car() : brand("Неизвестно"), model("Неизвестно"), licensePlate("A000AA") {
    std::cout << "Автомобиль, созданный по умолчанию" << std::endl;
}

// Конструктор полного заполнения
Car::Car(const std::string& br, const std::string& mdl,
    const std::string& plate, const std::vector<std::string>& items)
    : brand(br), model(mdl), trunkItems(items) {

    if (!isValidLicensePlate(plate)) {
        std::cout << "Внимание: Неверный формат номерного знака.Настройки по умолчанию" << std::endl;
        licensePlate = "A000AA";
    }
    else {
        licensePlate = plate;
    }

    std::cout << "Автомобиль, созданный с помощью full constructor" << std::endl;
}

// Конструктор копирования
Car::Car(const Car& other)
    : brand(other.brand), model(other.model),
    licensePlate(other.licensePlate), trunkItems(other.trunkItems) {
    std::cout << "Автомобиль, созданный с помощью copy constructor" << std::endl;
}

// Деструктор
Car::~Car() {
    std::cout << "Функция вызывает машину с гос.номером: "
        << licensePlate << " Очищены " << trunkItems.size() << " вещи " << std::endl;
    trunkItems.clear();
    std::cout << " Вектор очищен." << std::endl;
}

// Оператор присваивания
Car& Car::operator=(const Car& other) {
    std::cout << "Вызывается оператор присваивания" << std::endl;
    if (this != &other) {
        brand = other.brand;
        model = other.model;
        licensePlate = other.licensePlate;
        trunkItems = other.trunkItems;
    }
    return *this;
}

// Геттеры
std::string Car::getBrand() const { return brand; }
std::string Car::getModel() const { return model; }
std::string Car::getLicensePlate() const { return licensePlate; }
std::vector<std::string> Car::getTrunkItems() const { return trunkItems; }

// Сеттер для госномера с проверкой
bool Car::setLicensePlate(const std::string& plate) {
    if (isValidLicensePlate(plate)) {
        licensePlate = plate;
        return true;
    }
    std::cout << "Ошибка: Неверный формат номерного знака. Должно быть похоже на A555AA" << std::endl;
    return false;
}

//Вывод информации
void Car::displayInfo() const {
    std::cout << "=== Информация о машине ===" << std::endl;
    std::cout << "Фирма: " << brand << std::endl;
    std::cout << "Модел: " << model << std::endl;
    std::cout << "ГосНомер: " << licensePlate << std::endl;
    std::cout << "Вещи из багажника " << trunkItems.size();
    for (size_t i = 0; i < trunkItems.size(); ++i) {
        std::cout << trunkItems[i];
        if (i < trunkItems.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    std::cout << "=======================" << std::endl;
}

// Оператор+
Car Car::operator+(const Car& other) const {
    std::vector<std::string> newItems = trunkItems;

    // Добавляем элементы из other, учитывая повторы рядом
    for (const auto& item : other.trunkItems) {
        newItems.push_back(item);
    }

    // Выбираем марку
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    std::string newBrand = (dis(gen) == 0) ? brand : other.brand;

    // Генерируем новый госномер
    std::string newPlate;
    do {
        newPlate = generateRandomLicensePlate();
    } while (newPlate == licensePlate || newPlate == other.licensePlate);

    return Car(newBrand, model, newPlate, newItems);
}

// Оператор -
Car Car::operator-(const Car& other) const {
    std::vector<std::string> uniqueItems;

    // Собираем все уникальные элементы из обоих багажников
    std::vector<std::string> allItems = trunkItems;
    allItems.insert(allItems.end(), other.trunkItems.begin(), other.trunkItems.end());

    std::sort(allItems.begin(), allItems.end());
    auto last = std::unique(allItems.begin(), allItems.end());
    allItems.erase(last, allItems.end());

    uniqueItems = allItems;

    // Выбираем марку
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    std::string newBrand = (dis(gen) == 0) ? brand : other.brand;

    // Генерируем новый госномер
    std::string newPlate;
    do {
        newPlate = generateRandomLicensePlate();
    } while (newPlate == licensePlate || newPlate == other.licensePlate);

    return Car(newBrand, model, newPlate, uniqueItems);
}

// Оператор / 
Car Car::operator/(const Car& other) const {
    std::vector<std::string> commonItems;

    // Находим общие элементы
    for (const auto& item : trunkItems) {
        if (std::find(other.trunkItems.begin(), other.trunkItems.end(), item) != other.trunkItems.end()) {
            commonItems.push_back(item);
        }
    }

    // Удаляем дубликаты из общих элементов
    std::sort(commonItems.begin(), commonItems.end());
    auto last = std::unique(commonItems.begin(), commonItems.end());
    commonItems.erase(last, commonItems.end());

    // Выбираем марку 
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    std::string newBrand = (dis(gen) == 0) ? brand : other.brand;

    // Генерируем новый госномер
    std::string newPlate;
    do {
        newPlate = generateRandomLicensePlate();
    } while (newPlate == licensePlate || newPlate == other.licensePlate);

    return Car(newBrand, model, newPlate, commonItems);
}