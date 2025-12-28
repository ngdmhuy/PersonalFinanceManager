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
- Use the provided batch: `run_windows.bat` — this script builds and runs the application on Windows (if present and configured for your system).

### macOS (quick)
- Use the provided script: `run_mac.command` — make it executable, then run it from the project root:
```bash
chmod +x run_mac.command
./run_mac.command
```
(This script will typically build and run the project on macOS; you can also use the Generic CMake steps below.)

### Generic (CMake)
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Running
- Run the produced executable from the build directory, or use `run_windows.bat` on Windows or `run_mac.command` on macOS.

---

## 📹 Demo Video
- Demo video link: https://www.youtube.com/watch?v=hVvAY_DrcDo

---