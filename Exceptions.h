/****************
Mã sinh viên: 202419074
Họ tên: Vũ Thành Lâm
****************/
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>
#include "Device.h"

// Ngoai le co so cho cac loi nghiep vu muon/thiet bi.
class BorrowingException : public std::runtime_error {
public:
    explicit BorrowingException(const std::string& message)
        : std::runtime_error(message) {}
};

class DeviceUnavailableException : public BorrowingException {
public:
    std::string deviceId;
    DeviceStatus currentStatus;

    DeviceUnavailableException(const std::string& id, DeviceStatus status)
        : BorrowingException("Thiet bi " + id + " khong san sang. Trang thai: " + StatusToString(status)),
          deviceId(id), currentStatus(status) {}
};

class BorrowLimitExceededException : public BorrowingException {
public:
    std::string borrowerId;
    int maximumDevices;

    BorrowLimitExceededException(const std::string& id, int maximum)
        : BorrowingException("Nguoi muon " + id + " da dat gioi han " + std::to_string(maximum) + " thiet bi."),
          borrowerId(id), maximumDevices(maximum) {}
};

class InvalidBorrowPeriodException : public BorrowingException {
public:
    std::string deviceId;
    int requestedDays;
    int maximumDays;

    InvalidBorrowPeriodException(const std::string& id, int requested, int maximum)
        : BorrowingException("So ngay muon khong hop le cho thiet bi " + id +
                             ". Yeu cau: " + std::to_string(requested) +
                             ", toi da: " + std::to_string(maximum) + "."),
          deviceId(id), requestedDays(requested), maximumDays(maximum) {}
};

class DuplicateDeviceException : public BorrowingException {
public:
    explicit DuplicateDeviceException(const std::string& id)
        : BorrowingException("Thiet bi " + id + " da co trong phieu muon.") {}
};

class DeviceNotInSlipException : public BorrowingException {
public:
    explicit DeviceNotInSlipException(const std::string& id)
        : BorrowingException("Thiet bi " + id + " khong thuoc phieu muon.") {}
};

class DeviceAlreadyReturnedException : public BorrowingException {
public:
    explicit DeviceAlreadyReturnedException(const std::string& id)
        : BorrowingException("Thiet bi " + id + " da duoc tra truoc do.") {}
};

class InvalidReturnDateException : public BorrowingException {
public:
    InvalidReturnDateException()
        : BorrowingException("Ngay tra khong duoc truoc ngay muon.") {}
};

#endif
