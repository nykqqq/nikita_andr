#define NOMINMAX
#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <cctype>
#include <locale>
#include <codecvt>

// ============= Вспомогательная функция для русской сортировки =============
std::wstring utf8_to_wstring(const std::string& str) {
    try {
        std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
        return conv.from_bytes(str);
    } catch (...) {
        std::wstring w;
        for (size_t i = 0; i < str.size(); ++i)
            w += (wchar_t)(unsigned char)str[i];
        return w;
    }
}

// ============= Класс зубчатого массива =============
class JaggedArray {
private:
    std::vector<std::vector<std::string> > data;

public:
    JaggedArray() {}

    JaggedArray(const std::string& filename) {
        loadFromFile(filename);
    }

    // ========== Загрузка из файла (txt / csv / json) ==========
    void loadFromFile(const std::string& filename) {
        data.clear();

        std::ifstream file(filename.c_str());
        if (!file.is_open()) {
            throw std::runtime_error("Не удалось открыть файл: " + filename);
        }

        int format = 1; // 0=txt, 1=csv, 2=json
        if (filename.size() >= 4) {
            std::string ext = filename.substr(filename.size() - 4);
            if (ext == ".txt") format = 0;
            else if (ext == ".csv") format = 1;
            else if (ext == "json") format = 2;
        }

        char delimiter = (format == 0) ? ' ' : ',';

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            if (format == 2) {
                std::string cleaned;
                for (size_t k = 0; k < line.size(); ++k) {
                    char c = line[k];
                    if (c != '[' && c != ']' && c != '"') cleaned += c;
                }
                line = cleaned;
            }

            std::vector<std::string> row;
            std::stringstream ss(line);
            std::string item;

            while (std::getline(ss, item, delimiter)) {
                size_t start = item.find_first_not_of(" \t");
                size_t end = item.find_last_not_of(" \t");
                if (start == std::string::npos) continue;
                item = item.substr(start, end - start + 1);
                if (!item.empty()) row.push_back(item);
            }

            if (!row.empty()) data.push_back(row);
        }
        file.close();
    }

    // ========== Доступ A[i] ==========
    std::vector<std::string>& operator[](size_t index) {
        if (index >= data.size()) throw std::out_of_range("Индекс строки вне диапазона");
        return data[index];
    }

    const std::vector<std::string>& operator[](size_t index) const {
        if (index >= data.size()) throw std::out_of_range("Индекс строки вне диапазона");
        return data[index];
    }

    // ========== Добавить в конец строки k ==========
    // Если строки k нет — создаются пустые строки до k включительно.
    void add_endline(size_t k, const std::string& item) {
        // Создаём пустые строки, пока не дойдём до k
        while (data.size() <= k) {
            data.push_back(std::vector<std::string>());
        }
        data[k].push_back(item);
    }

    // ========== Удалить элемент по индексам ==========
    void delete_item(size_t i, size_t j) {
        if (i >= data.size() || j >= data[i].size()) throw std::out_of_range("Индекс вне диапазона");
        data[i].erase(data[i].begin() + (long)j);
    }

    // ========== Удалить элемент по значению ==========
    void delete_item(const std::string& item) {
        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < data[i].size(); ++j) {
                if (data[i][j] == item) {
                    data[i].erase(data[i].begin() + (long)j);
                    return;
                }
            }
        }
    }

    // ========== Удалить строку с каскадом ==========
    // Удаляются строки 0..i включительно. Остальные сдвигаются вверх.
    void delete_row(size_t i) {
        if (i >= data.size()) throw std::out_of_range("Индекс строки вне диапазона");
        data.erase(data.begin(), data.begin() + (long)(i + 1));
    }

    // ========== Оператор + ==========
    JaggedArray operator+(const JaggedArray& other) const {
        JaggedArray result;
        size_t rows = data.size() < other.data.size() ? data.size() : other.data.size();

        for (size_t i = 0; i < rows; ++i) {
            std::vector<std::string> row;
            size_t cols = data[i].size() < other.data[i].size()
                              ? data[i].size() : other.data[i].size();
            for (size_t j = 0; j < cols; ++j) {
                const std::string& s1 = data[i][j];
                const std::string& s2 = other.data[i][j];
                std::string r;
                if (s1.empty()) r = "";
                else if (s2.empty()) r = s1;
                else r = s1 + s2;
                row.push_back(r);
            }
            for (size_t j = cols; j < data[i].size(); ++j) row.push_back(data[i][j]);
            for (size_t j = cols; j < other.data[i].size(); ++j) row.push_back(other.data[i][j]);
            result.data.push_back(row);
        }
        for (size_t i = rows; i < data.size(); ++i) result.data.push_back(data[i]);
        for (size_t i = rows; i < other.data.size(); ++i) result.data.push_back(other.data[i]);
        return result;
    }

    // ========== Оператор ++ ==========
    JaggedArray& operator++() {
        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < data[i].size(); ++j) {
                std::string& s = data[i][j];
                if (!s.empty() && std::isalpha((unsigned char)s[0])) {
                    s[0] = (char)(s[0] + 1);
                }
            }
        }
        return *this;
    }

    JaggedArray operator++(int) {
        JaggedArray temp = *this;
        ++(*this);
        return temp;
    }

    // ========== Сортировка внутри строк по алфавиту (латиница + кириллица) ==========
    void sortRows() {
        for (size_t i = 0; i < data.size(); ++i) {
            std::sort(data[i].begin(), data[i].end(),
                [](const std::string& a, const std::string& b) {
                    std::wstring wa = utf8_to_wstring(a);
                    std::wstring wb = utf8_to_wstring(b);

                    for (size_t k = 0; k < wa.size(); ++k)
                        wa[k] = std::towlower(wa[k]);
                    for (size_t k = 0; k < wb.size(); ++k)
                        wb[k] = std::towlower(wb[k]);

                    return wa < wb;
                });
        }
    }

    // ========== Вывод с цветами ==========
    void print() const {
        for (size_t i = 0; i < data.size(); ++i) {
#ifdef _WIN32
            HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
            static const int colors[8] = {7, 9, 10, 11, 12, 13, 14, 15};
            SetConsoleTextAttribute(h, colors[i % 8]);
#else
            static const char* colors[8] = {
                "\033[0m","\033[34m","\033[32m","\033[36m",
                "\033[31m","\033[35m","\033[33m","\033[37m"
            };
            std::cout << colors[i % 8];
#endif
            std::cout << "Строка " << i << " (" << data[i].size() << "): ";
            for (size_t j = 0; j < data[i].size(); ++j)
                std::cout << "[" << data[i][j] << "] ";
            std::cout << std::endl;
        }
#ifdef _WIN32
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(h, 7);
#else
        std::cout << "\033[0m";
#endif
    }

    size_t size() const { return data.size(); }
    size_t rowSize(size_t i) const { return data[i].size(); }
};

// ============= Вспомогательные функции для ввода =============
size_t readIndex(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string s;
        std::getline(std::cin, s);
        if (s.empty()) continue;
        try {
            size_t pos;
            long long val = std::stoll(s, &pos);
            if (pos != s.size() || val < 0) {
                std::cout << "Нужно неотрицательное число. Попробуйте снова." << std::endl;
                continue;
            }
            return (size_t)val;
        } catch (...) {
            std::cout << "Не число. Попробуйте снова." << std::endl;
        }
    }
}

std::string readString(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

JaggedArray* chooseFile(JaggedArray& txt, JaggedArray& csv, JaggedArray& json) {
    while (true) {
        std::cout << "\nВыберите массив для работы:" << std::endl;
        std::cout << "  1 - из data.txt" << std::endl;
        std::cout << "  2 - из data.csv" << std::endl;
        std::cout << "  3 - из data.json" << std::endl;
        std::cout << "  0 - назад" << std::endl;

        std::string choice = readString("Ваш выбор: ");
        if (choice == "0") return NULL;
        if (choice == "1") return &txt;
        if (choice == "2") return &csv;
        if (choice == "3") return &json;
        std::cout << "Неверный ввод." << std::endl;
    }
}

// ============= Пункты меню =============
void menuPrint(JaggedArray& arr) {
    std::cout << "\n=== Вывод массива ===" << std::endl;
    arr.print();
}

void menuAdd(JaggedArray& arr) {
    std::cout << "\n=== Добавление элемента ===" << std::endl;
    std::cout << "(если строки k нет — создадутся пустые строки до неё)" << std::endl;
    arr.print();

    size_t k = readIndex("Введите номер строки k: ");
    std::string item = readString("Введите значение: ");

    arr.add_endline(k, item);
    std::cout << "Элемент '" << item << "' добавлен в строку " << k << "." << std::endl;
    arr.print();
}

void menuDeleteByIndex(JaggedArray& arr) {
    std::cout << "\n=== Удаление элемента по индексу ===" << std::endl;
    arr.print();

    size_t i = readIndex("Введите индекс строки i: ");
    size_t j = readIndex("Введите индекс столбца j: ");

    try {
        std::string removed = arr[i][j];
        arr.delete_item(i, j);
        std::cout << "Элемент [" << i << "][" << j << "] = '" << removed << "' удалён." << std::endl;
        arr.print();
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
    }
}

void menuDeleteByValue(JaggedArray& arr) {
    std::cout << "\n=== Удаление по значению ===" << std::endl;
    arr.print();

    std::string item = readString("Введите значение для удаления: ");
    arr.delete_item(item);
    std::cout << "Удаление выполнено." << std::endl;
    arr.print();
}

void menuDeleteRow(JaggedArray& arr) {
    std::cout << "\n=== Удаление строки с каскадом ===" << std::endl;
    std::cout << "ВНИМАНИЕ: удалятся строки 0..i включительно!" << std::endl;
    arr.print();

    size_t i = readIndex("Введите номер строки i: ");
    try {
        arr.delete_row(i);
        std::cout << "Строки 0.." << i << " удалены." << std::endl;
        arr.print();
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
    }
}

void menuSort(JaggedArray& arr) {
    std::cout << "\n=== Сортировка внутри строк по алфавиту ===" << std::endl;
    arr.sortRows();
    arr.print();
}

void menuInc(JaggedArray& arr) {
    std::cout << "\n=== Операция ++ (сдвиг первой буквы) ===" << std::endl;
    std::cout << "До:" << std::endl;
    arr.print();
    ++arr;
    std::cout << "После:" << std::endl;
    arr.print();
}

void menuConcat(JaggedArray& a, JaggedArray& b) {
    std::cout << "\n=== Оператор + (конкатенация) ===" << std::endl;
    std::cout << "\nA:" << std::endl; a.print();
    std::cout << "\nB:" << std::endl; b.print();
    JaggedArray c = a + b;
    std::cout << "\nA + B:" << std::endl;
    c.print();
}

// ============= Главная функция =============
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    try {
        JaggedArray arrTxt("data.txt");
        JaggedArray arrCsv("data.csv");
        JaggedArray arrJson("data.json");

        std::cout << "Файлы успешно загружены." << std::endl;

        while (true) {
            std::cout << "\n================= МЕНЮ =================" << std::endl;
            std::cout << " 1 - Вывести массив" << std::endl;
            std::cout << " 2 - Добавить элемент (add_endline)" << std::endl;
            std::cout << " 3 - Удалить элемент по индексу [i][j]" << std::endl;
            std::cout << " 4 - Удалить элемент по значению" << std::endl;
            std::cout << " 5 - Удалить строку с каскадом (delete_row)" << std::endl;
            std::cout << " 6 - Сортировать строки по алфавиту" << std::endl;
            std::cout << " 7 - Операция ++ (сдвиг первой буквы)" << std::endl;
            std::cout << " 8 - Оператор + (два массива)" << std::endl;
            std::cout << " 0 - Выход" << std::endl;
            std::cout << "=======================================" << std::endl;

            std::string choice = readString("Ваш выбор: ");

            if (choice == "0") {
                std::cout << "Выход." << std::endl;
                break;
            }
            else if (choice == "1") {
                JaggedArray* arr = chooseFile(arrTxt, arrCsv, arrJson);
                if (arr) menuPrint(*arr);
            }
            else if (choice == "2") {
                JaggedArray* arr = chooseFile(arrTxt, arrCsv, arrJson);
                if (arr) menuAdd(*arr);
            }
            else if (choice == "3") {
                JaggedArray* arr = chooseFile(arrTxt, arrCsv, arrJson);
                if (arr) menuDeleteByIndex(*arr);
            }
            else if (choice == "4") {
                JaggedArray* arr = chooseFile(arrTxt, arrCsv, arrJson);
                if (arr) menuDeleteByValue(*arr);
            }
            else if (choice == "5") {
                JaggedArray* arr = chooseFile(arrTxt, arrCsv, arrJson);
                if (arr) menuDeleteRow(*arr);
            }
            else if (choice == "6") {
                JaggedArray* arr = chooseFile(arrTxt, arrCsv, arrJson);
                if (arr) menuSort(*arr);
            }
            else if (choice == "7") {
                JaggedArray* arr = chooseFile(arrTxt, arrCsv, arrJson);
                if (arr) menuInc(*arr);
            }
            else if (choice == "8") {
                std::cout << "\nВыберите первый массив (A):" << std::endl;
                JaggedArray* a = chooseFile(arrTxt, arrCsv, arrJson);
                if (!a) continue;

                std::cout << "\nВыберите второй массив (B):" << std::endl;
                JaggedArray* b = chooseFile(arrTxt, arrCsv, arrJson);
                if (!b) continue;

                menuConcat(*a, *b);
            }
            else {
                std::cout << "Неверный ввод. Попробуйте снова." << std::endl;
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "ОШИБКА: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}