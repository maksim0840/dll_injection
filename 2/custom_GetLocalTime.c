#include <windows.h>

// Явно экспортируем функцию
__declspec(dllexport)
VOID WINAPI customGetLocalTime(LPSYSTEMTIME t) {
    t->wHour = 12;
    t->wMinute = 34;
    t->wSecond = 56;

    // SYSTEMTIME real;
    // GetLocalTime(&real);
    // t->wYear = real.wYear;
    // t->wMonth = real.wMonth;
    // t->wDay = real.wDay;
}
