#include "Book.h"
#include <QFile>
#include <QTextStream>

Book::Book(const QString& author, const QString& title,
                   const QString& code, const QString& ratio,
                   const QString& type)
    : m_author(author), m_title(title), m_code(code),
    m_ratio(ratio), m_type(type)
{
}

bool Book::saveToFile(const QString& filename) const
{
    QFile file(filename);
    if (!file.open(QIODevice::Append | QIODevice::Text))
        return false;
    QTextStream out(&file);
    out << m_author << "," << m_title << "," << m_code << ","
        << m_ratio << "," << m_type << "\n";
    file.close();
    return true;
}
