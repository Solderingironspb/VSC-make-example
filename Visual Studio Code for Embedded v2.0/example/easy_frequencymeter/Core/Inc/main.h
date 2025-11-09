#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stm32f1xx.h>
#include "stm32f103xx_CMSIS.h"

#define TICK_1_HZ    72005934.0f  // Сколько тиков в 1 герце замеряющего таймера
#define TIMEOUT_0_HZ 1000         // 1000мс - это 1 Гц

struct Input_capture_frequencymeter_struct {
    volatile uint32_t TIM_OVF;                // Счетчик переполнения
    volatile uint32_t Input_capture[2];       // Захваченные значения в прерывании input capture. 0 элемент - это первый захват, 1 элемент - это второй захват
    volatile uint8_t flag_is_first_captured;  // Флаг первого захвата
    volatile uint32_t Delta_ticks;            // Разница в тиках между захватами
    volatile float Frequency;                 // Рассчитанная частота
    volatile uint32_t Timer_0_Hz;             // Таймаут, который сработает, если сигнал будет меньше 0.5 Гц. Тогда частота отобразится, как 0 Гц.
};

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
