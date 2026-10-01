// Преобразование UTF-8 → wstring (для правильной русской сортировки)
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