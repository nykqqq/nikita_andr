#include <iostream>
#include <vector>
#include <algorithm>

// Функция для отображения массива
void displayArray(const std::vector<int>& arr) {
    if (arr.empty()) {
        std::cout << "[] (массив пуст)" << std::endl;
        return;
    }
    
    std::cout << "[";
    for (size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i];
        if (i < arr.size() - 1) {
            std::cout << " ";
        }
    }
    std::cout << "]" << std::endl;
}

// Функция для добавления элемента в начало
void addToBeginning(std::vector<int>& arr, int value) {
    arr.insert(arr.begin(), value);
}

// Функция для добавления элемента в конец
void addToEnd(std::vector<int>& arr, int value) {
    arr.push_back(value);
}

// Функция для очистки массива
void clearArray(std::vector<int>& arr) {
    arr.clear();
}

// Функция для поиска элемента в массиве
std::vector<size_t> findElement(const std::vector<int>& arr, int value) {
    std::vector<size_t> indices;
    
    for (size_t i = 0; i < arr.size(); ++i) {
        if (arr[i] == value) {
            indices.push_back(i);
        }
    }
    
    return indices;
}

// Функция для отображения результатов поиска
void displaySearchResults(const std::vector<size_t>& indices) {
    if (indices.empty()) {
        std::cout << "[] (элемент не найден)" << std::endl;
        return;
    }
    
    std::cout << "[";
    for (size_t i = 0; i < indices.size(); ++i) {
        std::cout << indices[i];
        if (i < indices.size() - 1) {
            std::cout << ",";
        }
    }
    std::cout << "]" << std::endl;
}

// Функция для выполнения задания варианта
void processVariantTask(std::vector<int>& arr) {
    if (arr.empty()) {
        std::cout << "Массив пуст, операция невозможна" << std::endl;
        return;
    }
    
    std::cout << "Массив до обработки: ";
    displayArray(arr);
    
    if (arr.size() % 2 == 0) {
        // Если количество элементов четное - удалить первый элемент
        if (!arr.empty()) {
            arr.erase(arr.begin());
            std::cout << "Удален первый элемент (четное количество элементов)" << std::endl;
        }
    } else {
        // Если количество элементов нечетное - удалить до 4 последних элементов
        if (arr.size() > 4) {
            // Удаляем все элементы с начала до (размер - 4)
            arr.erase(arr.begin(), arr.begin() + (arr.size() - 4));
            std::cout << "Удалены элементы до 4 последних (нечетное количество элементов)" << std::endl;
        } else {
            std::cout << "Массив содержит 4 или меньше элементов, удаление невозможно" << std::endl;
        }
    }
    
    std::cout << "Массив после обработки: ";
    displayArray(arr);
}

// Функция для отображения меню
void displayMenu() {
    std::cout << "\n=== МЕНЮ ===" << std::endl;
    std::cout << "0. Выход" << std::endl;
    std::cout << "1. Просмотр массива" << std::endl;
    std::cout << "2. Добавить элемент в начало" << std::endl;
    std::cout << "3. Добавить элемент в конец" << std::endl;
    std::cout << "4. Очистка всего массива" << std::endl;
    std::cout << "5. Поиск элемента в массиве" << std::endl;
    std::cout << "6. Задание варианта" << std::endl;
    std::cout << "Выберите пункт меню: ";
}

int main() {
    std::vector<int> array;
    bool running = true;
    int choice;
    
    std::cout << "Программа для работы с массивом целых чисел" << std::endl;
    
    while (running) {
        displayMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 0:
                running = false;
                std::cout << "Выход из программы." << std::endl;
                break;
                
            case 1: {
                std::cout << "Текущий массив: ";
                displayArray(array);
                break;
            }
                
            case 2: {
                std::cout << "Добавление элемента в начало" << std::endl;
                std::cout << "Массив до: ";
                displayArray(array);
                
                int value;
                std::cout << "Введите значение: ";
                std::cin >> value;
                addToBeginning(array, value);
                
                std::cout << "Массив после: ";
                displayArray(array);
                break;
            }
                
            case 3: {
                std::cout << "Добавление элемента в конец" << std::endl;
                std::cout << "Массив до: ";
                displayArray(array);
                
                int value;
                std::cout << "Введите значение: ";
                std::cin >> value;
                addToEnd(array, value);
                
                std::cout << "Массив после: ";
                displayArray(array);
                break;
            }
                
            case 4: {
                std::cout << "Очистка массива" << std::endl;
                std::cout << "Массив до: ";
                displayArray(array);
                
                clearArray(array);
                
                std::cout << "Массив после: ";
                displayArray(array);
                break;
            }
                
            case 5: {
                std::cout << "Поиск элемента в массиве" << std::endl;
                std::cout << "Текущий массив: ";
                displayArray(array);
                
                if (array.empty()) {
                    std::cout << "Массив пуст, поиск невозможен" << std::endl;
                    break;
                }
                
                int value;
                std::cout << "Введите значение для поиска: ";
                std::cin >> value;
                std::vector<size_t> indices = findElement(array, value);
                
                std::cout << "Индексы элемента " << value << ": ";
                displaySearchResults(indices);
                break;
            }
                
            case 6: {
                std::cout << "Выполнение задания варианта:" << std::endl;
                std::cout << "Если количество элементов четное, то удалить первый," << std::endl;
                std::cout << "иначе – удалить до 4 последних если это возможно." << std::endl;
                
                processVariantTask(array);
                break;
            }
                
            default:
                std::cout << "Неверный пункт меню. Пожалуйста, выберите от 0 до 6." << std::endl;
                std::cin.clear(); // Очищаем флаги ошибок
                std::cin.ignore(10000, '\n'); // Очищаем буфер
                break;
        }
        
        // Очищаем буфер ввода после каждого выбора
        std::cin.ignore(10000, '\n');
    }
    
    return 0;
}
