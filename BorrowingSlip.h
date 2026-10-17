/****************
Mã sinh viên: 202419074
Họ tên: Vũ Thành Lâm
****************/
#ifndef BORROWINGSLIP_H
#define BORROWINGSLIP_H

#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <ctime>
#include "Borrower.h"
#include "BorrowItem.h"
#include "Exceptions.h"

class BorrowingSlip {
private:
    std::string slipId;
    std::shared_ptr<IBorrower> borrower;
    std::string teachingRoom;
    std::tm borrowedDate;
    std::vector<BorrowItem> items;
    static constexpr double LATE_FEE_PER_DAY = 10000.0; // Gia dinh: 10.000 dong/ngay.

public:
    BorrowingSlip(const std::string& id, const std::shared_ptr<IBorrower>& person,
                  const std::string& room, const std::tm& date)
        : slipId(id), borrower(person), teachingRoom(room), borrowedDate(date) {
        if (slipId.empty() || teachingRoom.empty() || !borrower)
            throw std::invalid_argument("Ma phieu, nguoi muon va phong day phai hop le.");
    }

    const std::string& GetSlipId() const { return slipId; }
    const std::string& GetTeachingRoom() const { return teachingRoom; }
    const std::tm& GetBorrowedDate() const { return borrowedDate; }
    const std::vector<BorrowItem>& GetItems() const { return items; }

    // Kiem tra tat ca dieu kien truoc khi thay doi trang thai thiet bi/phieu.
    void AddDevice(const std::shared_ptr<Device>& device, int borrowDays) {
        if (!device) throw std::invalid_argument("Thiet bi khong duoc null.");
        if (borrowDays <= 0 || borrowDays > device->GetMaxBorrowDays())
            throw InvalidBorrowPeriodException(device->GetId(), borrowDays, device->GetMaxBorrowDays());

        auto duplicate = std::find_if(items.begin(), items.end(),
            [&](const BorrowItem& item) { return item.GetDevice()->GetId() == device->GetId(); });
        if (duplicate != items.end()) throw DuplicateDeviceException(device->GetId());

        if (static_cast<int>(items.size()) >= borrower->GetMaximumDevices())
            throw BorrowLimitExceededException(borrower->GetBorrowerId(), borrower->GetMaximumDevices());

        if (device->GetStatus() != DeviceStatus::Available)
            throw DeviceUnavailableException(device->GetId(), device->GetStatus());

        std::tm dueDate = borrowedDate;
        dueDate.tm_mday += borrowDays;
        dueDate.tm_isdst = -1;
        std::mktime(&dueDate);

        // Tao item truoc; neu tao that bai thi trang thai thiet bi chua bi doi.
        BorrowItem item(device, borrowedDate, dueDate);
        items.push_back(item);
        device->SetStatus(DeviceStatus::Borrowed);
    }

    double ReturnDevice(const std::string& deviceId, const std::tm& returnedDate) {
        auto it = std::find_if(items.begin(), items.end(),
            [&](const BorrowItem& item) { return item.GetDevice()->GetId() == deviceId; });
        if (it == items.end()) throw DeviceNotInSlipException(deviceId);
        if (it->IsReturned()) throw DeviceAlreadyReturnedException(deviceId);
        if (DaysBetween(it->GetBorrowedDate(), returnedDate) < 0)
            throw InvalidReturnDateException();

        int lateDays = it->GetLateDays(returnedDate);
        // Chi cap nhat item va trang thai sau khi cac dieu kien da hop le.
        it->Return(returnedDate);
        it->GetDevice()->SetStatus(DeviceStatus::Available);
        return lateDays * LATE_FEE_PER_DAY;
    }

    double CalculateTotalLateFee() const {
        double total = 0;
        for (const auto& item : items) {
            if (item.IsReturned())
                total += item.GetLateDays(item.GetReturnedDate()) * LATE_FEE_PER_DAY;
        }
        return total;
    }

    void Print() const {
        std::cout << "Phieu muon: " << slipId
                  << " | Nguoi muon: " << borrower->GetFullName()
                  << " | Phong: " << teachingRoom
                  << " | Ngay muon: " << DateToString(borrowedDate) << '\n';
        for (const auto& item : items) {
            std::cout << "  - " << item.GetDevice()->GetName()
                      << " [" << item.GetDevice()->GetId() << "]"
                      << " | Han tra: " << DateToString(item.GetDueDate())
                      << " | Trang thai: " << (item.IsReturned() ? "Da tra" : "Chua tra") << '\n';
        }
    }
};

#endif
