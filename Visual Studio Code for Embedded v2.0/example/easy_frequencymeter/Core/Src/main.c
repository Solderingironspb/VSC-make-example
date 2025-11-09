#include "main.h"
#include <stdio.h>

// Создадим структуру для Input Capture частотомера, основанного на таймере 3, канал 1
struct Input_capture_frequencymeter_struct IC_FM_TIM3_CH1;

extern volatile uint32_t SysTimer_ms;         // Переменная, аналогичная HAL_GetTick()
extern volatile uint32_t Delay_counter_ms;    // Счетчик для функции Delay_ms
extern volatile uint32_t Timeout_counter_ms;  // Переменная для таймаута функций

void SysTick_Handler(void) {
    SysTimer_ms++;

    if (Delay_counter_ms) {
        Delay_counter_ms--;
    }
    if (Timeout_counter_ms) {
        Timeout_counter_ms--;
    }

    if (IC_FM_TIM3_CH1.Timer_0_Hz) {
        IC_FM_TIM3_CH1.Timer_0_Hz--;
    } else {
        IC_FM_TIM3_CH1.Frequency = 0;
        IC_FM_TIM3_CH1.TIM_OVF = 0;
    }

#if defined FreeRTOS_USE
    xPortSysTickHandler();
#endif
}

void TIM3_IRQHandler(void) {
    // Обработка события переполнения таймера
    if (READ_BIT(TIM3->SR, TIM_SR_UIF)) {
        CLEAR_BIT(TIM3->SR, TIM_SR_UIF);  // Сбросим флаг переполнения
        IC_FM_TIM3_CH1.TIM_OVF++;         // Увеличиваем счётчик переполнений
    }

    // Обработка события захвата
    if (READ_BIT(TIM3->SR, TIM_SR_CC1IF)) {
        // Зададим таймаут в 2000 мс, чтоб в случае отсутствия сигнала, отобразить частоту в 0 Гц.
        IC_FM_TIM3_CH1.Timer_0_Hz = TIMEOUT_0_HZ;
        CLEAR_BIT(TIM3->SR, TIM_SR_CC1IF);  // Сбросим флаг захвата

        // Поймаем первый захват
        if (IC_FM_TIM3_CH1.flag_is_first_captured == false) {
            // Сохраняем значение и счётчик переполнений
            IC_FM_TIM3_CH1.Input_capture[0] = TIM3->CCR1;  // Сохраним значение, что мы получили при первом захвате
            IC_FM_TIM3_CH1.TIM_OVF = 0;                    // сбросим счетчик переполнений
            IC_FM_TIM3_CH1.flag_is_first_captured = true;
        } else {
            // Поймаем второй захват
            IC_FM_TIM3_CH1.Input_capture[1] = TIM3->CCR1;  // Сохраним значение, что мы получили при втором захвате

            // Рассчитаем, сколько тиков таймера прошло за период захвата, учитывая все переполнения
            uint32_t Total_Ticks = (IC_FM_TIM3_CH1.TIM_OVF * TIM3->ARR) + IC_FM_TIM3_CH1.Input_capture[1];
            // Рассчитаем, сколько по итогу тиков получается
            IC_FM_TIM3_CH1.Delta_ticks = Total_Ticks - IC_FM_TIM3_CH1.Input_capture[0];

            if (IC_FM_TIM3_CH1.Delta_ticks != 0) {
                // Вычислим частоту сигнала
                IC_FM_TIM3_CH1.Frequency = TICK_1_HZ / IC_FM_TIM3_CH1.Delta_ticks;  // Результат в Герцах
            }
            // Сбрасываем флаг для следующего измерения
            IC_FM_TIM3_CH1.flag_is_first_captured = false;
        }
    }
}

void CMSIS_TIM3_Capture_mode(void) {
    SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM3EN);  // Запустим тактирование TIM3

    // Настроим ножку TIM3_CH1(PA6) на вход без подтяжки
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN);                               // Запустим тактирование порта A
    MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF6_Msk, 0b01 << GPIO_CRL_CNF6_Pos);    // Input floating
    MODIFY_REG(GPIOA->CRL, GPIO_CRL_MODE6_Msk, 0b00 << GPIO_CRL_MODE6_Pos);  // Reserved

    // Перейдем к настройке таймера
    TIM3->CR1 = 0;  // На всякий случай сбросим все настройки CR1
    TIM3->CR2 = 0;  // На всякий случай сбросим все настройки CR2

    CLEAR_BIT(TIM3->CR1, TIM_CR1_CEN);                                // Выключим таймер
    CLEAR_BIT(TIM3->CR1, TIM_CR1_UDIS);                               // UEV enabled
    CLEAR_BIT(TIM3->CR1, TIM_CR1_URS);                                // Any of the following events generate an update interrupt or DMA request if enabled.
    CLEAR_BIT(TIM3->CR1, TIM_CR1_OPM);                                // One-pulse mode off
    CLEAR_BIT(TIM3->CR1, TIM_CR1_DIR);                                // Считаем вверх
    MODIFY_REG(TIM3->CR1, TIM_CR1_CMS_Msk, 0b00 << TIM_CR1_CMS_Pos);  // Режим выравнивания по краю
    CLEAR_BIT(TIM3->CR1, TIM_CR1_ARPE);                               // TIMx_ARR register is not buffered
    MODIFY_REG(TIM3->CR1, TIM_CR1_CKD_Msk, 0b00 << TIM_CR1_CKD_Pos);  // Без делителя

    // Настроим предделитель и период
    TIM3->PSC = 0;       // При SysClk = 72 MHz, CK_CNT = 72 MHz
    TIM3->ARR = 0xFFFF;  // Максимальный период для 16-битного таймера

    // Настроим CH1 TIM3 в режим Input Capture:
    TIM3->CCMR1 = 0;                                                          // Очистим регистр полностью
    MODIFY_REG(TIM3->CCMR1, TIM_CCMR1_CC1S_Msk, 0b01 << TIM_CCMR1_CC1S_Pos);  // CC1 channel is configured as input, IC1 is mapped on TI1
    // Настройка предделителя:
    MODIFY_REG(TIM3->CCMR1, TIM_CCMR1_IC1PSC_Msk, 0b01 << TIM_CCMR1_IC1PSC_Pos);  // no prescaler
    /*  This bit-field defines the ratio of the prescaler acting on CC1 input (IC1).
        The prescaler is reset as soon as CC1E=0 (TIMx_CCER register).
        00: no prescaler, capture is done each time an edge is detected on the capture input
        01: capture is done once every 2 events
        10: capture is done once every 4 events
        11: capture is done once every 8 events
    */

    // Настройка фильтра:
    MODIFY_REG(TIM3->CCMR1, TIM_CCMR1_IC1F_Msk, 0b0000 << TIM_CCMR1_IC1F_Pos);
    /*  IC1F: Input capture 1 filter
        This bit-field defines the frequency used to sample TI1 input and the length of the digital filter
        applied to TI1. The digital filter is made of an event counter in which N consecutive events
        are needed to validate a transition on the output:
        0000: No filter, sampling is done at fDTS
        0001: fSAMPLING=fCK_INT, N=2
        0010: fSAMPLING=fCK_INT, N=4
        0011: fSAMPLING=fCK_INT, N=8
        0100: fSAMPLING=fDTS/2, N=6
        0101: fSAMPLING=fDTS/2, N=8
        0110: fSAMPLING=fDTS/4, N=6
        0111: fSAMPLING=fDTS/4, N=8
        1000: fSAMPLING=fDTS/8, N=6
        1001: fSAMPLING=fDTS/8, N=8
        1010: fSAMPLING=fDTS/16, N=5
        1011: fSAMPLING=fDTS/16, N=6
        1100: fSAMPLING=fDTS/16, N=8
        1101: fSAMPLING=fDTS/32, N=5
        1110: fSAMPLING=fDTS/32, N=6
        1111: fSAMPLING=fDTS/32, N=8
    */

    // Разрешим захват по этому каналу
    SET_BIT(TIM3->CCER, TIM_CCER_CC1E);  // Capture enabled.
    // Настроим полярность сигнала захвата
    SET_BIT(TIM3->CCER, TIM_CCER_CC1P);  // non-inverted: capture is done on a rising edge of IC1. When used as external trigger, IC1 is non-inverted.

    // Настроим прерывания
    SET_BIT(TIM3->DIER, TIM_DIER_CC1IE);  // Capture/Compare 1 interrupt enable
    SET_BIT(TIM3->DIER, TIM_DIER_UIE);    // Update interrupt enabled. Будем использовать при обработке переполнения таймера.

    // Настраиваем приоритет и разрешаем прерывание в NVIC
    NVIC_EnableIRQ(TIM3_IRQn);
    NVIC_SetPriority(TIM3_IRQn, 0);

    // Запускаем таймер
    SET_BIT(TIM3->CR1, TIM_CR1_CEN);  // Запустим таймер
}

int main(void) {
    CMSIS_Debug_init();
    CMSIS_RCC_SystemClock_72MHz();
    CMSIS_SysTick_Timer_init();
    CMSIS_TIM3_Capture_mode();

    while (1) {
    }
}