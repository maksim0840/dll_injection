#include <Windows.h>
#include <minhook.h> // инструмент для перехвата функций

typedef VOID (WINAPI *funcptr_GetLocalTime)(LPSYSTEMTIME); // тип указателя на функцию
funcptr_GetLocalTime origGetLocalTime = NULL; // глобальная переменная для хранения указателя на оригинальную функцию

void WINAPI customGetLocalTime(LPSYSTEMTIME lpSystemTime) {
    lpSystemTime->wHour = 12;
    lpSystemTime->wMinute = 34;
    lpSystemTime->wSecond = 56;
}

// Точка входа в dll (вызывается самой Windows при загрузки dll)
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        MessageBox(NULL, "DLL injected!", "Notice", MB_OK);
        MH_Initialize();                                                                        // инициализируем MinHook
        MH_CreateHook(&GetLocalTime, &customGetLocalTime, (LPVOID*)&origGetLocalTime);          // перехватываем оригинальную функцию
        MH_EnableHook(&GetLocalTime);                                                           // активируем хук
    }
    return TRUE;
}