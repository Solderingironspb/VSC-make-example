## Примеры работ по сборке и отладке проектов для Windows и микроконтроллеров Stm32, в редакторе кода Visual Studio Code на языке Си.
*Все проекты полноценно собираются и разрабатываются только под системой **Windows**, т.к. используют скрипты на powershell.*

**Обязательно измените политику запуска скриптов Powersheell.**

**Если не хотите этого делать, то придется каждый раз запускать скрипт по типу:**

`Powershell.exe -executionpolicy bypass -File "C:\script.ps1" %name%`

**Как изменить политику запуска скриптов на powershell?**

*Сначала узнаем, какая политика сейчас:* `Get-ExecutionPolicy`

![Image](https://github.com/user-attachments/assets/1c34a3eb-05c7-4095-a49c-7103f3a8f204)

*Если **Restricted**, то можно заменить ее на **Remotesigned** или **Bypass**:*

*Команда:* `Set-ExecutionPolicy -Scope CurrentUser -ExecutionPolicy Bypass`

![Image](https://github.com/user-attachments/assets/a92c1635-25d5-4ffd-a2f1-1b399c33f9c4)

## Как начать работу? 

Нам нужны:
1) Утилита **make**
2) Toolchain для ARM **arm-none-eabi**
3) Инструмент для отладки **OpenOCD**
4) Скрипты **OpenOCD** для **interface** и **target** (Допустим конфиги для ST-Link v2 и STM32F103C8T6)
5) *.svd файл под микроконтроллер, который даст возможность просматривать периферию мк во время отладки

Для начала нам понадобится утилита **make** 

**make** — это утилита, автоматизирующая процесс преобразования файлов из одной формы в другую. Чаще всего это компиляция исходного кода в объектные файлы и последующая компоновка в исполняемые файлы или библиотеки.

Последнее, что я смог найти на сегодняшний день - это **GNU Make 4.3 от 2020 года**.

Находится он сразу с набором инструментов **MinGW**.
Скачать можно здесь *(gcc-12.3.0-
64.exe):* https://www.equation.com/servlet/equation.cmd?fa=fortran

Далее установим в раздел C:\MinGW

Должно получиться следующее:

![Image](https://github.com/user-attachments/assets/cbf99b8e-f49c-4fc8-b46c-855e1feb5f9f)

Далее уже по своему вкусу и уму смотрите, нужно ли Вам добавлять папку **bin** в переменные среды **PATH**, чтоб пользоваться  командой **make** из **powershell** без ввода пути или же нет. Почему возникает этот вопрос - все просто. Бывает на одной системе приходится компилировать несколько проектов под разное железо, где требуется разный make, gcc, gdb, size, objcopy и т.п.
Я папку **bin** от **MinGW**  добавлю в **PATH**, чтоб использовать **MinGW** для компиляции проектов на си под Windows, а также использовать **make** под сборку проектов для микроконтроллеров. Последующие установки **gcc-arm-none-eabi** и **OpenOCD** буду использовать по прямому пути, без добавления в **PATH**, т.к. данные утилиты под разные фирмы мк обычно модернизируются производителями и лучше использовать фирмовые, ~~но это не точно~~.

Сделаем вот так *(Нужное выделено желтым):*

![Image](https://github.com/user-attachments/assets/e5901112-628c-4359-871a-d82d75a0bafe)

Проверим:

![Image](https://github.com/user-attachments/assets/8e5d9a8f-4d85-4e65-81ec-83789c51fd6d)

## Установка arm-none-eabi и OpenOCD
Как-то в сообществе Embedded программистов выработалась такая привычка, что используй то, что дает производитель. Возможность получить непонятный косяк в работе резко уменьшается. Если мы работали до этого в **CubeIDE 1.7.0**, то нет особой необходимости что-то скачивать из интернета. У Вас уже все есть. (Не совсем верное утверждение, т.к. есть нюансы).

Допустим я работаю в **CubeIDE 1.7.0**. 

**arm-none-eabi** у меня находится по адресу: `C:\ST\STM32CubeIDE_1.7.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.9-2020-q2-update.win32_2.0.0.202105311346\tools\bin`

![Image](https://github.com/user-attachments/assets/227b41e3-e16c-4fb4-bf42-b5bbdec8c003)

![Image](https://github.com/user-attachments/assets/3abd9939-5a53-4558-80c3-c40b44ca35bd)


К сожалению, здесь есть проблема - в **CubeIDE 1.7.0** GDB имеет версию 8.3.1
Современный плагин **Cortex-Debug** для **Visual Studio Code** будет ругаться на версию, т.к. ему нужно > 9.
```
ERROR: GDB major version should be >= 9, yours is 8
-gdb-exit
-> ^exit
GDB session ended. exit-code: 0
GDB could not start as expected. Bad installation or version mismatch. See if you can start gdb from a shell prompt and check its version (Must be >= 9)
```

Что ж....отметаем версию **CubeIDE 1.7.0**, нам нужно что-то новее.
Я скачал **CubeIDE 1.17.0** и выдернул оттуда все, что нам требуется:

**arm-none-eabi**:
`C:\ST\STM32CubeIDE_1.17.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.12.3.rel1.win32_1.1.0.202410251130\tools\bin`

![Image](https://github.com/user-attachments/assets/72d8ac6a-c4c7-4cf9-8cee-7d65758d3a19)

Кроме, как из **CubeIDE**, можно порпобовать скачать **arm-none-eabi** с сайта: 

https://developer.arm.com/downloads/-/gnu-rm

Он там постарее будет. И как я говорил, в кубе он модифицирован под STM32. Я буду использовать от **CubeIDE 1.17.0**

**openocd** есть по адресу:
`C:\ST\STM32CubeIDE_1.17.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.openocd.win32_2.4.0.202409170845`

![Image](https://github.com/user-attachments/assets/14736a9e-9633-4e3e-9654-c6a59ae1c790)

Но он размазан по куче папок, увидел на сайте https://gnutoolchains.com/arm-eabi/openocd/ эту же версию, взял оттуда.

**make в папке CubeIDE 1.17.0 тоже есть, можем его использовать, либо из MinGW, что указывал ранее, разницы не заметил**:

`C:\ST\STM32CubeIDE_1.17.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.make.win32_2.2.0.202409170845`

![image](https://github.com/user-attachments/assets/ae003f6a-c5c7-444b-b08f-3693bf3cdb7a)

## Interface и target.
Итак, **make**, **arm-none-eabi** и **OpenOCD** (скачанный с https://gnutoolchains.com/arm-eabi/openocd/) у нас есть. 

Теперь для того, чтоб подключиться к тому же stm32 - нужен конфиг **interface** ST-Link и **target** микроконтроллера stm32.

У меня это все теперь находится здесь:

`C:\ST\tools\OpenOCD-20240916-0.12.0\share\openocd\scripts`

![Image](https://github.com/user-attachments/assets/fc8c0ec5-d877-4132-8342-3484ad121d79)

Кто ранее по моим видео работал в **VisualGDB** - думаю узнают структуру папок.

Для примера у нас STM32F103C8T6 и st-link v2

**interface:**
`C:\ST\tools\OpenOCD-20240916-0.12.0\share\openocd\scripts\interface\stlink.cfg`

**target:**
`C:\ST\tools\OpenOCD-20240916-0.12.0\share\openocd\scripts\target\stm32f1x.cfg`

Напомню, что у нас есть копии Stm32F103C8T6 и там CPUTAPID другой. Меняется в target файле:

```
#jtag scan chain
if { [info exists CPUTAPID] } {
   set _CPUTAPID $CPUTAPID
} else {
   if { [using_jtag] } {
      # See STM Document RM0008 Section 26.6.3
      set _CPUTAPID 0x3ba00477
   } {
      # this is the SW-DP tap id not the jtag tap id
      set _CPUTAPID 0x1ba01477 //Вот эту строку меняем на 0x2ba01477 для китайских мк
   }
}
```

По итогу **CubeIDE 1.17.0** наполовину нас всем снабдил. Это и будем использовать. Я думаю для других мк, тех же CH32V также можно все выдрать из **MounriverStudio**

Нам еще понадобится *.svd файл под наш микроконтроллер, чтоб имелась возможность использовать регистры периферии во время дебага.

Взять можно здесь: https://github.com/Solderingironspb/cmsis-svd-stm32

Собираем папку со всем необходимым. Не хочу пользоваться путями из **CubeIDE 1.17.0**, т.к. там черт ногу сломит в этом длинном названии. В названии папок пробелы не используем. Получилось так:

![Image](https://github.com/user-attachments/assets/19dbddca-f59e-4ea4-8180-862db74e86dd)

На этом подготовительные работы закончены. Теперь самое сложное - создать Makefile под проект.

Я постарался сделать максимально доступно для понимания. Комментарии и все такое. 

Признаюсь - создание первого Makefile под STM32F103C8T6 у меня заняло неделю красноглазиния.
Он не такой, каким выдает его **CubeMX**. все делалось для того, чтоб создать универсальную портянку, дабы имелась возможность быстро создавать проекты под разные мк, а также автоматизировать подхватку этих значений из Makefile с последующей подстановкой их в настройки **.vscode**, используя скрипты на **powershell**. Иначе проект можно несколько часов собирать, что уже перехочется так работать вообще....

**Тестовый проект:** https://github.com/Solderingironspb/VSC-make-example/tree/main/Example_STM32F103_VSCode

Итак, вот пример Makefile под STM32F103C8T6:

```
#   Makefile for STM32F103C8T6
#   Автор: Волков Олег
#   Дата создания скрипта: 26.02.2025
#   GitHub: https://github.com/Solderingironspb
#   Группа Вконтакте: https://vk.com/solderingiron.stm32
#   YouTube: https://www.youtube.com/channel/UCzZKTNVpcMSALU57G1THoVw
#   RuTube: https://rutube.ru/channel/36234184/
#   Яндекс Дзен: https://dzen.ru/id/622208eed2eb4c6d0cd16749

######################################################################################
# Пути для проекта
######################################################################################
# GNU Arm Embedded Toolchain:
GNU_TOOLCHAIN = C:/ST/tools/gnu-tools-for-stm32.12.3/tools/bin
GNU_TOOLCHAIN_GCC_PATH = $(GNU_TOOLCHAIN)/arm-none-eabi-gcc.exe
GNU_TOOLCHAIN_GDB_PATH = $(GNU_TOOLCHAIN)/arm-none-eabi-gdb.exe
GNU_TOOLCHAIN_SIZE_PATH = $(GNU_TOOLCHAIN)/arm-none-eabi-size.exe

# OpenOCD:
OPEN_OCD_PATH = C:/ST/tools/OpenOCD-20240916-0.12.0
OPEN_OCD_BIN_PATH = $(OPEN_OCD_PATH)/bin/openocd.exe

# OpenOCD, кофигурационные файлы под St-Link и микроконтроллер:
OPEN_OCD_INTERFACE_PATH = $(OPEN_OCD_PATH)/share/openocd/scripts/interface/stlink.cfg
OPEN_OCD_TARGET_PATH = $(OPEN_OCD_PATH)/share/openocd/scripts/target/stm32f1x.cfg

# SVD файл для описания периферии микроконтроллера
SVD_FILE_PATH = C:/ST/tools/cmsis-svd-stm32/stm32f1/STM32F103.svd

#####################################################################################
# Название проекта
#####################################################################################
TARGET = Clean_project

#####################################################################################
# Build path
#####################################################################################
BUILD_DIR = Debug

#####################################################################################
# Source location (Вводим через пробел, либо с новой строки, но с знаком '\' в конце)
#####################################################################################
# GNU C 
C_SOURCES = \
Core/Src/main.c \
Core/Src/stm32f103xx_CMSIS.c \
Core/Src/stm32f103xx_it.c \
Core/Src/syscalls.c \
Core/Src/sysmem.c \
Core/Src/system_stm32f1xx.c
# Assembly (Обратите внимание, почему-то STM32 выпускает ASM файлы, 
# то с расширением *.S, то c расширением *.s). Это важно. 
# Для унификации переименуйте в *.s (с маленькой буквой)
ASM_SOURCES = \
Core/Startup/startup_stm32f103c8tx.s

#####################################################################################
# Include location (Вводим через пробел, либо с новой строки, но с знаком '\' в конце)
# Пример: C_INCLUDES = Core/Inc Drivers/CMSIS 
#####################################################################################
# C includes
C_INCLUDES = \
Core/Inc/ \
Drivers/CMSIS/
# Assembly includes
AS_INCLUDES = 

#####################################################################################
# MCU_Settings
#####################################################################################

CPU = -mcpu=cortex-m3
INSTRUCTION_SET = -mthumb
FPU =
FLOAT_ABI = -mfloat-abi=soft

MCU = $(CPU) $(INCTRUCTION_SET) $(FPU) $(FLOAT_ABI) -specs=nano.specs

#####################################################################################
# MCU_GCC_Compiler (CFLAGS) (ASFLAGS)
#####################################################################################

# Defines (Записываются через пробел. Пример: C_DEFS =  DEBUG STM32F103xB)
# GNU C
C_DEFS =  DEBUG STM32F103xB
# Assembly
AS_DEFS = DEBUG

# Language standard
LANG_STD = -std=gnu11

# Optimization
#(None "-O0", Optimize for Debug "-Og", Optimize "-O1", Optimize more "-O2",
# Optimize most "-O3", Optimize for size "-Os", Optimize for speed "-Ofast") 
OPT = -O0 

# Debug level(None " ", Minimal "-g1", Default "-g", Maximum "-g3" )
DEBUG = -g3

# compile gcc flags
ASFLAGS = $(MCU) $(DEBUG) $(OPT) $(addprefix -, $(AS_DEFS)) $(AS_INCLUDES) 
CFLAGS = $(MCU) $(DEBUG) $(OPT) $(LANG_STD) $(addprefix -D, $(C_DEFS)) $(addprefix -I, $(C_INCLUDES))  -Wall -fdata-sections -ffunction-sections -fstack-usage

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
ASFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"

#####################################################################################
# Binaries
#####################################################################################
PREFIX = arm-none-eabi-

ifdef GNU_TOOLCHAIN
CC = $(GNU_TOOLCHAIN)/$(PREFIX)gcc
AS = $(GNU_TOOLCHAIN)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GNU_TOOLCHAIN)/$(PREFIX)objcopy
SZ = $(GNU_TOOLCHAIN)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#####################################################################################
# LDFLAGS
#####################################################################################
# link script
LDSCRIPT = \
STM32F103C8TX_FLASH.ld

# libraries
LIBS = -Wl,--start-group -lc -lm -Wl,--end-group
LIBDIR = 
LDFLAGS = $(MCU) -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#####################################################################################
# build the application (Можно не трогать этот участок)
#####################################################################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(ASFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		


##########################################################################
# update json files (Обновить JSON файлы в соответствие с данным Makefile)
##########################################################################

update-json:
	powershell.exe -executionpolicy bypass -File .vscode/ps_scripts/Makefile_update_json.ps1 "$(TARGET)" "$(BUILD_DIR)" "$(C_INCLUDES)" "$(C_DEFS)" "$(GNU_TOOLCHAIN_GCC_PATH)" "$(CFLAGS)" "$(GNU_TOOLCHAIN_GDB_PATH)" "$(OPEN_OCD_BIN_PATH)" "$(OPEN_OCD_INTERFACE_PATH) $(OPEN_OCD_TARGET_PATH)" "$(SVD_FILE_PATH)" "$(GNU_TOOLCHAIN_SIZE_PATH)"

##########################################################################
# clean up (for Windows)
##########################################################################
clean:
	rmdir /S /Q $(BUILD_DIR)

#########################################################################
# OpenOCD (Записать прошивку на мк и очистить память мк)
##########################################################################
flash: all
	$(OPEN_OCD_BIN_PATH) -f $(OPEN_OCD_INTERFACE_PATH) -f $(OPEN_OCD_TARGET_PATH) -c "program $(BUILD_DIR)/$(TARGET).elf verify reset exit"

erase: 
	$(OPEN_OCD_BIN_PATH) -f $(OPEN_OCD_INTERFACE_PATH) -f $(OPEN_OCD_TARGET_PATH) -c "init; reset halt; stm32f1x mass_erase 0; reset run; exit"
##########################################################################
# dependencies
##########################################################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
```


Эта портянка будет основой для работы с другими микроконтроллерами. В нее я зашил пути, скрипты на выполнение, но об этом чуточку ниже.

С путями все понятно, у нас все в одной папке. Указываем пути и готово. Только обратите внимание, что путь в windows идет с обратным слешем `\`, а здесь с прямым `/`.
Далее нужно заполнить пути для каждого *.c файла, *.s файла, указать все пути для папок include. Представляете сколько гемороя это вводить руками? И тут на помощь приходит мой скрипт

 **Find files for Makefile v1.0.ps1** - скрипт для поиска файлов.

 Просто закидываем в папку с проектом и запускаем. Получим вот такую красоту:

 ![Image](https://github.com/user-attachments/assets/0a84ba19-c7d4-4a16-88d3-76bf658ce1c1)

Вот пример вывода с проектом под HAL для STM32F411. Рукамми вводить это кошмар...

![Image](https://github.com/user-attachments/assets/c764cc3a-a87a-4dca-8596-551797d9cef2)

Кстати, обратите внимание, что у нас должен в Makefile быть подключен только один *.ld файл. Они обычно делают их несколько, чтоб можно было записывать что-то во FLASH или RAM, допустим, чтоб быстрее отлаживать проетк.

Далее смотрим наш Makefile: MCU_Settings, MCU_GCC_Compiler (CFLAGS) (ASFLAGS), LDFLAGS можем подсмотреть в CubeIDE.
Для этого открываем проект в CubeIDE->Project->Properties->C/C++ Build->Settings и смотрим настройки проекта. Думаю разберетесь, если начнете сравнивать с тем, что у меня написано.

![Image](https://github.com/user-attachments/assets/e35fb4e2-cbba-43d2-b23c-e1fdb4912739)

Сохраняем и пробуем забилдить. Хоть прямо в powershell: `make -j4 all`

![Image](https://github.com/user-attachments/assets/f06fded1-f206-4de8-8e6d-b40449924c61)

`-j4` в данном случае - это количество потоков на машине, которой я собираю проект. Нужно это для более быстрой компиляции в параллельном режиме. На Ryzen 5 3600 допустим 12 потоков, я тогда буду использовать `-j12`

Итог компиляции, если все верно:

![Image](https://github.com/user-attachments/assets/2794426a-6a0e-48f9-8f1d-e5a9367573a8)

Как мы видим - **make** от **MinGW** справился с задачей на отлично. 

Попробуем очистить проект и используем **make** тот, что свиснули у **CubeIDE 1.17.0**, но перед этим очистим забилденное.

Очистку проекта я решаю координально - просто удаляю папку Debug.

![Image](https://github.com/user-attachments/assets/6f27b6b7-f949-4136-8593-0c08b8c73262)

**make** от **CubeIDE 1.17.0** с использование прямого пути:

![Image](https://github.com/user-attachments/assets/e1e464e3-a355-4950-887f-3a5d805e7554) 


Поздравляю. Вы теперь можете забилдить проект прямо в **powershell**.

Наполненность папки Debug: 

![Image](https://github.com/user-attachments/assets/fc49427d-ac8c-418e-b67d-437046617afd)

В окне powershell, когда происходит успешный билд - есть строки:

```
   text    data     bss     dec     hex filename
   5668      92    1492    7252    1c54 Debug/Clean_project.elf
```

Это размер нашей прошивки. Как привести это все к удобоваримому виду, как в CubeIDE?

![Image](https://github.com/user-attachments/assets/59dc1f77-fc3f-433e-b467-db627a5ac59a)

Зайдем в папку с скомпилированным проектом, т.е. в папку Debug:

Откроем powershell и выполним команду: `C:\ST\tools\gnu-tools-for-stm32.12.3\tools\bin\arm-none-eabi-size -A Clean_project.elf`

![Image](https://github.com/user-attachments/assets/63c0b8e7-8f25-419c-a05d-5deb75d2e036)

Вот это более детальная информация о занимаемой памяти. Теперь как понять, что и куда? У нас есть *.ld файл в проекте. Нужно смотреть в него (STM32F103C8TX_FLASH.ld):

Вот размер RAM и FLASH. Здесь же размер heap и stack:

```
_Min_Heap_Size = 0x100; /* required amount of heap */
_Min_Stack_Size = 0x200; /* required amount of stack */

/* Memories definition */
MEMORY
{
  RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 20K
  FLASH    (rx)    : ORIGIN = 0x8000000,   LENGTH = 64K
}
```

далее идет портянка по типу:
```
/* Sections */
SECTIONS
{
  /* The startup code into "FLASH" Rom type memory */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >FLASH

  /* The program code and other data into "FLASH" Rom type memory */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >FLASH
```

Все что >FLASH идет во FLASH, все что в >RAM - соответственно в RAM. Можно себя также по CubeIDE проверить:

![Image](https://github.com/user-attachments/assets/5be497c0-6b0d-40ab-aaf7-b9e707127d42)

Соответственно, после несложных математических операций мы сможем посчитать общий объем занимаемой памяти во FLASH и RAM. 

Start address и End address можно глянуть в *.map файле в той же папке Debug:

 ![Image](https://github.com/user-attachments/assets/25fc8602-d1bf-40a5-a653-4cdecdac3b1e)

Start address - Это Origin

End address - Это Origin + Length

Теперь мы знаем, как рассчитать занимаемую память после компиляции. 

Я написал под это скрипт: **Build_Analyzer.ps1**

![Image](https://github.com/user-attachments/assets/0ecae9ef-1294-4202-b909-2104f8d284b4)

Его так не совсем удобно использовать. В VSCode он будет запускаться автоматом:

![Image](https://github.com/user-attachments/assets/b15a0125-5abe-48cf-9a09-e78106a57b5c)

Единственное что - придется под разные микроконтроллеры этот скрипт править, т.к. *.ld файлы у всех разные, да и бывает кроме RAM и FLASH еще куча всего.

**Материал находится в доработке*
