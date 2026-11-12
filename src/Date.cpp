#include "../include/Date.h"
using namespace std;

Date::Date(int day, int month, int year) : day(day), month(month), year(year) {}

Date::Date(const Date& other) : day(other.day), month(other.month), year(other.year) {}

Date& Date::operator=(const Date& other) {
    if (this != &other) {
        day = other.day;
        month = other.month;
        year = other.year;
    }
    return *this;
}

int Date::getDay() const {
    return day;
}

int Date::getMonth() const {
    return month;
}

int Date::getYear() const {
    return year;
}

void Date::setDay(int day) {
    this->day = day;
}

void Date::setMonth(int month) {
    this->month = month;
}

void Date::setYear(int year) {
    this->year = year;
}

bool Date::isValid() const {
    if (year < 1 || month < 1 || month > 12 || day < 1) {
        return false;
    }

    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        daysInMonth[1] = 29; // Leap year
    }

    return day <= daysInMonth[month - 1];
}

bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

bool Date::operator==(const Date& other) const {
    return day == other.day && month == other.month && year == other.year;
}

bool Date::operator>(const Date& other) const {
    return !(*this < other) && !(*this == other);
}
