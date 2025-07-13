1) Создаём динамическую библиотеку custom.dll из файла custom_GetLocalTime.c
  ```
  gcc -shared -o custom.dll custom_GetLocalTime.c -luser32 -lkernel32
  ```

2) Устанавливаем и открываем PE-bear
  ```
  winget install pe-bear
  pe-bear
  ```

3) Открываем dll библиотеку в PE-bear
  ```
  File -> Load PEs -> открываем custom.dll
  ```

4) Переключаемся на вкладку Exports и ищем нашу функцию (customGetLocalTime)
   
  <img width="2559" height="1324" alt="Снимок экрана 2025-07-13 224041" src="https://github.com/user-attachments/assets/44254b99-7a69-4990-aa87-3a529ed945dd" />

5) Запоминаем значение Function RVA (адресс кода функции относительно начала данного dll файла)
  - Function_RVA: 0x15A0

6) Переключаемся на вкладку Section Hdrs и ищем секуцию .text
   
  <img width="2556" height="1319" alt="Снимок экрана 2025-07-13 224217" src="https://github.com/user-attachments/assets/4b7e9443-d43e-416e-8464-dc2c96d94300" />

8)  Запоминаем значения Raw Addr и Virtual Addr
  - Raw_Addr: 0x600
  - Virtual_Addr: 0x1000

9) Вычисляем Raw Offset по формуле (Raw_Offset = Function_RVA - Virtual_Addr + Raw_Addr)
  - Raw_Offset = 0x15A0 - 0x1000 + 0x600 = 0xBA0

10) Переходим по Raw Offset и ищем наш участок в коде (для перехода: Ctrl + G)

  <img width="494" height="384" alt="Снимок экрана 2025-07-13 224537" src="https://github.com/user-attachments/assets/2a44b757-3646-4311-905c-74e57b103de9" />

11) Переключаемся на вкладку Disasm .text и в окне дизассемблера выделяем начало (самая верхняя команда после перехода) и конец функции (почти всегда команда ret)

  <img width="2556" height="1318" alt="Снимок экрана 2025-07-13 230819" src="https://github.com/user-attachments/assets/951756cb-6649-4615-9551-97e0102ccb4d" />
  
12) После этого в окне байт-кода программа сама подсветит нужные байты, которые нужно скопировать и запомнить
    - байты функции:
      55 48 89 E5 48 89 4D 10 48 8B 45 10 66 C7 40 08
      0C 00 48 8B 45 10 66 C7 40 0A 22 00 48 8B 45 10
      66 C7 40 0C 38 00 90 5D C3


 
