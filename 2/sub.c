#include "stdio.h"
#include <Windows.h>

int main(void) {
    DWORD pid = GetCurrentProcessId();
    printf("PID: %lu\n\n", pid);

    for (int i = 0; i < 1000; ++i) {
        SYSTEMTIME lt = {0};
        GetLocalTime(&lt);
        printf("The local time is: %02d:%02d:%02d\n", lt.wHour, lt.wMinute, lt.wSecond);
        Sleep(5000);
    }
    return 0;
}