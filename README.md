void delete_row(size_t i) {
    if (data.empty()) {
        std::cout << "Массив пуст." << std::endl;
        return;
    }
    if (i >= data.size()) {
        std::cout << "Строки " << i << " нет. Удаляю все строки 0.." << (data.size() - 1) << "." << std::endl;
        data.clear();
        return;
    }
    data.erase(data.begin(), data.begin() + (long)(i + 1));
}