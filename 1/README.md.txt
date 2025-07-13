0) Скачиваем библиотеку minhook

1) Создаём динамическую библиотеку, в которой изменяем поведение функции GetLocalTime + настраиваем перехват оригинальной функции через minhook 

gcc -shared -o custom.dll custom_GetLocalTime.c minhook/src/hook.c minhook/src/buffer.c minhook/src/trampoline.c minhook/src/hde/hde64.c -Iminhook/include -luser32 -lole32 -static-libgcc -static-libstdc++

2) создаём инжектор для подгрузки dll и активации хука

gcc dll_injector.c -o dll_injector.exe

3) запускаем инжектор и указываем id процесса запущенной программы, у которой необходимо подменить функцию GetLocalTime
