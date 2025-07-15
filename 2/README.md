В данном пункте будет рассмотрен способ внедрения в файл .exe собственной функции и переопределение вызова на неё через измнение инструкции call



# Получение байт кода кастомной функции
--------------------



1) Создаём динамическую библиотеку custom.dll из файла custom_GetLocalTime.c
   
    ```
    gcc -shared -o custom.dll custom_GetLocalTime.c -luser32 -lkernel32
    ```

3) Устанавливаем и открываем PE-bear (в обзоре представлена вресия PE-bear v0.7.1)

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

    - [.text]Raw_Addr: 0x600
    - [.text]Virtual_Addr: 0x1000

10) Вычисляем Raw Offset по формуле (Raw_Offset = Function_RVA - [.text]Virtual_Addr + [.text]Raw_Addr)
    
    - Raw_Offset = 0x15A0 - 0x1000 + 0x600 = 0xBA0

11) Переходим по Raw Offset и ищем наш участок в коде (для перехода: Ctrl + G)

    <img width="494" height="384" alt="Снимок экрана 2025-07-13 224537" src="https://github.com/user-attachments/assets/2a44b757-3646-4311-905c-74e57b103de9" />

11) Переключаемся на вкладку Disasm .text и в окне дизассемблера выделяем начало (самая верхняя команда после перехода) и конец функции (почти всегда команда ret)

    <img width="2556" height="1318" alt="Снимок экрана 2025-07-13 230819" src="https://github.com/user-attachments/assets/951756cb-6649-4615-9551-97e0102ccb4d" />
  
12) После этого в окне байт-кода программа сама подсветит нужные байты, которые нужно скопировать и запомнить

    - байты функции:

    ```
    55 48 89 E5 48 89 4D 10 48 8B 45 10 66 C7 40 08
    0C 00 48 8B 45 10 66 C7 40 0A 22 00 48 8B 45 10
    66 C7 40 0C 38 00 90 5D C3
    ```



# Изменение адресса вызова функции в .exe-шнике 
--------------------



1) Закрываем имеющийся dll-файл в PE-bear и открываем исполняемый .exe файл, который хотим изменить

    ```
    File -> Unload All
    File -> Load PEs -> открываем sub.exe
    ```

3) Переключаемся на вкадку Optional Hdr и ищем значения Image Base и File Alignment
    
    - Image_Base: 0x140000000
    - File_Alignment: 0x200
    
    <img width="2554" height="1322" alt="Снимок экрана 2025-07-15 134332" src="https://github.com/user-attachments/assets/9d3d7697-d831-4a87-a822-c0d1de8e1d16" />

4) В левом меню навигации находим вкладку "Section", кликаем ПКМ и создаём новую секцию
   
    <img width="2553" height="1321" alt="Снимок экрана 2025-07-13 234040" src="https://github.com/user-attachments/assets/025afa84-67dd-4675-b8f0-d6af2c85b804" />

5) Заполняем информацию о секции (количество байт в нашей функции = 41 = 0x29):
    
    - Section name (имя, начинающееся с точки, не длинее 8 символов): ".mycode"
    - Raw size (количество байт в секции, округлённое по кратности в FileAlignment в большую сторону): 0x200
    - Virtual size (точное количество байт в секции): 0x29
    - флаги (заполняем ТОЛЬКО те флаги, которые соотвествуют содержанию в секции): read, execute
      - read - данные в секции будут считываться процессом
      - write - данные в секции будут записываться процессом
      - execute - данные в секции будут исполняться процессом
 
    <img width="465" height="397" alt="Снимок экрана 2025-07-14 002546" src="https://github.com/user-attachments/assets/cc456373-75e7-4705-9f7f-3e5f04b0ae7c" />

6) Переходим в созданную секцию .mycode через левое меню навигации, после чего нас сразу перенаправляет на нужное смещение

7) В верхней части окна с байт-кодом записываем сохранённые байты функции
   
    <img width="2557" height="1316" alt="Снимок экрана 2025-07-14 003208" src="https://github.com/user-attachments/assets/ebd6867b-4b7c-453f-a310-610c0207b156" />

8) Переключаемся на вкладку Section Hdrs и запоминаем Virtual Addr нашей секции .mycode
    
    - [.mycode]Virtual_Addr: 0x1C000
    
    <img width="2556" height="1311" alt="Снимок экрана 2025-07-14 003801" src="https://github.com/user-attachments/assets/474b0d3b-41bb-479c-b49b-cf89f14c6071" />

9) В этой же вкладке (Section Hdrs) ищем секцию .text (секция с исполняемым кодом файла .exe) и запоминаем Raw Addr и Virtual Addr
    
    - [.text]Raw_Addr: 0x600
    - [.text]Virtual_Addr: 0x1000
    
    <img width="2556" height="1323" alt="Снимок экрана 2025-07-15 135042" src="https://github.com/user-attachments/assets/c078af5c-caa6-435f-abd1-63ffa38d4cc6" />

10) Создаём файл, содержащий ассэмблерный код секции .text и открываем

    ```
    File -> From all loaded -> Export Disassembly to -> сохраняем файл в папку с проектом
    Открываем файл sub.exe[.text].txt
    ```
    
12) В данном файле ищем инструкцию "call", связанную с нашей функцией, которую хотим заменить

    - В файле с кодом для удобства уже прописаны названия функций, адресса которых используются для перехода, поэтому просто в поиске (через Ctrl+F) ищем упоминание функции "[KERNEL32.dll].GetLocalTime" и запоминаем адресс команды.

    <img width="2555" height="1362" alt="Снимок экрана 2025-07-15 135810" src="https://github.com/user-attachments/assets/1c46bc82-dcf5-4444-83ef-4c491c19d9ce" />
  
    - File_Addr: 0x14000175e
      
13) Вычисляем Raw Addr нашей команды по формуле (Raw_Addr = File_Addr - Image_Base - [.text]Virtual_Addr + [.text]Raw_Addr)

    - Raw_Addr = 0x14000175e - 0x140000000 - 0x1000 + 0x600 = 0xD5E

14) Вычисляем адресс вызова нашей кастомной функции по формуле (Custom_Addr = Image_Base + [.mycode]Virtual Addr)

    - Мы указываем адресс функции, как адресс начала секции .mycode (если функция записана не в начале секции, то стоит добавить соотвествующее смещение)
    - Custom_Addr = 0x140000000 + 0x1C000 = 0x14001С000
  
16) Переходим по получившемуся значению Raw Addr команды (через Ctrl + G)
    
    <img width="497" height="378" alt="Снимок экрана 2025-07-15 140626" src="https://github.com/user-attachments/assets/8d05d4ff-cbf3-4ca0-9300-7f24287ea4a8" />

17) Ищем в окне дизассемблера 2 команды, которые ранее мы уже находили в текстовом файле (команда mov для передачи адресса функции и команда call для перехода по этому адрессу), выделяем эти 2 команды и смотрим на их представление в окне с байт-кодом

    <img width="2559" height="1320" alt="Снимок экрана 2025-07-15 141011" src="https://github.com/user-attachments/assets/3a34c312-95e7-4fba-abe6-6202e7098d85" />

18) Данные 2 команды можно заменить на 1 - вызов call нашей кастомной функции по адрессу Custom Addr. Байты, которые будут служить заменой выглядят следующим образом:

    ```
    E8 9D A8 01 00 90 90 90 90
    ```
    
    - E8 - команда call
    - 9D A8 01 00 - значение Custom Addr в виде байт-кода в формате little-endian
    - 90 - Nop-ы (пропуски значений)

        
      
    <img width="2556" height="1318" alt="Снимок экрана 2025-07-15 141426" src="https://github.com/user-attachments/assets/b7416245-8ba0-41ed-bad2-bcf28bab42b3" />

  
  20) В левом меню навигации находим название открытого файла (sub.exe), нажимаем ПКМ и сохраняем наши изменния в новый .exe файл
      
    <img width="2556" height="1319" alt="Снимок экрана 2025-07-15 141738" src="https://github.com/user-attachments/assets/3998356f-ae11-4753-aeda-4fd386019b98" />



