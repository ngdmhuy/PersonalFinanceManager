#include "Controllers/NavigationController.h"
#include "Controllers/AppController.h"
#include "Utils/PlatformUtils.h"

// Include Models
#include "Models/Transaction.h"
#include "Models/Income.h"
#include "Models/Expense.h"
#include "Models/Wallet.h"
#include "Models/Category.h"
#include "Models/IncomeSource.h"
#include "Models/RecurringTransaction.h"

#include "Views/InputValidator.h"

#include <iostream>
#include <sstream>
#include <iomanip>

struct ReportStat {
    std::string id;
    double amount;
    ReportStat(std::string i, double a) : id(i), amount(a) {}
};

static bool GetReportDateRange(Date& start, Date& end) {
    ConsoleView view;
    view.ClearScreen();
    view.PrintHeader("SELECT REPORT PERIOD");
    
    std::cout << "1. Current Month" << std::endl;
    std::cout << "2. Specific Year (Annual Report)" << std::endl;
    std::cout << "3. Custom Date Range" << std::endl;
    std::cout << "0. Back" << std::endl;
    
    int choice = InputValidator::GetValidIndex("Select option: ", 0, 3);
    
    Date today = Date::GetTodayDate();
    
    if (choice == 0) return false;
    
    if (choice == 1) {
        start = Date(1, today.GetMonth(), today.GetYear());
        end = Date::GetEndOfMonth(today.GetMonth(), today.GetYear());
    } 
    else if (choice == 2) {
        int year = InputValidator::GetValidIndex("Enter Year (e.g. 2024): ", 1900, 2100);
        start = Date(1, 1, year);
        end = Date(31, 12, year);
    } 
    else if (choice == 3) {
        start = InputValidator::GetValidDate("Start Date (YYYY-MM-DD): ");
        end = InputValidator::GetValidDate("End Date (YYYY-MM-DD): ");
        if (start > end) {
            view.ShowError("Start date cannot be after end date.");
            view.ShowInfo("Press any key to retry...");
            GetKeyPress(); 
            return false;
        }
    }
    return true;
}

void NavigationController::ShowReportsFlow() {
    bool inReportsMenu = true;
    while (inReportsMenu) {
        char c = menus.DisplayReportsMenu();
        if (c == 27) break; // ESC

        switch (c) {
            case '1': HandleMonthlySummary(); break;      
            case '2': HandleSpendingByCategory(); break;  
            case '3': HandleIncomeVsExpense(); break;     
            case '4': HandleWalletBalanceOverview(); break; 
            case '5': HandleIncomeBySource(); break;      
            default:
                view.ShowError("Invalid selection. Try again.");
                PauseWithMessage("Press any key to continue...");
                break;
        }
    }
}

void NavigationController::HandleMonthlySummary() {
    // 1. CHỌN KHOẢNG THỜI GIAN (Đáp ứng yêu cầu "Annual/Time-based Statistics")
    Date start, end;
    if (!GetReportDateRange(start, end)) return; // Người dùng chọn Back hoặc Hủy

    view.ClearScreen();
    // In tiêu đề có kèm ngày tháng để báo cáo trông chuyên nghiệp hơn
    view.PrintHeader("FINANCIAL SUMMARY (" + start.ToString() + " - " + end.ToString() + ")");

    double totalIncome = 0;
    double totalExpense = 0;

    // 2. TÍNH TOÁN (Có lọc theo ngày)
    ArrayList<Transaction*>* transactions = appController->GetTransactions();
    
    if (transactions) {
        for (size_t i = 0; i < transactions->Count(); ++i) {
            Transaction* t = transactions->Get(i);
            
            // [QUAN TRỌNG] Chỉ tính các giao dịch nằm trong khoảng thời gian đã chọn
            if (t->GetDate() >= start && t->GetDate() <= end) {
                if (t->GetType() == TransactionType::Income) {
                    totalIncome += t->GetAmount();
                } else if (t->GetType() == TransactionType::Expense) {
                    totalExpense += t->GetAmount();
                }
            }
        }
    }

    double netBalance = totalIncome - totalExpense;

    // 3. HIỂN THỊ KẾT QUẢ (Dạng bảng cho đẹp)
    std::string headers[] = {"Category", "Amount"};
    int widths[] = {30, 30}; // Canh chỉnh độ rộng cột
    
    view.PrintTableHeader(headers, widths, 2);

    // Dòng Total Income
    view.SetColor(ConsoleView::COLOR_SUCCESS); // Màu xanh cho thu nhập
    std::string rowInc[] = {"TOTAL INCOME (+)", view.FormatCurrency((long long)totalIncome)};
    view.PrintTableRow(rowInc, widths, 2);

    // Dòng Total Expense
    view.SetColor(ConsoleView::COLOR_ERROR); // Màu đỏ cho chi tiêu
    std::string rowExp[] = {"TOTAL EXPENSE (-)", view.FormatCurrency((long long)totalExpense)};
    view.PrintTableRow(rowExp, widths, 2);

    view.ResetColor();
    view.PrintTableSeparator(widths, 2);

    // Dòng Net Balance (Số dư ròng)
    std::string rowNet[] = {"NET BALANCE (=)", view.FormatCurrency((long long)netBalance)};
    view.PrintTableRow(rowNet, widths, 2);

    // Thông báo trạng thái tài chính
    std::cout << std::endl;
    if (netBalance >= 0) {
        view.ShowSuccess("   Result: SURPLUS. Good job keeping your finances healthy!");
    } else {
        view.ShowWarning("   Result: DEFICIT. You spent more than you earned in this period!");
    }

    PauseWithMessage("\nPress any key to return to menu...");
}

void NavigationController::HandleSpendingByCategory() {
    // 1. CHỌN NGÀY
    Date start, end;
    if (!GetReportDateRange(start, end)) return; 

    view.ClearScreen();
    view.PrintHeader("EXPENSE BY CATEGORY (" + start.ToString() + " - " + end.ToString() + ")");

    ArrayList<Category*>* categories = appController->GetCategoriesList();
    if (!categories || categories->Count() == 0) {
        view.ShowInfo("No categories defined.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    std::string headers[] = {"Category", "Amount", "% Total"};
    int widths[] = {30, 20, 10};
    view.PrintTableHeader(headers, widths, 3);

    // Tính tổng chi phí trong kỳ
    double totalExpenseInPeriod = 0;
    ArrayList<Transaction*>* allTrans = appController->GetTransactions();
    if (allTrans) {
        for(size_t i=0; i<allTrans->Count(); ++i) {
            Transaction* t = allTrans->Get(i);
            if (t->GetType() == TransactionType::Expense && t->GetDate() >= start && t->GetDate() <= end) {
                totalExpenseInPeriod += t->GetAmount();
            }
        }
    }

    for (size_t i = 0; i < categories->Count(); ++i) {
        Category* c = categories->Get(i);
        
        // Lấy transaction theo category
        ArrayList<Transaction*>* transList = appController->GetTransactionsByCategory(c->GetId());
        
        double catTotal = 0;
        if (transList) {
            for (size_t j = 0; j < transList->Count(); ++j) {
                Transaction* t = transList->Get(j);
                
                // [QUAN TRỌNG] Filter theo ngày
                if (t->GetDate() >= start && t->GetDate() <= end) {
                    catTotal += t->GetAmount();
                }
            }
            delete transList;
        }

        if (catTotal > 0) {
            double pct = (totalExpenseInPeriod > 0) ? (catTotal / totalExpenseInPeriod * 100.0) : 0;
            std::ostringstream ss; 
            ss << std::fixed << std::setprecision(1) << pct << "%";
            
            std::string row[] = {c->GetName(), view.FormatCurrency((long long)catTotal), ss.str()};
            view.PrintTableRow(row, widths, 3);
        }
    }

    view.PrintTableSeparator(widths, 3);
    view.PrintText("Total Expense in Period: " + view.FormatCurrency((long long)totalExpenseInPeriod));

    PauseWithMessage("Press any key to continue...");
}

void NavigationController::HandleIncomeBySource() {
    // 1. YÊU CẦU NGƯỜI DÙNG CHỌN KHOẢNG THỜI GIAN (Fix yêu cầu "Time-based")
    Date start, end;
    if (!GetReportDateRange(start, end)) return; // Người dùng chọn Back

    view.ClearScreen();
    // Hiển thị tiêu đề kèm khoảng thời gian đã chọn cho chuyên nghiệp
    view.PrintHeader("INCOME BY SOURCE (" + start.ToString() + " - " + end.ToString() + ")");

    ArrayList<IncomeSource*>* incomeSources = appController->GetIncomeSourcesList();
    if (!incomeSources || incomeSources->Count() == 0) {
        view.ShowInfo("No income sources defined.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    // Header bảng
    std::string headers[] = {"Source Name", "Amount", "% Total"};
    int widths[] = {30, 20, 10};
    view.PrintTableHeader(headers, widths, 3);

    // Tính tổng thu nhập TRONG KHOẢNG THỜI GIAN ĐÓ để làm mẫu số tính %
    double totalIncomeInPeriod = 0;
    
    // Bước phụ: Cần tính tổng trước để quy ra % (Optional, nhưng làm vậy mới chuẩn report)
    // Tuy nhiên để code đơn giản và chạy nhanh (O(N)), ta có thể bỏ qua % chính xác tuyệt đối 
    // hoặc chấp nhận % trên tổng thu nhập tìm được. Ở đây tôi sẽ tính tổng trước.
    
    // (Cách tối ưu hơn: Duyệt transaction 1 lần, bỏ vào Map tạm. Nhưng vì cấm STL, ta làm cách nested loop đơn giản)
    
    // Để hiển thị đúng %, ta cần tổng thu nhập của giai đoạn này trước
    ArrayList<Transaction*>* allTrans = appController->GetTransactions();
    if (allTrans) {
        for(size_t i=0; i<allTrans->Count(); ++i) {
            Transaction* t = allTrans->Get(i);
            if (t->GetType() == TransactionType::Income && t->GetDate() >= start && t->GetDate() <= end) {
                totalIncomeInPeriod += t->GetAmount();
            }
        }
    }

    // Duyệt từng Source để tính tiền
    for (size_t i = 0; i < incomeSources->Count(); ++i) {
        IncomeSource* s = incomeSources->Get(i);
        
        // Lấy toàn bộ transaction của Source này
        ArrayList<Transaction*>* transList = appController->GetTransactionsByIncomeSource(s->GetId());
        
        double sourceTotal = 0;
        if (transList) {
            for (size_t j = 0; j < transList->Count(); ++j) {
                Transaction* t = transList->Get(j);
                
                // [QUAN TRỌNG] CHỈ CỘNG NẾU NGÀY NẰM TRONG KHOẢNG ĐÃ CHỌN
                if (t->GetDate() >= start && t->GetDate() <= end) {
                    sourceTotal += t->GetAmount();
                }
            }
            delete transList; // Dọn dẹp bộ nhớ list tạm
        }

        // Chỉ hiện những nguồn có tiền > 0 trong kỳ này (cho gọn bảng)
        if (sourceTotal > 0) {
            double pct = (totalIncomeInPeriod > 0) ? (sourceTotal / totalIncomeInPeriod * 100.0) : 0;
            std::ostringstream ss; 
            ss << std::fixed << std::setprecision(1) << pct << "%";
            
            std::string row[] = {s->GetName(), view.FormatCurrency((long long)sourceTotal), ss.str()};
            view.PrintTableRow(row, widths, 3);
        }
    }

    view.PrintTableSeparator(widths, 3);
    view.PrintText("Total Income in Period: " + view.FormatCurrency((long long)totalIncomeInPeriod));
    
    PauseWithMessage("Press any key to continue...");
}

void NavigationController::HandleIncomeVsExpense() {
    HandleMonthlySummary(); 
}

void NavigationController::HandleWalletBalanceOverview() {
    view.ClearScreen();
    view.PrintHeader("WALLET BALANCES");

    ArrayList<Wallet*>* wallets = appController->GetWalletsList();
    double total = appController->GetTotalBalance();

    std::string headers[] = {"Wallet", "Balance", "%"};
    int widths[] = {30, 20, 10};
    view.PrintTableHeader(headers, widths, 3);

    for (size_t i = 0; i < wallets->Count(); ++i) {
        Wallet* w = wallets->Get(i);
        
        double pct = (total > 0) ? (w->GetBalance() / total * 100.0) : 0;
        std::ostringstream ss; ss << std::fixed << std::setprecision(1) << pct << "%";
        
        std::string row[] = {w->GetName(), view.FormatCurrency((long long)w->GetBalance()), ss.str()};
        view.PrintTableRow(row, widths, 3);
    }
    view.PrintTableSeparator(widths, 3);
    view.PrintText("TOTAL ASSETS: " + view.FormatCurrency((long long)total));

    PauseWithMessage("Press any key to continue...");
}   