#include "main.h"

uint32_t Counter = 0;
unsigned char Text[256] = {0,};

int main(void) {
    CMSIS_Debug_init();
    CMSIS_RCC_SystemClock_72MHz();
    CMSIS_SysTick_Timer_init();
    CMSIS_USART1_Init(); //115200 8N1
    CMSIS_GPIO_init(GPIOC, 13, GPIO_GENERAL_PURPOSE_OUTPUT, GPIO_OUTPUT_PUSH_PULL, GPIO_SPEED_50_MHZ);
    

    while (1) {
        Counter++;
        sprintf((char*)Text, "Привет мир!\r\nСчетчик досчитал до %ld\r\n", Counter);
        CMSIS_USART_Transmit(USART1, (unsigned char*)&Text, strlen((const char*)Text), 100);
    }
}