#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <windows.h>
#include <cctype>

using namespace std;

class JaggedArray {
private:
    vector<vector<string>> data;

    bool isRowEmpty(const vector<string>& row) const {
        for (const auto& s : row)
            if (!s.empty()) return false;
        return true;
    }

    // Удаление пустых строк сверху (хвоста)
    void trimEmptyTop() {
        while (!data.empty() && isRowEmpty(data.back()))
            data.pop_back();
    }

public:
    JaggedArray() {}

    // Создание файла, если его нет (демо-данные)
    static void ensureFileExists(const string& filename) {
        ifstream check(filename);
        if (check.is_open()) { check.close(); return; } // файл уже есть

        ofstream out(filename);
        if (!out.is_open()) {
            cout << "Не удалось создать файл \"" << filename << "\"\n";
            return;
        }
        // Стартовые данные
        out << "apple banana cherry\n";
        out << "dog cat\n";
        out << "hello world test\n";
        out.close();
        cout << "Файл \"" << filename << "\" не найден — создан с демо-данными.\n";
    }

    // Второй файл для оператора +
    static void ensureSecondFileExists(const string& filename) {
        ifstream check(filename);
        if (check.is_open()) { check.close(); return; }

        ofstream out(filename);
        if (!out.is_open()) {
            cout << "Не удалось создать файл \"" << filename << "\"\n";
            return;
        }
        out << "red green\n";
        out << "blue\n";
        out << "one two three\n";
        out.close();
        cout << "Файл \"" << filename << "\" не найден — создан с демо-данными.\n";
    }

    bool loadFromFile(const string& filename = "input.txt") {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Не удалось открыть \"" << filename << "\"\n";
            return false;
        }
        data.clear();
        string line;
        while (getline(file, line)) {
            vector<string> row;
            istringstream iss(line);
            string token;
            while (iss >> token) row.push_back(token);
            data.push_back(row);
        }
        file.close();
        cout << "Загружено строк из \"" << filename << "\": " << data.size() << "\n";
        return true;
    }

    // Сохранение массива обратно в файл
    bool saveToFile(const string& filename = "input.txt") const {
        ofstream out(filename);
        if (!out.is_open()) {
            cout << "Не удалось записать в \"" << filename << "\"\n";
            return false;
        }
        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < data[i].size(); ++j) {
                out << data[i][j];
                if (j + 1 < data[i].size()) out << ' ';
            }
            out << '\n';
        }
        out.close();
        cout << "Массив сохранён в \"" << filename << "\"\n";
        return true;
    }

    // Прокси A[i][j]
    class ProxyRow {
    private:
        vector<string>* row;
    public:
        ProxyRow(vector<string>* r) : row(r) {}

        string& operator[](size_t j) {
            if (j >= row->size())
                row->resize(j + 1, "");
            return (*row)[j];
        }
        size_t size() const { return row->size(); }
    };

    ProxyRow operator[](size_t i) {
        if (i >= data.size())
            data.resize(i + 1);
        return ProxyRow(&data[i]);
    }

    const string& at(size_t i, size_t j) const {
        static const string empty = "";
        if (i >= data.size() || j >= data[i].size()) return empty;
        return data[i][j];
    }

    // Удаление по двойному индексу
    void remove(size_t i, size_t j) {
        if (i >= data.size() || j >= data[i].size()) {
            cout << "Индекс вне диапазона\n";
            return;
        }
        data[i].erase(data[i].begin() + j);
        trimEmptyTop(); // уберёт пустые строки сверху, включая саму i, если она стала пустой
        cout << "Элемент [" << i << "][" << j << "] удалён\n";
    }

    // Удаление по значению
    void remove(const string& item) {
        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < data[i].size(); ++j) {
                if (data[i][j] == item) {
                    data[i].erase(data[i].begin() + j);
                    trimEmptyTop();
                    cout << "Элемент \"" << item << "\" удалён\n";
                    return;
                }
            }
        }
        cout << "Элемент \"" << item << "\" не найден\n";
    }

    // Добавить в конец строки k
    void add_endline(size_t k, const string& item) {
        if (k >= data.size())
            data.resize(k + 1);
        data[k].push_back(item);
        cout << "Элемент \"" << item << "\" добавлен в строку " << k << "\n";
    }

    // Оператор + : поэлементная конкатенация
    // "" + "a" = "a"; пустота + "a" = пустота (пустой остаётся пустым)
    JaggedArray operator+(const JaggedArray& other) const {
        JaggedArray result;
        size_t n = max(data.size(), other.data.size());
        result.data.resize(n);

        for (size_t i = 0; i < n; ++i) {
            size_t m = 0;
            if (i < data.size()) m = max(m, data[i].size());
            if (i < other.data.size()) m = max(m, other.data[i].size());
            result.data[i].resize(m);

            for (size_t j = 0; j < m; ++j) {
                string a = (i < data.size() && j < data[i].size()) ? data[i][j] : "";
                string b = (i < other.data.size() && j < other.data[i].size()) ? other.data[i][j] : "";

                if (a.empty() && b.empty()) result.data[i][j] = "";
                else if (a.empty()) result.data[i][j] = b;
                else if (b.empty()) result.data[i][j] = a;
                else result.data[i][j] = a + b;
            }
        }
        return result;
    }

    // Оператор ++ : первый символ сдвигается на 1 по коду, если буква
    JaggedArray& operator++() {
        for (auto& row : data) {
            for (auto& s : row) {
                if (!s.empty() && isalpha((unsigned char)s[0])) {
                    unsigned char c = (unsigned char)s[0];
                    if (c == 'z') s[0] = 'a';
                    else if (c == 'Z') s[0] = 'A';
                    else s[0] = (char)(c + 1);
                }
            }
        }
        return *this;
    }

    JaggedArray operator++(int) {
        JaggedArray copy = *this;
        ++(*this);
        return copy;
    }

    void sortRows() {
        for (auto& row : data) {
            sort(row.begin(), row.end(), [](const string& a, const string& b) {
                size_t n = min(a.size(), b.size());
                for (size_t i = 0; i < n; ++i) {
                    char ca = tolower((unsigned char)a[i]);
                    char cb = tolower((unsigned char)b[i]);
                    if (ca != cb) return ca < cb;
                }
                return a.size() < b.size();
                });
        }
        cout << "Строки отсортированы по английскому алфавиту (без учёта регистра)\n";
    }

    void print() const {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        int colors[] = { 7, 9, 10, 11, 12, 13, 14, 6, 3, 5, 2, 1 };
        const int colorCount = 12;

        if (data.empty()) {
            cout << "(массив пуст)\n";
            return;
        }
        for (size_t i = 0; i < data.size(); ++i) {
            SetConsoleTextAttribute(hConsole, colors[i % colorCount]);
            cout << "[" << i << "]: ";
            for (size_t j = 0; j < data[i].size(); ++j)
                cout << "\"" << data[i][j] << "\" ";
            cout << "\n";
        }
        SetConsoleTextAttribute(hConsole, 7);
    }

    size_t rows() const { return data.size(); }
};

void showMenu() {
    cout << "\n===== МЕНЮ =====\n";
    cout << "1. Перезагрузить массив из input.txt\n";
    cout << "2. Показать массив\n";
    cout << "3. Обратиться к A[i][j] (получить/установить)\n";
    cout << "4. Удалить по двойному индексу delete(i, j)\n";
    cout << "5. Удалить по значению delete(item)\n";
    cout << "6. Добавить элемент в конец строки k\n";
    cout << "7. Оператор + (поэлементная конкатенация с input2.txt)\n";
    cout << "8. Оператор ++ (сдвиг первых букв)\n";
    cout << "9. Сортировать строки\n";
    cout << "10. Сохранить массив в input.txt\n";
    cout << "0. Выход\n";
    cout << "Ваш выбор: ";
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    setlocale(LC_ALL, "RU");

    const string FILE1 = "input.txt";
    const string FILE2 = "input2.txt";

    // Автосоздание файлов в папке проекта, если их нет
    cout << "=== Проверка файлов проекта ===\n";
    JaggedArray::ensureFileExists(FILE1);
    JaggedArray::ensureSecondFileExists(FILE2);

    JaggedArray arr;
    cout << "\n=== Автозагрузка " << FILE1 << " ===\n";
    arr.loadFromFile(FILE1);
    arr.print();

    JaggedArray arr2;
    bool arr2Loaded = false;

    int choice;
    while (true) {
        showMenu();
        cin >> choice;
        if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); continue; }
        cin.ignore();

        switch (choice) {
        case 1:
            arr.loadFromFile(FILE1);
            arr.print();
            break;

        case 2:
            arr.print();
            break;

        case 3: {
            size_t i, j;
            cout << "i j: "; cin >> i >> j; cin.ignore();
            cout << "Текущее: \"" << arr[i][j] << "\"\n";
            cout << "Новое значение (Enter — оставить): ";
            string val; getline(cin, val);
            if (!val.empty()) arr[i][j] = val;
            break;
        }

        case 4: {
            size_t i, j;
            cout << "i j: "; cin >> i >> j; cin.ignore();
            arr.remove(i, j);
            break;
        }

        case 5: {
            cout << "Значение: ";
            string val; getline(cin, val);
            arr.remove(val);
            break;
        }

        case 6: {
            size_t k;
            cout << "k: "; cin >> k; cin.ignore();
            cout << "Значение: ";
            string val; getline(cin, val);
            arr.add_endline(k, val);
            break;
        }

        case 7: {
            if (!arr2Loaded) {
                JaggedArray::ensureSecondFileExists(FILE2);
                arr2.loadFromFile(FILE2);
                arr2Loaded = true;
            }
            arr = arr + arr2;
            cout << "Результат операции + сохранён в основной массив:\n";
            arr.print();
            break;
        }

        case 8:
            ++arr;
            cout << "Оператор ++ применён:\n";
            arr.print();
            break;

        case 9:
            arr.sortRows();
            arr.print();
            break;

        case 10:
            arr.saveToFile(FILE1);
            break;

        case 0:
            return 0;

        default:
            cout << "Неверный выбор\n";
        }
    }
    return 0;
}
