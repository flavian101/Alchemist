#pragma once
#include <comdef.h>
#include <Windows.h>
#include <iostream>

#if defined (DEBUG) || defined (_DEBUG)
#define CHECK_RESULT(hr) \
do { \
  if (FAILED(hr)) { \
    LPWSTR lpMsgBuf = NULL; \
    DWORD dwChars = FormatMessage( \
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, \
      NULL, \
      hr, \
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
      (LPWSTR)&lpMsgBuf, \
      0, \
      NULL \
    ); \
    if (dwChars > 0) { \
MessageBox(NULL, lpMsgBuf, L"Error", MB_OK | MB_ICONERROR); \
LocalFree(lpMsgBuf); \
    } \
  } \
} while (0)
#else
#define CHECK_RESULT(x)
#endif