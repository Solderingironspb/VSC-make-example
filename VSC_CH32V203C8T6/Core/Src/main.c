#include "main.h"
#include "stdio.h"

uint32_t Counter = 12;

#define DEBUG_USE   //Использовать DEBUG по USART

int main(void) {
    RVMSIS_Debug_init(); //Настройка дебага
    RVMSIS_RCC_SystemClock_144MHz(); //Настройка системной частоты
    RVMSIS_SysTick_Timer_init(); //Настройка системного таймера
    RVMSIS_USART3_Init(); //См. файл syscalls.c USART3 115200 8N1 выбран для отладки через printf. Ножка PB10
    RVMSIS_GPIO_init(GPIOC, 13, GPIO_GENERAL_PURPOSE_OUTPUT, GPIO_OUTPUT_PUSH_PULL, GPIO_SPEED_50_MHZ);
#ifdef DEBUG_USE
    printf("Hello world!\r\n");
#endif

    while(1) {
        GPIOC->BSHR = GPIO_BSHR_BS13;
        Delay_ms(100);
        GPIOC->BSHR = GPIO_BSHR_BR13;
        Delay_ms(100);
        Counter++;
    }
}
