// Person.h
#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>

class Person
{
public:
    Person();
    Person(const QString &name, const QString &surname, int height, float weight);

    QString name() const;
    QString surname() const;
    int height() const;
    float weight() const;

    void setName(const QString &name);
    void setSurname(const QString &surname);
    void setHeight(int height);
    void setWeight(float weight);

    bool isValid() const; // проверка: все поля заполнены, рост >0, вес >0
    QString errorMessage() const;

    QJsonObject toJson() const;
    static Person fromJson(const QJsonObject &obj);

private:
    QString m_name;
    QString m_surname;
    int m_height;
    float m_weight;
};

#endif // PERSON_H