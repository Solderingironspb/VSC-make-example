CC = C:/MinGW/gcc/bin/gcc.exe

TARGET = Example

BUILD_DIR = Debug

SRC_DIRS = Src rtd_calculator

SRCS = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))

C_INCLUDES = \
-IInc \
-Irtd_calculator

# Объектные файлы
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))

CFLAGS =  -Wall -g3 -std=c11 -O0 $(C_INCLUDES)

# Основная цель
all: $(BUILD_DIR) $(OBJS) $(TARGET)

# Сборка исполняемого файла
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Компиляция исходных файлов в объектные
$(BUILD_DIR)/%.o: %.c
	@mkdir $(subst /,\,$(dir $@)) 2>NUL || exit 0
	$(CC) $(CFLAGS) -c $< -o $@

# Создание директории для сборки, если её нет
$(BUILD_DIR):
	@mkdir $(subst /,\,$(BUILD_DIR)) 2>NUL || exit 0

# Очистка
clean:
	rmdir /S /Q $(BUILD_DIR)
	del /S /Q $(TARGET).exe