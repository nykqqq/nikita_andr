// Добавить элемент в конец строки k.
// Если строки k не существует — создаются пустые строки до k включительно.
void add_endline(size_t k, const std::string& item) {
    while (data.size() <= k) {
        data.push_back(std::vector<std::string>());
    }
    data[k].push_back(item);
}