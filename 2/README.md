1) Создаём динамическую библиотеку 

gcc -shared -o custom.dll custom_GetLocalTime.c -luser32 -lkernel32

