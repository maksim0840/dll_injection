#include <windows.h>
#include <stdio.h>

int main() {
    DWORD pid;
    printf("Enter PID of target process: ");
    scanf("%lu", &pid);

    // УКАЗЫВАТЬ ПОЛНЫЙ ПУТЬ ДО .dll (ЕСЛИ .exe ФАЙЛ В НАХОДИТСЯ В ДРУГОЙ ДИРЕКТОРИИ)!!!!!!!!!
    const char* dllPath = "custom.dll";

    // Открываем чужой процесс с полными правами доступа
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProc) {
        printf("[-] OpenProcess failed.\n");
        return 1;
    }

    // Выделяем память в чужом процессе под запись пути кастомной dll
    LPVOID remoteStr = VirtualAllocEx(hProc, NULL, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
    if (!remoteStr) {
        printf("[-] VirtualAllocEx failed.\n");
        return 1;
    }

    // Записываем путь до dll в память процесса
    if (!WriteProcessMemory(hProc, remoteStr, dllPath, strlen(dllPath) + 1, NULL)) {
        printf("[-] WriteProcessMemory failed.\n");
        return 1;
    }

    // Получаем адресс функции LoadLibraryA из библиотеки kernel32.dll
    LPVOID loadLibraryAddr = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

    // Создаём дополнительный поток в чужом процессе и вызываем в нём функцию LoadLibraryA (функция для загрузки самой dll)
    HANDLE hThread = CreateRemoteThread(
        hProc, NULL, 0,
        (LPTHREAD_START_ROUTINE)loadLibraryAddr,
        remoteStr,
        0, NULL
    );
    
    if (!hThread) {
        printf("[-] CreateRemoteThread failed.\n");
        return 1;
    }

    printf("[+] DLL injected successfully!\n");
    CloseHandle(hProc);
    return 0;
}