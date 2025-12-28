@echo off
setlocal
title Personal Finance Manager - Universal Builder

:: 1. Navigate to script folder
cd /d "%~dp0"

echo ==================================================
echo      BUILDING: PERSONAL FINANCE MANAGER
echo ==================================================

:: 2. CLEANUP (Optional: Remove specific cache if needed, but keeping build folder is faster)
if not exist build mkdir build
cd build

:: 3. AUTO-DETECT COMPILER
where g++ >nul 2>nul
if %errorlevel% equ 0 (
    echo [DETECTED] MinGW / G++ Compiler found.
    echo [INFO] Configuring for MinGW Makefiles...
    cmake -G "MinGW Makefiles" ..
) else (
    echo [DETECTED] No MinGW found. Assuming Visual Studio.
    echo [INFO] Configuring for Visual Studio...
    cmake ..
)

:: 4. CHECK CONFIGURATION STATUS
if %errorlevel% neq 0 (
    echo [CRITICAL ERROR] CMake configuration failed.
    echo Ensure you have Visual Studio C++ workload OR MinGW installed.
    pause
    exit /b
)

:: 5. COMPILE
echo.
echo [INFO] Compiling...
cmake --build . --config Release

if %errorlevel% neq 0 (
    echo [ERROR] Build failed. See messages above.
    pause
    exit /b
)

echo.
echo [SUCCESS] Launching Application...
echo ==================================================

:: 6. SMART RUN
if exist "Release\PersonalFinanceManager.exe" (
    cd Release
    PersonalFinanceManager.exe
) else (
    if exist "PersonalFinanceManager.exe" (
        PersonalFinanceManager.exe
    ) else (
        echo [ERROR] Could not find executable file.
        echo Checked: build\Release\PersonalFinanceManager.exe
        echo Checked: build\PersonalFinanceManager.exe
    )
)

pause