/****************
Mã sinh viên: 202419074
Họ tên: Vũ Thành Lâm
****************/
#ifndef CLASSROOM_H
#define CLASSROOM_H

#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "Device.h"
#include "Exceptions.h"

// Phong hoc quan ly danh sach thiet bi bang shared_ptr.
class ClassRoom {
private:
    std::string roomId;
    std::string roomName;
    int capacity;
    std::vector<std::shared_ptr<Device>> devices;

public:
    ClassRoom(const std::string& id, const std::string& name, int roomCapacity)
        : roomId(id), roomName(name), capacity(roomCapacity) {
        if (roomId.empty() || roomName.empty())
            throw std::invalid_argument("Ma phong va ten phong khong duoc rong.");
        if (capacity <= 0) throw std::invalid_argument("Suc chua phai lon hon 0.");
    }

    void AddDevice(const std::shared_ptr<Device>& device) {
        if (!device) throw std::invalid_argument("Khong the them thiet bi null.");
        if (FindDevice(device->GetId()))
            throw DuplicateDeviceException(device->GetId());
        devices.push_back(device);
    }

    bool RemoveDevice(const std::string& deviceId) {
        auto it = std::find_if(devices.begin(), devices.end(),
            [&](const std::shared_ptr<Device>& d) { return d->GetId() == deviceId; });
        if (it == devices.end()) return false;
        devices.erase(it);
        return true;
    }

    std::shared_ptr<Device> FindDevice(const std::string& deviceId) const {
        for (const auto& device : devices)
            if (device->GetId() == deviceId) return device;
        return nullptr;
    }

    // Da hinh: goi phuong thuc ao cua tung thiet bi, khong can if/switch theo loai.
    double CalculateAnnualMaintenanceCost() const {
        double total = 0;
        for (const auto& device : devices)
            total += device->CalculateAnnualMaintenanceCost();
        return total;
    }

    std::vector<std::shared_ptr<Device>> GetDevicesRequiringMaintenance() const {
        std::vector<std::shared_ptr<Device>> result;
        int currentYear = DeviceCurrentYearForRoom();
        for (const auto& device : devices) {
            if (device->GetStatus() == DeviceStatus::UnderMaintenance ||
                currentYear - device->GetYearIntroduced() > 5)
                result.push_back(device);
        }
        return result;
    }

    const std::string& GetRoomId() const { return roomId; }
    const std::string& GetRoomName() const { return roomName; }
    int GetCapacity() const { return capacity; }
    const std::vector<std::shared_ptr<Device>>& GetDevices() const { return devices; }

private:
    int DeviceCurrentYearForRoom() const {
        std::time_t now = std::time(nullptr);
        std::tm* local = std::localtime(&now);
        return local ? local->tm_year + 1900 : 2026;
    }
};

#endif
