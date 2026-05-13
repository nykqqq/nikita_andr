// Person.cpp
#include "Person.h"
#include <QRegularExpression>

Person::Person() : m_height(0), m_weight(0.0f) {}

Person::Person(const QString &name, const QString &surname, int height, float weight)
    : m_name(name), m_surname(surname), m_height(height), m_weight(weight) {}

QString Person::name() const { return m_name; }
QString Person::surname() const { return m_surname; }
int Person::height() const { return m_height; }
float Person::weight() const { return m_weight; }

void Person::setName(const QString &name) { m_name = name; }
void Person::setSurname(const QString &surname) { m_surname = surname; }
void Person::setHeight(int height) { m_height = height; }
void Person::setWeight(float weight) { m_weight = weight; }

bool Person::isValid() const {
    return !m_name.isEmpty() &&
           !m_surname.isEmpty() &&
           m_height > 0 && m_height < 300 &&
           m_weight > 0.0f && m_weight < 500.0f;
}

QString Person::errorMessage() const {
    if (m_name.isEmpty()) return "Имя не заполнено";
    if (m_surname.isEmpty()) return "Фамилия не заполнена";
    if (m_height <= 0) return "Рост должен быть > 0";
    if (m_weight <= 0.0f) return "Вес должен быть > 0";
    return "";
}

QJsonObject Person::toJson() const {
    QJsonObject obj;
    obj["name"] = m_name;
    obj["surname"] = m_surname;
    obj["height"] = m_height;
    obj["weight"] = m_weight;
    return obj;
}

Person Person::fromJson(const QJsonObject &obj) {
    Person p;
    p.setName(obj["name"].toString());
    p.setSurname(obj["surname"].toString());
    p.setHeight(obj["height"].toInt());
    p.setWeight(obj["weight"].toDouble());
    return p;
}