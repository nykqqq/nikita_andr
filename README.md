https://drive.google.com/file/d/1A88J4kslU2Gq9ZwTyo5wfY5OxpymCWAr/view?usp=sharing
// ✅ Сортировка по алфавиту (латиница + кириллица)
void sortRows() {
    for (size_t i = 0; i < data.size(); ++i) {
        std::sort(data[i].begin(), data[i].end(),
            [](const std::string& a, const std::string& b) {
                std::wstring wa = utf8_to_wstring(a);
                std::wstring wb = utf8_to_wstring(b);

                // Приводим к нижнему регистру для сравнения без учёта регистра
                for (size_t k = 0; k < wa.size(); ++k)
                    wa[k] = std::towlower(wa[k]);
                for (size_t k = 0; k < wb.size(); ++k)
                    wb[k] = std::towlower(wb[k]);

                return wa < wb;
            });
    }
}