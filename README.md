# Personal Finance Manager (C++)

A simple console-based personal finance manager written in C++.

- **Purpose:** Track wallets, incomes, expenses and recurring transactions via a clear, testable command-line UI.  
- **Status:** Finished — core features implemented (Models, Views, Controllers); report drafted and diagrams prepared.

---

## 📁 Project layout

Top-level tree (important files/folders):

```
PersonalFinanceManager/
  ├─ include/           # public headers (Views, Models, Controllers, Utils)
  ├─ src/               # source files (implementations)
  ├─ data/              # sample and runtime data
  ├─ Report/            # LaTeX report and images
  │   ├─ content/       # sections (including M3.tex)
  │   └─ img/           # images / screenshots
  ├─ CMakeLists.txt
  ├─ makefile
  ├─ run_windows.bat
  └─ run_mac.command
```

Key source areas:
- `include/Views` & `src/Views` — Console drawing, menus, dashboard, input validators
- `include/Controllers` & `src/Controllers` — Navigation and UI flows (Nav* files) and `AppController` for business rules
- `src/Models` — Transaction, Wallet, Category, IncomeSource, RecurringTransaction

---

## 🛠 Requirements

- C++ toolchain (C++17+), CMake

---

## 🔧 Build & Run

### Windows (quick)
- Use the provided batch: `run_windows.bat` — this script builds and runs the application (if present and configured for your system).

### Generic (CMake)
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release

```

### Running
- Run the produced executable from the build directory, or use `run_windows.bat` on Windows.

---

## ✅ Tests

- There is a `UnitTest.cpp` file in `src/` (unit test skeletons). Build the tests using the same CMake build and run the generated test executable (check `build/` for test binary).
- Suggested tests: `InputValidator` (money/date/index validation) and basic controller flows (add/view/delete flows).

---

## 🧭 Useful file references

- UI / Views: `include/Views/*`, `src/Views/*`
- Controllers: `src/Controllers/Nav*.cpp`, `src/Controllers/AppController.cpp`
- Models: `src/Models/*`
- Utilities: `src/Utils/*`
- Report: `Report/main.tex`, `Report/content/M3.tex`

---
