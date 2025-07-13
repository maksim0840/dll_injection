1) Создаём динамическую библиотеку custom.dll из файла custom_GetLocalTime.c
```
gcc -shared -o custom.dll custom_GetLocalTime.c -luser32 -lkernel32
```
