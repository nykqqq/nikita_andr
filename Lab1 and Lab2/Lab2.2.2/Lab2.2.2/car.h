
#ifndef CAR_H
#define CAR_H

#include <iostream>
#include <string>
#include <vector>

class Car {
private:
    std::string brand;                 // ìàðêà
    std::string model;                  // ìîäåëü
    std::string licensePlate;           // ãîñ. íîìåð
    std::vector<std::string> trunkItems; // âåêòîð âåùåé â áàãàæíèêå

protected:
    // Ìåòîä äëÿ ãåíåðàöèè ñëó÷àéíîãî ãîñíîìåðà
    std::string generateRandomLicensePlate() const;

    // Ïðîâåðêà ôîðìàòà ãîñíîìåðà
    bool isValidLicensePlate(const std::string& plate) const;

public:
    // Êîíñòðóêòîð ïî óìîë÷àíèþ
    Car();

    // Êîíñòðóêòîð ïîëíîãî çàïîëíåíèÿ
    Car(const std::string& br, const std::string& mdl,
        const std::string& plate, const std::vector<std::string>& items);

    // Êîíñòðóêòîð êîïèðîâàíèÿ
    Car(const Car& other);

    // Äåñòðóêòîð
    ~Car();

    // Îïåðàòîð ïðèñâàèâàíèÿ
    Car& operator=(const Car& other);

    // Ãåòòåðû
    std::string getBrand() const;
    std::string getModel() const;
    std::string getLicensePlate() const;
    std::vector<std::string> getTrunkItems() const;

    // Ñåòòåð äëÿ ãîñíîìåðà
    bool setLicensePlate(const std::string& plate);

    // Ìåòîä äëÿ âûâîäà èíôîðìàöèè
    void displayInfo() const;

    // Îïåðàòîðû
    Car operator+(const Car& other) const;
    Car operator-(const Car& other) const;
    Car operator/(const Car& other) const;
};

#endif 
