/****************
Ma sinh vien: 202419074
Ho ten: Vũ Thành Lâm
****************/

#include <iostream>
#include <memory>
#include <vector>
#include <exception>
#include <string>
#include <ctime>

#include "Device.h"
#include "Exceptions.h"
#include "ClassRoom.h"
#include "Borrower.h"
#include "BorrowItem.h"
#include "BorrowingSlip.h"

using namespace std;

// Chay phep thu loi; chuong trinh tiep tuc sau khi bat ngoai le.
template <typename Function>
void TestException(const string& title, Function action) {
    cout << "\n[KIEM THU NGOAI LE] " << title << '\n';

    try {
        action();
        cout << "Khong phat sinh loi.\n";
    }
    catch (const BorrowingException& ex) {
        cout << "Bat ngoai le nghiep vu: " << ex.what() << '\n';
    }
    catch (const exception& ex) {
        cout << "Bat ngoai le: " << ex.what() << '\n';
    }

    cout << "Ket thuc phep thu.\n";
}

int main() {
    try {
        // Ngay kiem thu co dinh de ket qua co the lap lai.
        tm borrowDate = MakeDate(2026, 10, 17);
        tm onTimeDate = MakeDate(2026, 10, 19);
        tm lateDate = MakeDate(2026, 10, 20);
        tm earlierDate = MakeDate(2026, 10, 16);

        // ==================================================
        // PHAN I: 7 TINH HUONG KIEM THU BINH THUONG
        // ==================================================

        cout << "============================================\n";
        cout << " PHAN I: KIEM THU LUONG THANH CONG\n";
        cout << "============================================\n";

        // 1. Tao mot giang vien.
        auto lecturer = make_shared<Lecturer>(
            "GV001", "Nguyen Van An"
        );

        cout << "\n[1] TAO GIANG VIEN\n";
        cout << "Ma giang vien: GV001\n";
        cout << "Ho ten: Nguyen Van An\n";

        // 2. Tao dieu khien may chieu, dieu khien dieu hoa,
        //    va micro khong day.
        auto projector = make_shared<ProjectorRemote>(
            "DC001", 2023, 500000, 2
        );

        auto airRemote = make_shared<AirConditionerRemote>(
            "DH001", 2022, 350000, 1
        );

        auto microphone = make_shared<Microphone>(
            "MIC001", 2024, 800000, 1
        );

        cout << "\n[2] TAO BA THIET BI\n";
        cout << projector->ToString() << '\n';
        cout << airRemote->ToString() << '\n';
        cout << microphone->ToString() << '\n';

        // Tao them cac thiet bi phuc vu kiem thu ngoai le.
        auto maintenanceRemote = make_shared<AirConditionerRemote>(
            "DH003", 2022, 350000, 1,
            DeviceStatus::UnderMaintenance
        );

        auto thirdDevice = make_shared<AirConditionerRemote>(
            "DH002", 2025, 300000, 1
        );

        // Tao phong hoc va quan ly cac thiet bi.
        ClassRoom room("P101", "Phong hoc 101", 60);

        room.AddDevice(projector);
        room.AddDevice(airRemote);
        room.AddDevice(microphone);
        room.AddDevice(maintenanceRemote);
        room.AddDevice(thirdDevice);

        // 3. Tao phieu muon cho mot phong day.
        BorrowingSlip slip(
            "PM001", lecturer, "P101", borrowDate
        );

        cout << "\n[3] TAO PHIEU MUON\n";
        cout << "Ma phieu: PM001\n";
        cout << "Phong day: P101\n";

        // 4. Them dieu khien may chieu va micro vao phieu.
        slip.AddDevice(projector, 2);
        slip.AddDevice(microphone, 1);

        cout << "\n[4] THEM THIET BI VAO PHIEU MUON\n";
        slip.Print();

        // 5. Tra dieu khien may chieu dung han.
        cout << "\n[5] TRA DIEU KHIEN MAY CHIEU DUNG HAN\n";
        cout << "Ma thiet bi: DC001\n";
        cout << "Ngay tra: 19/10/2026\n";
        cout << "Phi tra muon: "
             << slip.ReturnDevice("DC001", onTimeDate)
             << " dong\n";

        // 6. Tra micro muon 2 ngay.
        cout << "\n[6] TRA MICRO MUON 2 NGAY\n";
        cout << "Ma thiet bi: MIC001\n";
        cout << "Ngay tra: 20/10/2026\n";
        cout << "Phi tra muon: "
             << slip.ReturnDevice("MIC001", lateDate)
             << " dong\n";

        cout << "Tong phi tra muon cua PM001: "
             << slip.CalculateTotalLateFee() << " dong\n";

        // Tao phieu muon thu hai co thiet bi chua duoc tra.
        auto unreturnedMic = make_shared<Microphone>(
            "MIC002", 2024, 800000, 1
        );

        BorrowingSlip slip2(
            "PM002", lecturer, "P101", borrowDate
        );

        slip2.AddDevice(unreturnedMic, 1);

        // 7. Hien thi phieu co thiet bi tra muon va chua tra.
        cout << "\n[7] HIEN THI PHIEU TRA MUON VA CHUA TRA\n";

        cout << "\n--- PHIEU PM001: CAC THIET BI DA DUOC TRA ---\n";
        slip.Print();

        cout << "\n--- PHIEU PM002: THIET BI CHUA DUOC TRA ---\n";
        slip2.Print();

        cout << "\nTong phi tra muon cua PM001: "
             << slip.CalculateTotalLateFee() << " dong\n";

        // ==================================================
        // PHAN II: 7 TINH HUONG KIEM THU NGOAI LE
        // ==================================================

        cout << "\n============================================\n";
        cout << " PHAN II: KIEM THU CAC TINH HUONG NGOAI LE\n";
        cout << "============================================\n";

        // 1. Muon thiet bi dang bao tri.
        TestException(
            "Muon dieu khien dieu hoa dang UnderMaintenance",
            [&]() {
                BorrowingSlip s(
                    "PM003", lecturer, "P101", borrowDate
                );

                s.AddDevice(maintenanceRemote, 1);
            }
        );

        // 2. Tro giang muon thiet bi thu ba.
        auto assistant = make_shared<TeachingAssistant>(
            "TG001", "Tran Thi Binh"
        );

        TestException(
            "Tro giang muon thiet bi thu ba",
            [&]() {
                BorrowingSlip s(
                    "PM004", assistant, "P101", borrowDate
                );

                auto d1 = make_shared<ProjectorRemote>(
                    "T1", 2024, 200000, 2
                );

                auto d2 = make_shared<Microphone>(
                    "T2", 2024, 300000, 1
                );

                auto d3 = make_shared<AirConditionerRemote>(
                    "T3", 2024, 250000, 1
                );

                s.AddDevice(d1, 1);
                s.AddDevice(d2, 1);
                s.AddDevice(d3, 1);
            }
        );

        // 3. Them cung mot micro hai lan.
        TestException(
            "Them cung mot thiet bi hai lan",
            [&]() {
                BorrowingSlip s(
                    "PM005", lecturer, "P101", borrowDate
                );

                auto mic = make_shared<Microphone>(
                    "MIC-DUP", 2024, 400000, 1
                );

                s.AddDevice(mic, 1);
                s.AddDevice(mic, 1);
            }
        );

        // 4. Muon micro 3 ngay trong khi gioi han la 1 ngay.
        TestException(
            "Muon micro vuot qua so ngay toi da",
            [&]() {
                BorrowingSlip s(
                    "PM006", lecturer, "P101", borrowDate
                );

                auto mic = make_shared<Microphone>(
                    "MIC-LONG", 2024, 400000, 1
                );

                s.AddDevice(mic, 3);
            }
        );

        // 5. Tra thiet bi khong thuoc phieu.
        TestException(
            "Tra thiet bi khong thuoc phieu",
            [&]() {
                BorrowingSlip s(
                    "PM007", lecturer, "P101", borrowDate
                );

                s.ReturnDevice("KHONGTONTAI", onTimeDate);
            }
        );

        // 6. Tra cung mot thiet bi hai lan.
        TestException(
            "Tra cung mot thiet bi hai lan",
            [&]() {
                BorrowingSlip s(
                    "PM008", lecturer, "P101", borrowDate
                );

                auto mic = make_shared<Microphone>(
                    "MIC-RETURN", 2024, 400000, 1
                );

                s.AddDevice(mic, 1);
                s.ReturnDevice("MIC-RETURN", onTimeDate);
                s.ReturnDevice("MIC-RETURN", onTimeDate);
            }
        );

        // 7. Ngay tra truoc ngay muon.
        TestException(
            "Tra thiet bi truoc ngay muon",
            [&]() {
                BorrowingSlip s(
                    "PM009", lecturer, "P101", borrowDate
                );

                auto mic = make_shared<Microphone>(
                    "MIC-DATE", 2024, 400000, 1
                );

                s.AddDevice(mic, 1);
                s.ReturnDevice("MIC-DATE", earlierDate);
            }
        );

        // Hien thi cac thiet bi can bao tri.
        cout << "\n============================================\n";
        cout << " CAC THIET BI CAN BAO TRI\n";
        cout << "============================================\n";

        for (const auto& device :
             room.GetDevicesRequiringMaintenance()) {
            cout << device->ToString() << '\n';
        }

        cout << "\nChuong trinh ket thuc.\n";
    }
    catch (const BorrowingException& ex) {
        cerr << "Loi nghiep vu: " << ex.what() << '\n';
        return 1;
    }
    catch (const exception& ex) {
        cerr << "Loi khoi tao/chay chuong trinh: "
             << ex.what() << '\n';
        return 1;
    }

    return 0;
}
