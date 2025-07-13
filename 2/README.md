# Байт код функции
--------------------

1) Создаём динамическую библиотеку custom.dll из файла custom_GetLocalTime.c
  ```
  gcc -shared -o custom.dll custom_GetLocalTime.c -luser32 -lkernel32
  ```

2) Устанавливаем и открываем PE-bear (в обзоре представлена вресия PE-bear v0.7.1)
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

# Изменение exe файла
--------------------

1) Копируем испоняемый файл, который хотим измнеить (чтобы полностью не перезаписать оригинал)
   ```
   sub.exe -> sub_new.exe
   ```
   
2) Закрываем имеющийся файл в PE-bear и снова открываем через него другой - копию исполняемого .exe файла
  ```
  File -> Load PEs -> открываем sub_new.exe
  ```

3) В левом меню навигации нажимаем на вкладку "NT Headers" -> "Optional Header" и в центральном меню переключаемся на вкладку "Optional Hdr"
   
 <img width="2555" height="1310" alt="Снимок экрана 2025-07-14 000326" src="https://github.com/user-attachments/assets/45547c42-ff20-4c00-8c10-9b2c2b2fba0b" />

4) Запоминаем значения File Alignment
  - File Alignment: 0x200

5) В левой меню навигации находим вкладку "Section", кликаем ПКМ и создаём новую секцию
   
   <img width="2553" height="1321" alt="Снимок экрана 2025-07-13 234040" src="https://github.com/user-attachments/assets/025afa84-67dd-4675-b8f0-d6af2c85b804" />

6) Заполняем информацию о секции (количество байт в нашей функции = 41 = 0x29):
   - Section name (имя, начинающееся с точки, не длинее 8 символов): ".mycode"
   - Raw size (количество байт в секции, округлённое по кратности в FileAlignment в большую сторону): 0x200
   - Virtual size (точное количество байт в секции): 0x29
   - флаги (заполняем ТОЛЬКО те флаги, которые соотвествуют содержанию в секции): read, execute
     -- read - данные в секции будут считываться процессом
     -- write - данные в секции будут записываться процессом
     -- execute - данные в секции будут исполняться процессом
 
  <img width="465" height="397" alt="Снимок экрана 2025-07-14 002546" src="https://github.com/user-attachments/assets/cc456373-75e7-4705-9f7f-3e5f04b0ae7c" />

7) Переходим в созданную секцию .mycode через левое меню навигации, после чего нас сразу перенаправляет на нужное смещение

8) В верхней части окошка с байт-кодом записываем сохранённые байты функции
   
  <img width="2557" height="1316" alt="Снимок экрана 2025-07-14 003208" src="https://github.com/user-attachments/assets/ebd6867b-4b7c-453f-a310-610c0207b156" />

9) Переключаемся на вкладку Section Hdrs и запоминаем Raw Addr и Virtual Addr нашей секции .mycode
  - Raw Addr: 0x16800
  - Virtual Addr: 0x1C000
    
  <img width="2556" height="1311" alt="Снимок экрана 2025-07-14 003801" src="https://github.com/user-attachments/assets/474b0d3b-41bb-479c-b49b-cf89f14c6071" />

10) В этой же вкладке ищем секцию "НУЖНАЯ!!!!!!" .idata и запоминаем Raw Addr и Virtual Addr
  - Raw Addr: 0xB200
  - Virtual Addr: 0x11000
    
  <img width="2557" height="1319" alt="Снимок экрана 2025-07-14 011817" src="https://github.com/user-attachments/assets/c239e664-00e4-4f15-a984-6abc58ffe0ef" />

10) Переключаемся на вкладку Imports и ищем адресс Thunk для функции, которую хотим заменить (в данном случае - функция GetLocaltTime)
    - Thunk: 0x11414

  <img width="2556" height="1318" alt="Снимок экрана 2025-07-14 004639" src="https://github.com/user-attachments/assets/9f8ab34d-f25d-423b-91c7-cfa88c5e3d66" />
  
11) Найдём адресс указателя на функцию, которую хотим подменить по формуле (Raw_Offset = Thunk - [.idata]Virtual_Addr + [.idata]Raw_Addr)
    - Raw_Offset = 0x11414 - 0x11000 + 0xB200 = 0xB614

12) Переходим по Raw_Offset (Ctrl + G)

  <img width="495" height="380" alt="Снимок экрана 2025-07-14 011628" src="https://github.com/user-attachments/assets/05604da4-f9b0-40dc-9ae5-7d5095be19c2" />

13) Перезаписать имеющиеся 8 байт на значение [.mycode]Virtual Addr в формате little endian
    - 00 00 1C 00 00 00 00 00

    <img width="2558" height="1318" alt="Снимок экрана 2025-07-14 012249" src="https://github.com/user-attachments/assets/a430fc0f-ce59-4fdf-9a1a-451e74eb47fb" />




