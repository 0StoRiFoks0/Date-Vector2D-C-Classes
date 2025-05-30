#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cmath>
#include <ctime>
#include <stdexcept>

class Date {
private:
    int day, month, year;
    static int currentCount;
    static int totalCreated;

    static bool isLeapYear(int y) {
        return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    }

    static int daysInMonth(int m, int y) {
        static const int days[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
        if (m == 2 && isLeapYear(y)) return 29;
        return days[m - 1];
    }

    void normalize() {
        while (day > daysInMonth(month, year)) {
            day -= daysInMonth(month, year);
            month++;
            if (month > 12) {
                month = 1;
                year++;
            }
        }
        while (day < 1) {
            month--;
            if (month < 1) {
                month = 12;
                year--;
            }
            day += daysInMonth(month, year);
        }
    }

public:
    Date(int d = 1, int m = 1, int y = 1) : day(d), month(m), year(y) {
        if (m < 1 || m > 12 || d < 1 || d > daysInMonth(m, y)) {
            throw std::invalid_argument("Invalid date");
        }
        currentCount++;
        totalCreated++;
    }

    Date(const Date& other) : day(other.day), month(other.month), year(other.year) {
        currentCount++;
        totalCreated++;
    }

    ~Date() {
        currentCount--;
    }

    Date& operator++() {
        day++;
        normalize();
        return *this;
    }

    Date operator++(int) {
        Date temp = *this;
        ++(*this);
        return temp;
    }

    Date& operator--() {
        day--;
        normalize();
        return *this;
    }

    Date operator--(int) {
        Date temp = *this;
        --(*this);
        return temp;
    }

    Date operator+(int days) const {
        Date temp(*this);
        temp.day += days;
        temp.normalize();
        return temp;
    }

    int operator-(const Date& other) const {
        std::tm a = {0, 0, 0, day, month - 1, year - 1900};
        std::tm b = {0, 0, 0, other.day, other.month - 1, other.year - 1900};
        return std::difftime(std::mktime(&a), std::mktime(&b)) / (60 * 60 * 24);
    }

    int dayOfWeek() const {
        std::tm t = {0, 0, 0, day, month - 1, year - 1900};
        std::mktime(&t);
        return t.tm_wday == 0 ? 7 : t.tm_wday;
    }

    int weekNumber() const {
        std::tm t = {0, 0, 0, day, month - 1, year - 1900};
        std::mktime(&t);
        return t.tm_yday / 7 + 1;
    }

    void setDay(int d) {
        if (d >= 1 && d <= daysInMonth(month, year)) day = d;
    }
    void setMonth(int m) {
        if (m >= 1 && m <= 12) month = m;
    }
    void setYear(int y) {
        if (y >= 1) year = y;
    }

    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }

    std::string toString() const {
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << day << "."
            << std::setw(2) << month << "."
            << year;
        return oss.str();
    }

    static Date fromString(const std::string& str) {
        int d, m, y;
        char dot1, dot2;
        std::istringstream iss(str);
        if (!(iss >> d >> dot1 >> m >> dot2 >> y) || dot1 != '.' || dot2 != '.') {
            throw std::invalid_argument("Invalid string format for Date.");
        }
        return Date(d, m, y);
    }

    static int getCurrentCount() { return currentCount; }
    static int getTotalCreated() { return totalCreated; }

    void print() const {
        std::cout << toString() << std::endl;
    }
};

int Date::currentCount = 0;
int Date::totalCreated = 0;

// --- Клас Vector2D ---
class Vector2D {
private:
    int x, y;
public:
    Vector2D(int x = 0, int y = 0) : x(x), y(y) {}

    void print() const {
        std::cout << "Vector(" << x << ", " << y << ")" << std::endl;
    }

    double length() const {
        return std::sqrt(x * x + y * y);
    }

    void multiply(int scalar) {
        x *= scalar;
        y *= scalar;
    }
};

// --- Дружній інтерфейс ---
void dateMenu() {
    Date date;
    std::cout << "Створено дату за замовчуванням: ";
    date.print();

    int choice;
    do {
        std::cout << "\n=== Меню роботи з Date ===\n";
        std::cout << "1. Вивести поточну дату\n";
        std::cout << "2. Змінити дату (ручне введення)\n";
        std::cout << "3. Додати дні\n";
        std::cout << "4. Знайти різницю з іншою датою\n";
        std::cout << "5. День тижня\n";
        std::cout << "6. Номер тижня\n";
        std::cout << "7. Вивести як рядок\n";
        std::cout << "8. Зчитати з рядка\n";
        std::cout << "9. Показати лічильники об'єктів\n";
        std::cout << "0. Повернутись до головного меню\n";
        std::cout << "Ваш вибір: ";
        std::cin >> choice;

        if (choice == 1) {
            date.print();
        } else if (choice == 2) {
            int d, m, y;
            std::cout << "Введіть день, місяць і рік: ";
            std::cin >> d >> m >> y;
            try {
                date = Date(d, m, y);
                std::cout << "Дата оновлена: ";
                date.print();
            } catch (const std::exception& e) {
                std::cout << "Помилка: " << e.what() << std::endl;
            }
        } else if (choice == 3) {
            int days;
            std::cout << "Скільки днів додати? ";
            std::cin >> days;
            date = date + days;
            date.print();
        } else if (choice == 4) {
            std::string input;
            std::cout << "Введіть іншу дату (dd.mm.yyyy): ";
            std::cin >> input;
            try {
                Date other = Date::fromString(input);
                std::cout << "Різниця: " << (date - other) << " днів\n";
            } catch (...) {
                std::cout << "Некоректна дата.\n";
            }
        } else if (choice == 5) {
            std::cout << "День тижня (1-Пн ... 7-Нд): " << date.dayOfWeek() << std::endl;
        } else if (choice == 6) {
            std::cout << "Номер тижня: " << date.weekNumber() << std::endl;
        } else if (choice == 7) {
            std::cout << "Рядок: " << date.toString() << std::endl;
        } else if (choice == 8) {
            std::string str;
            std::cout << "Введіть рядок дати (dd.mm.yyyy): ";
            std::cin >> str;
            try {
                date = Date::fromString(str);
                std::cout << "Дата зчитана: ";
                date.print();
            } catch (...) {
                std::cout << "Некоректний формат.\n";
            }
        } else if (choice == 9) {
            std::cout << "Поточних об'єктів: " << Date::getCurrentCount() << std::endl;
            std::cout << "Створено всього: " << Date::getTotalCreated() << std::endl;
        }
    } while (choice != 0);
}

void vectorMenu() {
    Vector2D vec;
    int choice;
    do {
        std::cout << "\n=== Меню роботи з Vector2D ===\n";
        std::cout << "1. Задати координати\n";
        std::cout << "2. Вивести вектор\n";
        std::cout << "3. Довжина вектора\n";
        std::cout << "4. Множення на число\n";
        std::cout << "0. Повернутись до головного меню\n";
        std::cout << "Ваш вибір: ";
        std::cin >> choice;

        if (choice == 1) {
            int x, y;
            std::cout << "Введіть x і y: ";
            std::cin >> x >> y;
            vec = Vector2D(x, y);
        } else if (choice == 2) {
            vec.print();
        } else if (choice == 3) {
            std::cout << "Довжина: " << vec.length() << std::endl;
        } else if (choice == 4) {
            int k;
            std::cout << "На яке число множити? ";
            std::cin >> k;
            vec.multiply(k);
            vec.print();
        }
    } while (choice != 0);
}

int main() {
    int choice;
    do {
        std::cout << "\n=== ГОЛОВНЕ МЕНЮ ===\n";
        std::cout << "1. Робота з класом Date\n";
        std::cout << "2. Робота з класом Vector2D\n";
        std::cout << "0. Вихід\n";
        std::cout << "Ваш вибір: ";
        std::cin >> choice;

        switch (choice) {
            case 1: dateMenu(); break;
            case 2: vectorMenu(); break;
        }
    } while (choice != 0);

    std::cout << "Дякую за використання програми!\n";
    return 0;
}
