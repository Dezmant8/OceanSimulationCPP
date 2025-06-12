#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <iostream>
#include <string>

// Шаблон на случай, разных уровней логирования
// (показывает применение template'ов)

template <typename Tag>
class Logger {
public:
    template <typename... Args>
    static void log(Args&&... args) {
        (std::cout << ... << args) << '\n';
    }
};
// Специализация по умолчанию
struct LogTag {};
using Log = Logger<LogTag>; // alias (короче писать)
#endif // LOGGER_HPP