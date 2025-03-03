## Примеры работ по сборке и отладке проектов для Windows и микроконтроллеров Stm32, в редакторе кода Visual Studio Code на языке Си.
*Все проекты полноценно собираются и разрабатываются только под системой **Windows**, т.к. используют скрипты на powershell.*

**Обязательно измените политику запуска скриптов Powersheell.**

**Если не хотите этого делать, то придется каждый раз запускать скрипт по типу:**

`Powershell.exe -executionpolicy bypass -File "C:\script.ps1" %name%`

**Как изменить политику запуска скриптов на powershell?**

*Сначала узнаем, какая политика сейчас:* `Get-ExecutionPolicy`

![Image](https://github.com/user-attachments/assets/1c34a3eb-05c7-4095-a49c-7103f3a8f204)

*Если Restricted, то можно заменить ее на **Remotesigned** или **Bypass**:*

*Команда:* `Set-ExecutionPolicy -Scope CurrentUser -ExecutionPolicy Bypass`

![Image](https://github.com/user-attachments/assets/a92c1635-25d5-4ffd-a2f1-1b399c33f9c4)

## Как начать работу? Больше касается разработки ПО под Windows на Си, но отсюда мы сможем достать свежий make.
Для начала нам понадобится утилита **make**. 
**make** — это утилита, автоматизирующая процесс преобразования файлов из одной формы в другую. Чаще всего это компиляция исходного кода в объектные файлы и последующая компоновка в исполняемые файлы или библиотеки.

Последнее, что я смог найти на сегодняшний день - это **GNU Make 4.3 от 2020 года**.
Находится он сразу с набором инструментов **MinGW**.
Скачать можно здесь *(gcc-12.3.0-
64.exe):* https://www.equation.com/servlet/equation.cmd?fa=fortran

Далее установим в раздел C:\MinGW

Должно получиться следующее:
![Image](https://github.com/user-attachments/assets/cbf99b8e-f49c-4fc8-b46c-855e1feb5f9f)

Далее уже по своему вкусу и уму смотрите, нужно ли Вам добавлять папку bin в переменные среды PATH или же нет. Почему возникает этот вопрос - все просто. Бывает на одной системе приходится компилировать несколько проектов под разное железо, где требуется разный make, gcc, gdb, size, objcopy и т.п.
Я папку bin добавлю в PATH, чтоб использовать MinGW для компиляции проектов на си под Windows, но последующие установки gcc-arm-none-eabi и OpenOCD буду использовать по прямому пути, без добавления в PATH, т.к. данные утилиты под разные фирмы мк обычно модернизируются производителями и лучше использовать фирмовые ~~но это не точно~~.

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
Современный плагин **Cortex-Debug** для Visual Studio Code будет ругаться на версию, т.к. ему нужно > 9.
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

**openocd** есть по адресу:
`C:\ST\STM32CubeIDE_1.17.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.openocd.win32_2.4.0.202409170845`

![Image](https://github.com/user-attachments/assets/14736a9e-9633-4e3e-9654-c6a59ae1c790)

Но он размазан по куче папок, увидел на сайте https://gnutoolchains.com/arm-eabi/openocd/ эту же версию, взял оттуда.

**make в папке CubeIDE 1.17.0 тоже есть, можем его использовать, либо из MinGW, что указывал ранее, разницы не заметил**:

`C:\ST\STM32CubeIDE_1.17.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.make.win32_2.2.0.202409170845`

![image](https://github.com/user-attachments/assets/ae003f6a-c5c7-444b-b08f-3693bf3cdb7a)

## Interface и target.
Итак, make, arm-none-eabi и OpenOCD (скачанный с https://gnutoolchains.com/arm-eabi/openocd/) у нас есть. 

Теперь для того, чтоб подключиться к тому же stm32 - нужен конфиг интерфейса ST-link и таргет микроконтроллера stm32.

У меня это все теперь находится здесь:
`C:\ST\tools\OpenOCD-20240916-0.12.0\share\openocd\scripts`

![Image](https://github.com/user-attachments/assets/fc8c0ec5-d877-4132-8342-3484ad121d79)

Кто ранее по моим видео работал в **VisualGDB** - думаю узнают структуру папок.

Для примера у нас STM32F103C8T6 и st-link v2

**interface:**
`C:\ST\tools\OpenOCD-20240916-0.12.0\share\openocd\scripts\interface\stlink.cfg`

**target:**
`C:\ST\tools\OpenOCD-20240916-0.12.0\share\openocd\scripts\target\stm32f1x.cfg`

Напомню, что у нас есть копии Stm32F103C8T6 и там ChipID другой. Меняется в target файле:
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

Нам еще для дебага, чтоб использовать регистры периферии, понадобится svd файл под наш микроконтроллер. Взять можно здесь: https://github.com/Solderingironspb/cmsis-svd-stm32

Собираем папку со всем необходимым. Не хочу пользоваться путями из **CubeIDE 1.17.0**, т.к. там черт ногу сломит в этом длинном названии. В названии папок пробелы не используем. Получилось так:

![Image](https://github.com/user-attachments/assets/19dbddca-f59e-4ea4-8180-862db74e86dd)

На этом подготовительные работы закончены. Теперь самое сложно - создать Makefile под проект.

Я постарался сделать максимально доступно для понимания. Комментарии и все такое. 

Признаюсь - создание первого Makefile под STM32F103C8T6 у меня заняло неделю красноглазиния. 

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
# Source location (Папки вводим через пробел).GCC_SIZE_PATHC_INCLUDES
# Пример: SRC_DIRS = Core/Src Drivers/CMSIS ModbusRTU
#####################################################################################
# GNU C 
C_SOURCES = \
Core/Src/main.c \
Core/Src/stm32f103xx_CMSIS.c \
Core/Src/stm32f103xx_it.c \
Core/Src/syscalls.c \
Core/Src/sysmem.c \
Core/Src/system_stm32f1xx.c
# Assembly
ASM_SOURCES = \
Core/Startup/startup_stm32f103c8tx.s

#####################################################################################
# Include location (Папки вводим через пробел).
# Пример: C_INCLUDES = Core/Inc Drivers/CMSIS 
#####################################################################################
# C includes
C_INCLUDES = Core/Inc Drivers/CMSIS 
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
CFLAGS = $(MCU) $(DEBUG) $(OPT) $(LANG_STD) $(addprefix -D, $(C_DEFS)) $(addprefix -I, $(C_INCLUDES))  -Wall -fdata-sections -ffunction-sections

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
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#####################################################################################
# build the application
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

##########################################################################
# OpenOCD
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


**Материал находится в доработке*
