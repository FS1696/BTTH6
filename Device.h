/****************
Mã sinh viên: 202419074
Họ tên: Vũ Thành Lâm
****************/
#ifndef DEVICE_H
#define DEVICE_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <ctime>

enum class DeviceStatus {
    Available,
    Borrowed,
    UnderMaintenance,
    Retired
};

inline std::string StatusToString(DeviceStatus status) {
    switch (status) {
        case DeviceStatus::Available: return "Available";
        case DeviceStatus::Borrowed: return "Borrowed";
        case DeviceStatus::UnderMaintenance: return "UnderMaintenance";
        case DeviceStatus::Retired: return "Retired";
        default: return "Unknown";
    }
}

// Lop truu tuong mo ta thong tin chung cua thiet bi.
class Device {
private:
    const std::string deviceId; // Ma chi duoc gan khi khoi tao.
protected:
    std::string name;
    int yearIntroduced;
    double purchasePrice;
    DeviceStatus status;
    int maxBorrowDays;

    static int CurrentYear() {
        std::time_t now = std::time(nullptr);
        std::tm* local = std::localtime(&now);
        return local ? local->tm_year + 1900 : 2026;
    }

public:
    Device(const std::string& id, const std::string& deviceName,
           int year, double price, int maxDays,
           DeviceStatus initialStatus = DeviceStatus::Available)
        : deviceId(id), name(deviceName), yearIntroduced(year),
          purchasePrice(price), status(initialStatus), maxBorrowDays(maxDays) {
        if (deviceId.empty()) throw std::invalid_argument("Ma thiet bi khong duoc rong.");
        if (name.empty()) throw std::invalid_argument("Ten thiet bi khong duoc rong.");
        if (purchasePrice <= 0) throw std::invalid_argument("Gia mua phai lon hon 0.");
        if (yearIntroduced > CurrentYear()) throw std::invalid_argument("Nam su dung khong duoc lon hon nam hien tai.");
        if (maxBorrowDays <= 0) throw std::invalid_argument("So ngay muon toi da phai lon hon 0.");
    }

    virtual ~Device() = default;

    const std::string& GetId() const { return deviceId; }
    const std::string& GetName() const { return name; }
    int GetYearIntroduced() const { return yearIntroduced; }
    double GetPurchasePrice() const { return purchasePrice; }
    DeviceStatus GetStatus() const { return status; }
    int GetMaxBorrowDays() const { return maxBorrowDays; }

    void SetStatus(DeviceStatus newStatus) { status = newStatus; }

    // Lop con tu cai dat cong thuc chi phi bao tri.
    virtual double CalculateAnnualMaintenanceCost() const = 0;

    virtual std::string ToString() const {
        return "Ma: " + deviceId + " | Ten: " + name +
               " | Nam su dung: " + std::to_string(yearIntroduced) +
               " | Gia mua: " + std::to_string(purchasePrice) +
               " | Trang thai: " + StatusToString(status) +
               " | So ngay muon toi da: " + std::to_string(maxBorrowDays);
    }
};

class AirConditionerRemote : public Device {
public:
    AirConditionerRemote(const std::string& id, int year, double price,
                         int maxDays = 1,
                         DeviceStatus status = DeviceStatus::Available)
        : Device(id, "Dieu khien dieu hoa", year, price, maxDays, status) {}

    double CalculateAnnualMaintenanceCost() const override {
        return GetPurchasePrice() * 0.05;
    }
};

class Microphone : public Device {
public:
    Microphone(const std::string& id, int year, double price,
               int maxDays = 1,
               DeviceStatus status = DeviceStatus::Available)
        : Device(id, "Micro khong day", year, price, maxDays, status) {}

    double CalculateAnnualMaintenanceCost() const override {
        return GetPurchasePrice() * 0.08;
    }
};

class ProjectorRemote : public Device {
public:
    ProjectorRemote(const std::string& id, int year, double price,
                    int maxDays = 2,
                    DeviceStatus status = DeviceStatus::Available)
        : Device(id, "Dieu khien may chieu", year, price, maxDays, status) {}

    double CalculateAnnualMaintenanceCost() const override {
        return GetPurchasePrice() * 0.06;
    }
};

#endif
