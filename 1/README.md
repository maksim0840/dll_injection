0) Скачиваем библиотеку [minhook](https://github.com/TsudaKageyu/minhook)

1) Создаём динамическую библиотеку, в которой изменяем поведение функции GetLocalTime + настраиваем перехват оригинальной функции через minhook
   
```
gcc -shared -o custom.dll custom_GetLocalTime.c minhook/src/hook.c minhook/src/buffer.c minhook/src/trampoline.c minhook/src/hde/hde64.c -Iminhook/include -luser32 -lole32 -static-libgcc -static-libstdc++
```

2) Создаём инжектор для подгрузки dll и активации хука

```
gcc dll_injector.c -o dll_injector.exe
```

3) Запускаем наш целевой .exe файл и получем id запущенного процесса (id процесса можно также узнать через диспетчер задач)
   
4) Запускаем инжектор и указываем id процесса запущенного .exe файла, у которого необходимо подменить функцию GetLocalTime
