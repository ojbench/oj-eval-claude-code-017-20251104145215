#ifndef UTILS_H
#define UTILS_H

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <time.h>

const int MAX_STRING_LEN = 256;
const int MAX_USERS = 1000;
const int MAX_TRAINS = 1000;
const int MAX_ORDERS = 10000;
const int MAX_STATIONS = 100;

struct Date {
    int month, day;

    Date() : month(0), day(0) {}
    Date(int m, int d) : month(m), day(d) {}

    bool operator<(const Date& other) const {
        if (month != other.month) return month < other.month;
        return day < other.day;
    }

    bool operator<=(const Date& other) const {
        if (month != other.month) return month < other.month;
        return day <= other.day;
    }

    bool operator==(const Date& other) const {
        return month == other.month && day == other.day;
    }

    bool operator>(const Date& other) const {
        if (month != other.month) return month > other.month;
        return day > other.day;
    }
};

struct Time {
    int hour, minute;

    Time() : hour(0), minute(0) {}
    Time(int h, int m) : hour(h), minute(m) {}

    bool operator<(const Time& other) const {
        if (hour != other.hour) return hour < other.hour;
        return minute < other.minute;
    }

    Time operator+(int minutes) const {
        Time result = *this;
        result.minute += minutes;
        result.hour += result.minute / 60;
        result.minute %= 60;
        result.hour %= 24;
        return result;
    }
};

inline int parseInt(const char* str) {
    return atoi(str);
}

inline bool isValidUsername(const char* str) {
    if (strlen(str) == 0 || strlen(str) > 20) return false;
    if (!isalpha(str[0])) return false;
    for (int i = 0; str[i]; i++) {
        if (!isalnum(str[i]) && str[i] != '_') return false;
    }
    return true;
}

inline bool isValidPassword(const char* str) {
    int len = strlen(str);
    if (len < 6 || len > 30) return false;
    for (int i = 0; str[i]; i++) {
        if (!isalnum(str[i]) && str[i] != '_') return false;
    }
    return true;
}

inline bool isValidName(const char* str) {
    int len = strlen(str);
    if (len < 6 || len > 10) return false; // 2-5 Chinese characters, each 3 bytes in UTF-8
    // Simple check for Chinese characters (basic range)
    for (int i = 0; str[i]; i++) {
        if ((unsigned char)str[i] >= 0x80) continue; // Chinese character byte
        if (!isalnum(str[i]) && str[i] != '_') return false;
    }
    return true;
}

inline bool isValidEmail(const char* str) {
    int len = strlen(str);
    if (len == 0 || len > 30) return false;
    bool hasAt = false, hasDot = false;
    for (int i = 0; str[i]; i++) {
        if (str[i] == '@') {
            if (hasAt) return false;
            hasAt = true;
        } else if (str[i] == '.') {
            hasDot = true;
        } else if (!isalnum(str[i]) && str[i] != '@' && str[i] != '.') {
            return false;
        }
    }
    return hasAt && hasDot;
}

inline Date parseDate(const char* str) {
    int month = parseInt(str);
    int day = parseInt(str + 3);
    return Date(month, day);
}

inline Time parseTime(const char* str) {
    int hour = parseInt(str);
    int minute = parseInt(str + 3);
    return Time(hour, minute);
}

inline int dateDiff(const Date& d1, const Date& d2) {
    // Simple calculation assuming same year (2021)
    int days1 = d1.month * 30 + d1.day;
    int days2 = d2.month * 30 + d2.day;
    return days2 - days1;
}

#endif // UTILS_H