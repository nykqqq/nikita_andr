#include <iostream>
#include <vector>
#include <array>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// Функция сортировки - передача по значению (копия массива)
void sortByValue(std::vector<int> arr, bool ascending) {
    if (ascending) {
        std::sort(arr.begin(), arr.end());
        std::cout << "Сортировка по возрастанию (передача по значению): ";
    } else {
        std::sort(arr.begin(), arr.end(), std::greater<int>());
        std::cout << "Сортировка по убыванию (передача по значению): ";
    }
    
    // Выводим отсортированный массив
    std::cout << "[";
    for (size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i];
        if (i < arr.size() - 1) std::cout << " ";
    }
    std::cout << "]" << std::endl;
}

// Функция сортировки - передача по ссылке (работаем с оригиналом)
void sortByReference(std::vector<int>& arr, bool ascending) {
    if (ascending) {
        std::sort(arr.begin(), arr.end());
        std::cout << "Сортировка по возрастанию (передача по ссылке): ";
    } else {
        std::sort(arr.begin(), arr.end(), std::greater<int>());
        std::cout << "Сортировка по убыванию (передача по ссылке): ";
    }
    
    // Выводим отсортированный массив
    std::cout << "[";
    for (size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i];
        if (i < arr.size() - 1) std::cout << " ";
    }
    std::cout << "]" << std::endl;
}

// Функция сортировки - передача по указателю (работаем с оригиналом через указатель)
void sortByPointer(std::vector<int>* arr, bool ascending) {
    if (ascending) {
        std::sort(arr->begin(), arr->end());
        std::cout << "Сортировка по возрастанию (передача по указателю): ";
    } else {
        std::sort(arr->begin(), arr->end(), std::greater<int>());
        std::cout << "Сортировка по убыванию (передача по указателю): ";
    }
    
    // Выводим отсортированный массив
    std::cout << "[";
    for (size_t i = 0; i < arr->size(); ++i) {
        std::cout << (*arr)[i];
        if (i < arr->size() - 1) std::cout << " ";
    }
    std::cout << "]" << std::endl;
}

// Функция для отображения массива
void displayArray(const std::vector<int>& arr, const std::string& message = "") {
    if (!message.empty()) {
        std::cout << message;
    }
    std::cout << "[";
    for (size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i];
        if (i < arr.size() - 1) std::cout << " ";
    }
    std::cout << "]" << std::endl;
}

// Демонстрация работы с std::array (фиксированный размер)
void demonstrateArray() {
    std::cout << "\n=== Работа с std::array ===" << std::endl;
    
    // Создаем std::array на 10 элементов
    std::array<int, 10> fixedArray;
    
    // Заполняем случайными числами от -10 до 10
    for (int i = 0; i < 10; ++i) {
        fixedArray[i] = rand() % 21 - 10; // от -10 до 10
    }
    
    std::cout << "Исходный std::array: [";
    for (int i = 0; i < 10; ++i) {
        std::cout << fixedArray[i];
        if (i < 9) std::cout << " ";
    }
    std::cout << "]" << std::endl;
    
    // Сортировка std::array
    std::sort(fixedArray.begin(), fixedArray.end());
    std::cout << "Отсортированный std::array: [";
    for (int i = 0; i < 10; ++i) {
        std::cout << fixedArray[i];
        if (i < 9) std::cout << " ";
    }
    std::cout << "]" << std::endl;
}

int main() {
    // Инициализация генератора случайных чисел
    srand(static_cast<unsigned int>(time(nullptr)));
    
    std::cout << "=== Демонстрация способов передачи аргументов в функцию ===" << std::endl;
    
    // 1. Создаем и заполняем вектор 10 случайными числами от -10 до 10
    std::vector<int> numbers;
    for (int i = 0; i < 10; ++i) {
        numbers.push_back(rand() % 21 - 10); // от -10 до 10
    }
    
    std::cout << "\nИсходный массив: ";
    displayArray(numbers);
    
    // 2. Демонстрация передачи по значению
    std::cout << "\n--- Передача по значению ---" << std::endl;
    std::cout << "Исходный массив до вызова sortByValue: ";
    displayArray(numbers);
    
    sortByValue(numbers, true); // Сортировка по возрастанию
    
    std::cout << "Исходный массив после вызова sortByValue: ";
    displayArray(numbers);
    std::cout << "Вывод: исходный массив НЕ изменился!" << std::endl;
    
    // 3. Демонстрация передачи по ссылке
    std::cout << "\n--- Передача по ссылке ---" << std::endl;
    std::cout << "Исходный массив до вызова sortByReference: ";
    displayArray(numbers);
    
    sortByReference(numbers, false); // Сортировка по убыванию
    
    std::cout << "Исходный массив после вызова sortByReference: ";
    displayArray(numbers);
    std::cout << "Вывод: исходный массив ИЗМЕНИЛСЯ!" << std::endl;
    
    // 4. Демонстрация передачи по указателю
    std::cout << "\n--- Передача по указателю ---" << std::endl;
    
    // Восстанавливаем исходный порядок
    std::random_shuffle(numbers.begin(), numbers.end());
    std::cout << "Массив после перемешивания: ";
    displayArray(numbers);
    
    std::cout << "Массив до вызова sortByPointer: ";
    displayArray(numbers);
    
    sortByPointer(&numbers, true); // Сортировка по возрастанию
    
    std::cout << "Массив после вызова sortByPointer: ";
    displayArray(numbers);
    std::cout << "Вывод: исходный массив ИЗМЕНИЛСЯ!" << std::endl;
    
    // 5. Демонстрация работы с std::array
    demonstrateArray();
    
    // 6. Сравнение способов передачи
    std::cout << "\n=== Сравнение способов передачи аргументов ===" << std::endl;
    std::cout << "1. Передача по значению:" << std::endl;
    std::cout << "   - Создается полная копия массива" << std::endl;
    std::cout << "   - Изменения внутри функции НЕ влияют на оригинал" << std::endl;
    std::cout << "   - Медленнее (копирование данных)" << std::endl;
    std::cout << "   - Потребляет больше памяти (хранит копию)" << std::endl;
    std::cout << "   - Безопаснее (нельзя случайно изменить оригинал)" << std::endl;
    
    std::cout << "\n2. Передача по ссылке:" << std::endl;
    std::cout << "   - Передается ссылка на оригинальный массив" << std::endl;
    std::cout << "   - Изменения внутри функции ВЛИЯЮТ на оригинал" << std::endl;
    std::cout << "   - Быстрее (нет копирования)" << std::endl;
    std::cout << "   - Экономит память" << std::endl;
    std::cout << "   - Менее безопасно (можно изменить оригинал)" << std::endl;
    std::cout << "   - Используется & в объявлении параметра" << std::endl;
    
    std::cout << "\n3. Передача по указателю:" << std::endl;
    std::cout << "   - Передается адрес оригинального массива" << std::endl;
    std::cout << "   - Изменения внутри функции ВЛИЯЮТ на оригинал" << std::endl;
    std::cout << "   - Быстрее (нет копирования)" << std::endl;
    std::cout << "   - Экономит память" << std::endl;
    std::cout << "   - Менее безопасно (можно изменить оригинал)" << std::endl;
    std::cout << "   - Требует проверки на nullptr" << std::endl;
    std::cout << "   - Используется * в объявлении параметра и -> для доступа" << std::endl;
    
    std::cout << "\nРекомендации:" << std::endl;
    std::cout << "- Используйте передачу по значению для небольших объектов" << std::endl;
    std::cout << "- Используйте передачу по ссылке для больших объектов" << std::endl;
    std::cout << "- Используйте const ссылку если не нужно изменять объект" << std::endl;
    std::cout << "- Используйте указатели когда нужна возможность передачи 'отсутствия' объекта (nullptr)" << std::endl;
    
    return 0;
}
