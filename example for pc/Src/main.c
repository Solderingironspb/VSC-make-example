#include "main.h"
#include "rtd_calculator.h"

int choice; //Выбор
float number; //Ввод числа
bool result; //

void Check_input_number(void) {
    while (1) {
        printf("Подсказка: (Введите число c плавающей точкой. Используйте точку, а не запятую.): ");
        result = scanf("%f", &number);
        if (result == TRUE) {
            break;
        } else {
            printf("Ошибка: введено некорректное значение. Пожалуйста, попробуйте снова.\n");
            while (getchar() != '\n');  // Очищаем буфер ввода
        }
    }
}

int main(void) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    printf("Volkov Oleg 2025\nPT100 Example calculator\nPt100 (385)\n");
    
    while (1) { 
        printf("Введите команду:\n");
        printf("1 - Расчет сопротивления по температуре\n");
        printf("2 - Расчет температуры по сопротивлению\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);  

        if (choice == 1) {
            printf("Вы выбрали \"Расчет сопротивления по температуре\"\nВведите сопротивление в (Ом)\n");
            Check_input_number();
            printf("Расчитанное сопротивление: %f (Ом)\r\n", Get_Resistance_PT(number, PT100_R0, PT_A_385));
            break;
        } else if (choice == 2) {
            printf("Вы выбрали \"Расчет температуры по сопротивлению\"\nВведите температуру в (°C)\n");
            Check_input_number();
            printf("Расчитанная температура: %f (°С)\r\n", Get_Temperature_PT(number, PT100_R0, PT_A_385));
            break;
        } else {
            printf("Неверный ввод. Пожалуйста, введите 1 или 2.\n");
        }
    }

    return 0;
}
