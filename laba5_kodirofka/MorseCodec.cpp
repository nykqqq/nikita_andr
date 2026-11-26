#include "MorseCodec.h"
#include <QDebug>
#include <QRandomGenerator>

MorseCodec::MorseCodec() {
    m_rusToMorse[QChar(0x0410)] = "*-";     // А
    m_rusToMorse[QChar(0x0411)] = "-***";   // Б
    m_rusToMorse[QChar(0x0412)] = "*--";    // В
    m_rusToMorse[QChar(0x0413)] = "--*";    // Г
    m_rusToMorse[QChar(0x0414)] = "-**";    // Д
    m_rusToMorse[QChar(0x0415)] = "*";      // Е
    m_rusToMorse[QChar(0x0416)] = "***-";   // Ж
    m_rusToMorse[QChar(0x0417)] = "--**";   // З
    m_rusToMorse[QChar(0x0418)] = "**";     // И
    m_rusToMorse[QChar(0x0419)] = "*---";   // Й
    m_rusToMorse[QChar(0x041A)] = "-*-";    // К
    m_rusToMorse[QChar(0x041B)] = "*-**";   // Л
    m_rusToMorse[QChar(0x041C)] = "--";     // М
    m_rusToMorse[QChar(0x041D)] = "-*";     // Н
    m_rusToMorse[QChar(0x041E)] = "---";    // О
    m_rusToMorse[QChar(0x041F)] = "*--*";   // П
    m_rusToMorse[QChar(0x0420)] = "*-*";    // Р
    m_rusToMorse[QChar(0x0421)] = "***";    // С
    m_rusToMorse[QChar(0x0422)] = "-";      // Т
    m_rusToMorse[QChar(0x0423)] = "**-";    // У
    m_rusToMorse[QChar(0x0424)] = "**-*";   // Ф
    m_rusToMorse[QChar(0x0425)] = "****";   // Х
    m_rusToMorse[QChar(0x0426)] = "-*-*";   // Ц
    m_rusToMorse[QChar(0x0427)] = "---*";   // Ч
    m_rusToMorse[QChar(0x0428)] = "----";   // Ш
    m_rusToMorse[QChar(0x0429)] = "--*-";   // Щ
    m_rusToMorse[QChar(0x042A)] = "--*--";  // Ъ
    m_rusToMorse[QChar(0x042B)] = "-*--";   // Ы
    m_rusToMorse[QChar(0x042C)] = "-**-";   // Ь
    m_rusToMorse[QChar(0x042D)] = "**-**";  // Э
    m_rusToMorse[QChar(0x042E)] = "**--";   // Ю
    m_rusToMorse[QChar(0x042F)] = "*-*-";   // Я

    for (const auto& pair : m_rusToMorse) {
        m_morseToRus[pair.second] = pair.first;
    }
}

QString MorseCodec::encode(const QString& text) {
    QString result;
    QString upperText = text.toUpper();

    for (int i = 0; i < upperText.length(); ++i) {
        QChar ch = upperText[i];
        auto it = m_rusToMorse.find(ch);
        if (it != m_rusToMorse.end()) {
            if (!result.isEmpty()) result += " ";
            result += it->second;
        } else if (ch == ' ') {
            result += " / ";
        }
    }
    return result.trimmed();
}

QString MorseCodec::applyNoviceError(const QString& morseCode) {
    QStringList tokens = morseCode.split(' ', Qt::SkipEmptyParts);
    QStringList corruptedTokens;

    for (const QString& token : tokens) {
        if (token == "/") {
            corruptedTokens << token;
            continue;
        }

        if (token.length() > 1 && QRandomGenerator::global()->bounded(100) < 30) {
            int pos = QRandomGenerator::global()->bounded(1, token.length());
            QString corrupted = token.left(pos) + "/" + token.mid(pos);
            corruptedTokens << corrupted;
            qDebug() << "Ошибка внесена в букву:" << token << "->" << corrupted;
        } else {
            corruptedTokens << token;
        }
    }

    return corruptedTokens.join(" ");
}

QStringList MorseCodec::decode(const QString& morseCode) {
    QStringList results;
    results << "";

    QStringList tokens = morseCode.split(' ', Qt::SkipEmptyParts);

    for (const QString& token : tokens) {
        if (token == "/") {
            for (QString& res : results) {
                res += " ";
            }
            continue;
        }

        QStringList tokenVariants;
        if (token.contains('/')) {
            tokenVariants = recoverCandidates(token);
        } else {
            auto it = m_morseToRus.find(token);
            if (it != m_morseToRus.end()) {
                tokenVariants << QString(it->second);
            } else {
                tokenVariants << "?";
            }
        }

        QStringList newResults;
        for (const QString& res : results) {
            for (const QString& var : tokenVariants) {
                newResults << res + var;
            }
        }
        results = newResults;
    }

    return results;
}

QStringList MorseCodec::recoverCandidates(const QString& corruptedToken) {
    QStringList parts = corruptedToken.split('/');
    QStringList candidates;

    QString full = parts.join("");
    auto itFull = m_morseToRus.find(full);
    if (itFull != m_morseToRus.end()) {
        candidates << QString(itFull->second);
    }

    bool allPartsValid = true;
    QString separateResult;
    for (const QString& part : parts) {
        auto itPart = m_morseToRus.find(part);
        if (itPart != m_morseToRus.end()) {
            separateResult += itPart->second;
        } else {
            allPartsValid = false;
            break;
        }
    }
    if (allPartsValid && !separateResult.isEmpty() && !candidates.contains(separateResult)) {
        candidates << separateResult;
    }

    if (candidates.isEmpty()) {
        candidates << "[Не удалось восстановить]";
    }

    return candidates;
}
