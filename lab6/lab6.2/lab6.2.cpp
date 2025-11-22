
#include <iostream>
#include <cstdlib>

using namespace std;

// Функция для нахождения строк с нулевыми значениями
int* findZeroRows(int** matrix, int rows, int cols, int* count) {
    *count = 0;

    // Подсчитываем количество строк с нулями
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] == 0) {
                (*count)++;
                break;
            }
        }
    }

    if (*count == 0) {
        return nullptr;
    }

    // Выделяем память
    int* zeroRows = (int*)malloc(*count * sizeof(int));
    if (zeroRows == nullptr) {
        return nullptr;
    }

    // Заполняем массив индексами строк с нулями
    int index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] == 0) {
                zeroRows[index++] = i;
                break;
            }
        }
    }

    return zeroRows;
}

// Функция для удаления строк из матрицы
void removeRowsFromMatrix(int** matrix, int* rows, int cols, int* rowsToRemove, int removeCount) {
    int newIndex = 0;

    // Сдвигаем строки, перезаписывая те, которые нужно удалить
    for (int i = 0; i < *rows; i++) {
        bool shouldRemove = false;

        // Проверяем, нужно ли удалять текущую строку
        for (int j = 0; j < removeCount; j++) {
            if (i == rowsToRemove[j]) {
                shouldRemove = true;
                break;
            }
        }

        if (!shouldRemove) {
            // Копируем строку на новую позицию
            if (newIndex != i) {
                for (int j = 0; j < cols; j++) {
                    matrix[newIndex][j] = matrix[i][j];
                }
            }
            newIndex++;
        }
    }

    
    *rows = newIndex;
}

// Функция для вывода матрицы
void printMatrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
}

// Функция для освобождения памяти
void freeMatrix(int** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    setlocale(LC_ALL, "Russian");
    unsigned int A, B;
    int C, D;


    // Ввод данных с клавиатуры
    cout << "Введите A: ";
    cin >> A;
    cout << "Введите B: ";
    cin >> B;
    cout << "Введите C: ";
    cin >> C;
    cout << "Введите D: ";
    cin >> D;

    // 1. Создаем матрицу 2x2
    cout << "\n1. Создаем матрицу 2x2:" << endl;
    int** matrix = (int**)malloc(2 * sizeof(int*));
    for (int i = 0; i < 2; i++) {
        matrix[i] = (int*)malloc(2 * sizeof(int));
    }

    // Заполняем матрицу 2x2
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            matrix[i][j] = i * C + j * D;
        }
    }
    printMatrix(matrix, 2, 2);

    cout << "\n2. Расширяем матрицу до (" << (A + 2) << "x" << (B + 2) << "):" << endl;
    int rows = A + 2;
    int cols = B + 2;

    // Расширяем массив указателей на строки
    matrix = (int**)realloc(matrix, rows * sizeof(int*));


    for (int i = 0; i < rows; i++) {
        if (i < 2) {

            matrix[i] = (int*)realloc(matrix[i], cols * sizeof(int));
        }
        else {
            // Выделяем память для новых строк
            matrix[i] = (int*)malloc(cols * sizeof(int));
        }
    }

    // Заполняем расширенную матрицу по формуле
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = i * C + j * D;
        }
    }
    printMatrix(matrix, rows, cols);

    // 3. Передаем матрицу в функцию и находим строки с нулями
    cout << "\n3. Ищем строки с нулевыми значениями:" << endl;

    int zeroCount;
    int* zeroRows = findZeroRows(matrix, rows, cols, &zeroCount);

    if (zeroRows != nullptr && zeroCount > 0) {
        cout << "Найдены строки с нулями: ";
        for (int i = 0; i < zeroCount; i++) {
            cout << zeroRows[i] +
                1 << " ";
        }
        cout << endl;

        // 4. Удаляем строки с нулями
        cout << "\n4. Удаляем строки с нулями:" << endl;
        removeRowsFromMatrix(matrix, &rows, cols, zeroRows, zeroCount);

        cout << "Результирующая матрица (" << rows << "x" << cols << "):" << endl;
        if (rows > 0) {
            printMatrix(matrix, rows, cols);
        }
        else {
            cout << "Все строки удалены" << endl;
        }

        free(zeroRows);
    }
    else {
        cout << "Строк с нулевыми значениями не найдено." << endl;
        cout << "Матрица без изменений:" << endl;
        printMatrix(matrix, rows, cols);
    }

    // Освобождаем память
    freeMatrix(matrix, rows);



    cout << "--------------------------------------------" << endl;
    // Ввод значений переменных
    int t, q;
    std::cout << "Введите значение переменной a: ";
    std::cin >> t;
    std::cout << "Введите значение переменной b: ";
    std::cin >> q;

    std::cout << "Исходные значения: a = " << t << ", b = " << q << std::endl;

    // Выделение памяти с помощью new
    int* ptrT = new int;
    int* ptrQ = new int;

    // Направление указателей на переменные (копирование значений)
    *ptrT = t;
    *ptrQ = q;

    // Увеличение значения переменной a в 2 раза через указатель
    *ptrT *= 2;
    t = *ptrT; // Обновляем исходную переменную
    std::cout << "После увеличения a в 2 раза: a = " << t << ", b = " << q << std::endl;

    // Обмен значений через указатели
    int temp = *ptrT;
    *ptrT = *ptrQ;
    *ptrQ = temp;

    // Обновляем исходные переменные
    t = *ptrT;
    q = *ptrQ;

    std::cout << "После обмена значений: a = " << t << ", b = " << q << std::endl;

    // Освобождение памяти с помощью delete
    delete ptrT;
    delete ptrQ;

    return 0;
}