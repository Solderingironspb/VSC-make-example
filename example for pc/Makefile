CC = C:/MinGW/gcc/bin/gcc.exe

TARGET = Example

BUILD_DIR = Debug

C_SOURCES = \
rtd_calculator/rtd_calculator.c \
Src/main.c

C_INCLUDES = \
Inc/ \
rtd_calculator/


# Language standard
LANG_STD = -std=gnu11

# Optimization
#(None "-O0", Optimize for Debug "-Og", Optimize "-O1", Optimize more "-O2",
# Optimize most "-O3", Optimize for size "-Os", Optimize for speed "-Ofast") 
OPT = -O0

# Debug level(None " ", Minimal "-g1", Default "-g", Maximum "-g3" )
DEBUG = -g3

# Объектные файлы
OBJECTS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(C_SOURCES)))

# Пути для поиска исходных файлов
vpath %.c $(sort $(dir $(C_SOURCES)))

# Флаги компиляции
CFLAGS = -Wall $(DEBUG) $(LANG_STD) $(OPT) $(addprefix -I, $(C_INCLUDES))

# Основная цель
all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).exe

# Сборка исполняемого файла
$(BUILD_DIR)/$(TARGET).exe: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Компиляция исходных файлов в объектные
$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Создание директории для сборки, если её нет
$(BUILD_DIR):
	mkdir $@	

# Очистка
clean:
	rmdir /S /Q $(BUILD_DIR)