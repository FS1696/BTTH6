/****************
Mã sinh viên: 202419074
Họ tên: Vũ Thành Lâm
****************/
#ifndef BORROWITEM_H
#define BORROWITEM_H

#include <memory>
#include <string>
#include <ctime>
#include "Device.h"
#include "Exceptions.h"

inline std::time_t ToTimeT(const std::tm& date) {
    std::tm copy = date;
    copy.tm_hour = 12; copy.tm_min = 0; copy.tm_sec = 0;
    return std::mktime(&copy);
}

inline std::tm MakeDate(int year, int month, int day) {
    std::tm date = {};
    date.tm_year = year - 1900;
    date.tm_mon = month - 1;
    date.tm_mday = day;
    date.tm_isdst = -1;
    std::mktime(&date); // Chuan hoa ngay/thang.
    return date;
}

inline std::string DateToString(const std::tm& date) {
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%d/%m/%Y", &date);
    return buffer;
}

inline int DaysBetween(const std::tm& start, const std::tm& end) {
    double seconds = std::difftime(ToTimeT(end), ToTimeT(start));
    return static_cast<int>(seconds / (60 * 60 * 24));
}

class BorrowItem {
private:
    std::shared_ptr<Device> device;
    std::tm borrowedDate;
    std::tm dueDate;
    std::tm fromPeriodDate;
    std::tm toPeriodDate;
    bool returned;
    std::tm returnedDate;

public:
    BorrowItem(const std::shared_ptr<Device>& d, const std::tm& borrowed,
               const std::tm& due)
        : device(d), borrowedDate(borrowed), dueDate(due),
          fromPeriodDate(borrowed), toPeriodDate(due), returned(false), returnedDate{} {
        if (!device) throw std::invalid_argument("Thiet bi trong BorrowItem khong duoc null.");
        if (DaysBetween(borrowedDate, dueDate) < 0)
            throw std::invalid_argument("Han tra phai sau hoac bang ngay muon.");
    }

    const std::shared_ptr<Device>& GetDevice() const { return device; }
    const std::tm& GetBorrowedDate() const { return borrowedDate; }
    const std::tm& GetDueDate() const { return dueDate; }
    bool IsReturned() const { return returned; }
    const std::tm& GetReturnedDate() const { return returnedDate; }

    void Return(const std::tm& date) {
        if (returned) throw DeviceAlreadyReturnedException(device->GetId());
        if (DaysBetween(borrowedDate, date) < 0) throw InvalidReturnDateException();
        returnedDate = date;
        returned = true;
    }

    int GetLateDays(const std::tm& date) const {
        int lateDays = DaysBetween(dueDate, date);
        return lateDays > 0 ? lateDays : 0;
    }
};

#endif
