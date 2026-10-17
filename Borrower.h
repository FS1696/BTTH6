/****************
Mã sinh viên: 202419074
Họ tên: Vũ Thành Lâm
****************/
#ifndef BORROWER_H
#define BORROWER_H

#include <string>
#include <stdexcept>

// C++ khong co interface rieng; dung lop truu tuong co ham thuan ao.
class IBorrower {
public:
    virtual ~IBorrower() = default;
    virtual const std::string& GetBorrowerId() const = 0;
    virtual const std::string& GetFullName() const = 0;
    virtual int GetMaximumDevices() const = 0;
};

class Lecturer : public IBorrower {
private:
    std::string borrowerId, fullName;
public:
    Lecturer(const std::string& id, const std::string& name)
        : borrowerId(id), fullName(name) {
        if (id.empty() || name.empty()) throw std::invalid_argument("Thong tin giang vien khong hop le.");
    }
    const std::string& GetBorrowerId() const override { return borrowerId; }
    const std::string& GetFullName() const override { return fullName; }
    int GetMaximumDevices() const override { return 3; }
};

class TeachingAssistant : public IBorrower {
private:
    std::string borrowerId, fullName;
public:
    TeachingAssistant(const std::string& id, const std::string& name)
        : borrowerId(id), fullName(name) {
        if (id.empty() || name.empty()) throw std::invalid_argument("Thong tin tro giang khong hop le.");
    }
    const std::string& GetBorrowerId() const override { return borrowerId; }
    const std::string& GetFullName() const override { return fullName; }
    int GetMaximumDevices() const override { return 2; }
};

#endif
