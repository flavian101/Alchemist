#pragma once
#include <comdef.h>
#include <Windows.h>
#include <iostream>
#include <sstream>

#if defined(DEBUG) || defined(_DEBUG)
#define CHECK_RESULT(hr) \
do { \
    HRESULT check_hr = hr; \
    if (FAILED(check_hr)) { \
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
            std::wstringstream errorStream; \
            errorStream << L"Error in file " << __FILE__ << L" at line " << __LINE__ << L": " << lpMsgBuf; \
            MessageBox(nullptr, errorStream.str().c_str(), L"Error", MB_OK | MB_ICONERROR); \
            LocalFree(lpMsgBuf); \
        } \
    } \
} while (0)
#else
#define CHECK_RESULT(hr) hr
#endif
