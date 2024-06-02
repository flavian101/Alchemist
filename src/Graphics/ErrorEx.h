#pragma once

#include <comdef.h>
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <fstream> 

#if defined(DEBUG) || defined(_DEBUG)
#define CHECK_RESULT(hr) \
do { \
    HRESULT check_hr = hr; \
    if (FAILED(check_hr)) { \
        std::wstringstream errorStream; \
        LPWSTR lpMsgBuf = nullptr; \
        DWORD dwChars = FormatMessage( \
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, \
            nullptr, \
            check_hr, \
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
            reinterpret_cast<LPWSTR>(&lpMsgBuf), \
            0, \
            nullptr \
        ); \
        if (dwChars > 0) { \
            errorStream << L"Error in function " << __func__ << L" at line " << __LINE__ << L": " << lpMsgBuf; \
            LocalFree(lpMsgBuf); \
        } else { \
            errorStream << L"Unknown error in function " << __func__ << L" at line " << __LINE__ << L": HRESULT = " << std::hex << check_hr; \
        } \
        MessageBox(nullptr, errorStream.str().c_str(), L"Error", MB_OK | MB_ICONERROR); \
        \
        /* Optional: Log to file */ \
        std::wofstream logFile(L"error_log.txt", std::ios_base::app); \
        if (logFile.is_open()) { \
            logFile << errorStream.str() << std::endl; \
            logFile.close(); \
        } \
        \
        /* Break into debugger */ \
        __debugbreak(); \
    } \
} while (0)
#else
#define CHECK_RESULT(hr) hr
#endif
