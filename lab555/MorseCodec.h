#ifndef MORSECODEC_H
#define MORSECODEC_H

#include <QString>
#include <QStringList>
#include <map>
#include <vector>

class MorseCodec {
public:
    MorseCodec();

    // П.1 Кодирование: Текст -> Морзе (*, -, /)
    QString encode(const QString& text);

    // П.3 Декодирование: Морзе -> Текст
    // Возвращает список возможных вариантов декодирования (если были ошибки)
    QStringList decode(const QString& morseCode);

    // П.2 Передача (Нарушение): Вставляет лишнюю паузу (/) внутри буквы
    // Ограничение: не более одной паузы на букву.
    QString applyNoviceError(const QString& morseCode);

    // П.4 Восстановление: Генерирует все возможные варианты текста для кода с ошибкой
    // (Используется внутри decode, но вынесено для наглядности)
    QStringList recoverCandidates(const QString& corruptedToken);

private:
    std::map<QChar, QString> m_rusToMorse; // Русская буква -> Код
    std::map<QString, QChar> m_morseToRus; // Код -> Русская буква

    // Вспомогательная функция для рекурсивного восстановления
    void backtrackRecover(const QString& token, int startIndex, QString currentLetter, QStringList& results);
};

#endif // MORSECODEC_H
