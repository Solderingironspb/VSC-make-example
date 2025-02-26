#include "stm32f103xx_it.h"

extern volatile uint32_t SysTimer_ms;         // Переменная, аналогичная HAL_GetTick()
extern volatile uint32_t Delay_counter_ms;    // Счетчик для функции Delay_ms
extern volatile uint32_t Timeout_counter_ms;  // Переменная для таймаута функций

uint16_t Timeout_blink = 100 - 1;
uint16_t Timer_blink;

bool flag_blink = false;

void SysTick_Handler(void) {
    SysTimer_ms++;

    if (Delay_counter_ms) {
        Delay_counter_ms--;
    }
    if (Timeout_counter_ms) {
        Timeout_counter_ms--;
    }

	/*Blink на прерываниях*/
    if (Timer_blink) {
        Timer_blink--;
    } else {
        flag_blink = !flag_blink;
        if (flag_blink) {
            GPIOC->BSRR = GPIO_BSRR_BS13;
        } else {
            GPIOC->BSRR = GPIO_BSRR_BR13;
        }
        Timer_blink = Timeout_blink;
    }

#if defined FreeRTOS_USE
    xPortSysTickHandler();
#endif
}