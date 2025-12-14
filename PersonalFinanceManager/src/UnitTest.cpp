//
//  UnitTest.cpp
//  PersonalFinanceManager
//
//  Created by Le Tran Hoang Long (Quality Assurance)
//  Standard Unit Testing Framework implementation
//

#include <iostream>
#include <string>
#include <cmath>   // abs()
#include <cstdio>  // remove()
#include <vector>

// Include logic cần test
#include "../include/Controllers/AppController.h"
#include "../include/Utils/Date.h"

// --- 1. UTILS & COLORS ---
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

// --- 2. FILE CLEANUP UTILS (Quan trọng cho Test Isolation) ---
void CleanUpDataFiles() {
    // Xóa file cũ để mỗi bài test đều chạy trên data sạch
    std::remove("data/wallets.bin");
    std::remove("data/categories.bin");
    std::remove("data/sources.bin");
    std::remove("data/transactions.bin");
    std::remove("data/recurring.bin");
}

// --- 3. ASSERTION MACROS (Trái tim của Framework) ---
// Dùng Macro để lấy được __LINE__ (số dòng) và __FILE__

int totalTests = 0;
int passedTests = 0;

// So sánh Double (Số tiền)
#define ASSERT_DOUBLE_EQ(expected, actual) \
    do { \
        double diff = std::abs((expected) - (actual)); \
        if (diff > 0.001) { \
            std::cout << RED << "[FAILED] " << __FUNCTION__ << " (Line " << __LINE__ << ")" << RESET << "\n"; \
            std::cout << "  Expected: " << std::fixed << std::setprecision(2) << (expected) << "\n"; \
            std::cout << "  Actual  : " << std::fixed << std::setprecision(2) << (actual) << "\n"; \
            return false; \
        } \
    } while(0)

// So sánh String
#define ASSERT_STRING_EQ(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            std::cout << RED << "[FAILED] " << __FUNCTION__ << " (Line " << __LINE__ << ")" << RESET << "\n"; \
            std::cout << "  Expected: \"" << (expected) << "\"\n"; \
            std::cout << "  Actual  : \"" << (actual) << "\"\n"; \
            return false; \
        } \
    } while(0)

// So sánh Bool (True/False)
#define ASSERT_TRUE(condition, message) \
    do { \
        if (!(condition)) { \
            std::cout << RED << "[FAILED] " << __FUNCTION__ << " (Line " << __LINE__ << ")" << RESET << "\n"; \
            std::cout << "  Message: " << (message) << "\n"; \
            return false; \
        } \
    } while(0)

#define TEST_HEADER(name) \
    std::cout << BLUE << "\n[RUNNING] " << name << "..." << RESET << "\n"; \
    totalTests++;

// ==========================================
// 4. TEST CASES (CÁC BÀI TEST CỤ THỂ)
// ==========================================

// Bài 1: Test Logic Thêm Ví & Số dư ban đầu
bool Test_Wallet_Creation() {
    TEST_HEADER("Test_Wallet_Creation");
    CleanUpDataFiles(); // Reset dữ liệu
    
    AppController app;
    app.AddWallet("Ví Chính", 500000);
    app.AddWallet("Ví Phụ", 100000);

    // Check 1: Số lượng ví
    if (app.GetWalletsList()->Count() != 2) {
        std::cout << RED << "  Count mismatch!" << RESET << "\n";
        return false;
    }

    // Check 2: Thông tin ví đầu tiên
    Wallet* w1 = app.GetWalletsList()->Get(0);
    ASSERT_STRING_EQ("Ví Chính", w1->GetName());
    ASSERT_DOUBLE_EQ(500000.0, w1->GetBalance());
    
    // Check 3: Thêm ví trùng tên (Giả sử logic là cho phép nhưng cảnh báo, hoặc vẫn thêm)
    // Ở đây ta test việc GetWalletById phải chạy đúng
    Wallet* found = app.GetWalletById(w1->GetId());
    ASSERT_TRUE(found != nullptr, "GetWalletById returned null for existing ID");
    
    return true;
}

// Bài 2: Test Cộng trừ tiền (Transaction Logic)
bool Test_Transaction_Balance_Update() {
    TEST_HEADER("Test_Transaction_Balance_Update");
    CleanUpDataFiles();

    AppController app;
    app.AddWallet("TestWallet", 1000000); // 1 Triệu
    
    // Lấy ID tự sinh
    std::string wId = app.GetWalletsList()->Get(0)->GetId();
    
    // Tạo danh mục giả (vì AddTransaction cần ID danh mục)
    app.AddCategory("Food");
    std::string cId = app.GetCategoriesList()->Get(0)->GetId();
    
    app.AddIncomeSource("Salary");
    std::string sId = app.GetIncomeSourcesList()->Get(0)->GetId();

    // 1. Thêm Thu Nhập 500k -> Balance = 1.5tr
    app.AddTransaction(500000, wId, sId, TransactionType::Income, Date::GetTodayDate(), "Salary 1");
    ASSERT_DOUBLE_EQ(1500000.0, app.GetWalletById(wId)->GetBalance());

    // 2. Thêm Chi Tiêu 200k -> Balance = 1.3tr
    app.AddTransaction(200000, wId, cId, TransactionType::Expense, Date::GetTodayDate(), "Lunch");
    ASSERT_DOUBLE_EQ(1300000.0, app.GetWalletById(wId)->GetBalance());
    
    // 3. Check số lượng Transaction
    ASSERT_TRUE(app.GetTransactions()->Count() == 2, "Transaction count incorrect");

    return true;
}

// Bài 3: Test Sửa Giao Dịch (Logic Khó Nhất)
bool Test_Edit_Transaction_Recalculation() {
    TEST_HEADER("Test_Edit_Transaction_Recalculation");
    CleanUpDataFiles();

    AppController app;
    app.AddWallet("Wallet", 0); // Bắt đầu bằng 0
    std::string wId = app.GetWalletsList()->Get(0)->GetId();
    app.AddCategory("Food");
    std::string cId = app.GetCategoriesList()->Get(0)->GetId();

    // Thêm Expense 50k -> Balance = -50k
    app.AddTransaction(50000, wId, cId, TransactionType::Expense, Date::GetTodayDate(), "Pho");
    std::string tId = app.GetTransactions()->Get(0)->GetId();
    
    ASSERT_DOUBLE_EQ(-50000.0, app.GetWalletById(wId)->GetBalance());

    // Sửa Expense: Từ 50k thành 80k
    // Logic đúng: Hoàn lại 50k (về 0), trừ đi 80k -> Balance = -80k
    app.EditTransaction(tId, 80000, Date::GetTodayDate(), "Pho Dac Biet");

    ASSERT_DOUBLE_EQ(-80000.0, app.GetWalletById(wId)->GetBalance());
    
    // Check xem Description đã đổi chưa
    ASSERT_STRING_EQ("Pho Dac Biet", app.GetTransactions()->Get(0)->GetDescription());

    return true;
}

// Bài 4: Test Bảo vệ dữ liệu (Chặn xóa)
bool Test_Delete_Constraint() {
    TEST_HEADER("Test_Delete_Constraint");
    CleanUpDataFiles();

    AppController app;
    app.AddWallet("Main", 100);
    std::string wId = app.GetWalletsList()->Get(0)->GetId();
    
    // 1. Xóa khi chưa có giao dịch -> Phải OK
    // (Lưu ý: Logic xóa ví có thể trả về void hoặc bool, giả sử AppController trả về bool)
    // Nếu trong code của bạn là void, bạn cần sửa AppController trả về bool để test được chỗ này
    // Ở đây tôi giả định bạn đã dùng code tôi gửi (trả về bool)
    
    // Tạo lại ví để test case fail
    app.AddCategory("TestCat");
    std::string cId = app.GetCategoriesList()->Get(0)->GetId();
    app.AddTransaction(10, wId, cId, TransactionType::Expense, Date::GetTodayDate(), "Test");
    
    // 2. Xóa khi ĐÃ có giao dịch -> Phải Fail
    bool result = app.DeleteWallet(wId);
    ASSERT_TRUE(result == false, "Should NOT delete wallet with transactions");
    
    // Ví vẫn phải còn đó
    ASSERT_TRUE(app.GetWalletById(wId) != nullptr, "Wallet should still exist");

    return true;
}

// Bài 5: Test Validate Đầu vào (Edge Cases)
bool Test_Input_Validation() {
    TEST_HEADER("Test_Input_Validation");
    CleanUpDataFiles();
    AppController app;
    
    // Thêm ví tên rỗng
    app.AddWallet("", 100);
    // Kỳ vọng: Không thêm được
    ASSERT_TRUE(app.GetWalletsList()->Count() == 0, "Should not add wallet with empty name");
    
    app.AddWallet("Valid", 100);
    std::string wId = app.GetWalletsList()->Get(0)->GetId();
    
    // Thêm giao dịch số tiền Âm
    app.AddCategory("Cat");
    std::string cId = app.GetCategoriesList()->Get(0)->GetId();
    
    app.AddTransaction(-500, wId, cId, TransactionType::Expense, Date::GetTodayDate(), "Bad Input");
    // Kỳ vọng: Không thêm được -> Count vẫn là 0
    ASSERT_TRUE(app.GetTransactions()->Count() == 0, "Should not add transaction with negative amount");
    
    // Số dư không đổi
    ASSERT_DOUBLE_EQ(100.0, app.GetWalletById(wId)->GetBalance());
    
    return true;
}

// ==========================================
// MAIN RUNNER
// ==========================================

int main() {
    std::cout << "==========================================\n";
    std::cout << "    PERSONAL FINANCE MANAGER - UNIT TEST  \n";
    std::cout << "==========================================\n";

    std::vector<bool> results;
    results.push_back(Test_Wallet_Creation());
    results.push_back(Test_Transaction_Balance_Update());
    results.push_back(Test_Edit_Transaction_Recalculation());
    results.push_back(Test_Delete_Constraint());
    results.push_back(Test_Input_Validation());

    std::cout << "\n==========================================\n";
    std::cout << "SUMMARY REPORT:\n";
    
    for (size_t i = 0; i < results.size(); ++i) {
        if (results[i]) {
            std::cout << GREEN << "[PASS] Test Case " << (i + 1) << RESET << "\n";
            passedTests++;
        } else {
            std::cout << RED << "[FAIL] Test Case " << (i + 1) << RESET << "\n";
        }
    }
    
    std::cout << "------------------------------------------\n";
    if (passedTests == totalTests) {
        std::cout << GREEN << "RESULT: " << passedTests << "/" << totalTests << " PASSED. EXCELLENT!" << RESET << "\n";
    } else {
        std::cout << RED << "RESULT: " << passedTests << "/" << totalTests << " PASSED. FIX BUGS!" << RESET << "\n";
    }
    std::cout << "==========================================\n";

    // Clean up sau khi test xong
    CleanUpDataFiles(); 
    
    return 0;
}