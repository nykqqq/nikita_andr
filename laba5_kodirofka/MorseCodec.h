#ifndef MORSECODEC_H
#define MORSECODEC_H

#include <QString>
#include <QStringList>
#include <map>
#include <vector>

class MorseCodec {
public:
    MorseCodec();


    QString encode(const QString& text);


    QStringList decode(const QString& morseCode);


    QString applyNoviceError(const QString& morseCode);


    QStringList recoverCandidates(const QString& corruptedToken);

private:
    std::map<QChar, QString> m_rusToMorse;
    std::map<QString, QChar> m_morseToRus;


    void backtrackRecover(const QString& token, int startIndex, QString currentLetter, QStringList& results);
};

#endif
