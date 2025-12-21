/*
#include <iostream>
#include <string>
#include <cstdio> // De dung ham remove xoa file

// Include cac file can thiet
#include "Controllers/AppController.h"
#include "Models/Wallet.h"
#include "Models/Transaction.h"
#include "Utils/Date.h"

using namespace std;

// Ham xoa sach data cu de test khong bi trung
void LamSachData() {
    remove("data/wallets.bin");
    remove("data/categories.bin");
    remove("data/sources.bin");
    remove("data/transactions.bin");
    remove("data/recurring.bin");
}

// ---------------------------------------------------------
// TEST CASE 1: Tao vi moi
// Muc tieu: Kiem tra vi tao ra co dung so tien ban dau khong
// ---------------------------------------------------------
bool Test_TaoVi() {
    cout << "\n[TEST 1] Thu nghiem tao Vi moi..." << endl;
    
    // Khoi tao Controller (View = nullptr vi khong can in ra man hinh)
    AppController app(nullptr);
    
    // Tao vi ten "Vi Test" voi 1 trieu
    app.AddWallet("Vi Test", 1000000);
    
    // Kiem tra xem list co phan tu nao khong
    if (app.GetWalletsList()->Count() == 0) {
        cout << "-> LOI: Khong tao duoc vi!" << endl;
        return false;
    }

    // Lay vi vua tao ra check
    Wallet* w = app.GetWalletsList()->Get(0);
    if (w->GetBalance() == 1000000) {
        cout << "-> PASS: Tao vi thanh cong, so du dung." << endl;
        return true;
    } else {
        cout << "-> LOI: So du sai! Ky vong 1000000, thuc te: " << w->GetBalance() << endl;
        return false;
    }
}

// ---------------------------------------------------------
// TEST CASE 2: Them giao dich va cap nhat so du
// Muc tieu: Them Expense -> Vi phai bi tru tien
// ---------------------------------------------------------
bool Test_GiaoDich() {
    cout << "\n[TEST 2] Thu nghiem them Giao dich (Expense)..." << endl;
    AppController app(nullptr);
    
    // Setup du lieu: 1 Vi, 1 Category
    app.AddWallet("Vi Chinh", 500000); // Co 500k
    app.AddCategory("An uong");
    
    // Lay ID cua vi va category vua tao
    string walletId = app.GetWalletsList()->Get(0)->GetId();
    string catId = app.GetCategoriesList()->Get(0)->GetId();
    Date today(20, 12, 2025);

    // Them giao dich chi tieu 100k
    // Ham: AddTransaction(amount, walletId, catId, type, date, desc)
    app.AddTransaction(100000, walletId, catId, TransactionType::Expense, today, "An sang");

    // Kiem tra so du sau khi tru
    Wallet* w = app.GetWalletById(walletId);
    double soDuHienTai = w->GetBalance();
    
    // 500k - 100k phai con 400k
    if (soDuHienTai == 400000) {
        cout << "-> PASS: So du cap nhat dung (500k - 100k = 400k)." << endl;
        return true;
    } else {
        cout << "-> LOI: Tinh toan sai! So du hien tai: " << soDuHienTai << endl;
        return false;
    }
}

// ---------------------------------------------------------
// TEST CASE 3: Rang buoc toan ven (Data Integrity)
// Muc tieu: Khong cho xoa Category neu dang co giao dich dung no
// ---------------------------------------------------------
bool Test_RangBuocXoa() {
    cout << "\n[TEST 3] Thu nghiem xoa Category dang su dung..." << endl;
    AppController app(nullptr);
    
    // Setup: 1 Vi, 1 Cat, 1 Trans dung Cat do
    app.AddWallet("Vi Demo", 100000);
    app.AddCategory("Mua sam");
    
    string wId = app.GetWalletsList()->Get(0)->GetId();
    string cId = app.GetCategoriesList()->Get(0)->GetId();
    Date d(20, 12, 2025);
    
    app.AddTransaction(50000, wId, cId, TransactionType::Expense, d, "Mua ao");
    
    // Thu xoa Category "Mua sam"
    bool ketQuaXoa = app.DeleteCategory(cId);
    
    // Ky vong: ketQuaXoa phai la FALSE (khong cho xoa)
    if (ketQuaXoa == false) {
        cout << "-> PASS: He thong da chan xoa Category dang co giao dich." << endl;
        return true;
    } else {
        cout << "-> LOI: He thong cho phep xoa Category dang su dung -> Sai logic!" << endl;
        return false;
    }
}

// ---------------------------------------------------------
// MAIN TEST RUNNER
// ---------------------------------------------------------
int main() {
    cout << "=== BAT DAU CHAY UNIT TEST (SINH VIEN TU VIET) ===" << endl;
    
    // Xoa data cu truoc khi test
    LamSachData();

    int passCount = 0;
    int totalTests = 3;

    if (Test_TaoVi()) passCount++;
    
    // Xoa data de test 2 chay doc lap
    LamSachData();
    if (Test_GiaoDich()) passCount++;

    // Xoa data de test 3 chay doc lap
    LamSachData();
    if (Test_RangBuocXoa()) passCount++;
    
    cout << "\n========================================" << endl;
    cout << "KET QUA TONG HOP: " << passCount << "/" << totalTests << " bai test dat yeu cau." << endl;
    
    if (passCount == totalTests) {
        cout << "DANH GIA: OK, san sang nop bai!" << endl;
    } else {
        cout << "DANH GIA: Con loi, can fix gap!" << endl;
    }

    // Dung man hinh de xem ket qua
    cout << "Nhan Enter de thoat...";
    cin.get();
    
    return 0;
}
*/