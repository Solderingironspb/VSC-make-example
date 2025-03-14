/*
 * ch32v203xx_it.c
 *
 *  Created on: Aug 17, 2023
 *      Author: Oleg Volkov
 */


 #include "ch32v203xx_it.h"

 extern volatile uint32_t SysTimer_ms; //Переменная, аналогичная HAL_GetTick()
 extern volatile uint32_t Delay_counter_ms; //Счетчик для функции Delay_ms
 extern volatile uint32_t Timeout_counter_ms; //Переменная для таймаута функций
 
 extern struct USART_name husart1; //Объявляем структуру по USART.(см. ch32v203x_RVMSIS.h)
 extern struct USART_name husart2; //Объявляем структуру по USART.(см. ch32v203x_RVMSIS.h)
 extern struct USART_name husart3; //Объявляем структуру по USART.(см. ch32v203x_RVMSIS.h)
 
 /*
  ******************************************************************************
  *  @breif Прерывание по флагу CNTIF
  ******************************************************************************
  */
 
 void SysTick_Handler(void) {
     SysTick->SR &= ~(1 << 0);
     SysTimer_ms++;
 
     if (Delay_counter_ms) {
         Delay_counter_ms--;
     }
     if (Timeout_counter_ms) {
         Timeout_counter_ms--;
     }
 }
 
 /**
  ******************************************************************************
  *  @breif Прерывание по USART1
  ******************************************************************************
  */
 
 void USART1_IRQHandler(void) {
     if (READ_BIT(USART1->STATR, USART_STATR_RXNE)) {
         //Если пришли данные по USART
         if (husart1.rx_counter < USART_MAX_LEN_RX_BUFFER) { //Если байт прилетело меньше, чем размер буфера
             husart1.rx_buffer[husart1.rx_counter] = USART1->DATAR; //Считаем данные в соответствующую ячейку в rx_buffer
             husart1.rx_counter++; //Увеличим счетчик принятых байт на 1
         } else {
             husart1.rx_counter = 0; //Если больше - сбросим счетчик.
         }
     }
     if (READ_BIT(USART1->STATR, USART_STATR_IDLE)) {
         //Если прилетел флаг IDLE
         USART1->DATAR; //Сбросим флаг IDLE
         husart1.rx_len = husart1.rx_counter; //Узнаем, сколько байт получили
         husart1.rx_counter = 0; //сбросим счетчик приходящих данных
     }
 }
 
 /**
  ******************************************************************************
  *  @breif Прерывание по USART2
  ******************************************************************************
  */
 
 void USART2_IRQHandler(void) {
     if (READ_BIT(USART2->STATR, USART_STATR_RXNE)) {
         //Если пришли данные по USART
         if (husart2.rx_counter < USART_MAX_LEN_RX_BUFFER) { //Если байт прилетело меньше, чем размер буфера
             husart2.rx_buffer[husart2.rx_counter] = USART2->DATAR; //Считаем данные в соответствующую ячейку в rx_buffer
             husart2.rx_counter++; //Увеличим счетчик принятых байт на 1
         } else {
             husart2.rx_counter = 0; //Если больше - сбросим счетчик.
         }
     }
     if (READ_BIT(USART2->STATR, USART_STATR_IDLE)) {
         //Если прилетел флаг IDLE
         USART2->DATAR; //Сбросим флаг IDLE
         husart2.rx_len = husart2.rx_counter; //Узнаем, сколько байт получили
         husart2.rx_counter = 0; //сбросим счетчик приходящих данных
     }
 }
 
 /**
  ******************************************************************************
  *  @breif Прерывание по USART3
  ******************************************************************************
  */
 
 void USART3_IRQHandler(void) {
     if (READ_BIT(USART3->STATR, USART_STATR_RXNE)) {
         //Если пришли данные по USART
         if (husart3.rx_counter < USART_MAX_LEN_RX_BUFFER) { //Если байт прилетело меньше, чем размер буфера
             husart3.rx_buffer[husart3.rx_counter] = USART3->DATAR; //Считаем данные в соответствующую ячейку в rx_buffer
             husart3.rx_counter++; //Увеличим счетчик принятых байт на 1
         } else {
             husart3.rx_counter = 0; //Если больше - сбросим счетчик.
         }
     }
     if (READ_BIT(USART3->STATR, USART_STATR_IDLE)) {
         //Если прилетел флаг IDLE
         USART3->DATAR; //Сбросим флаг IDLE
         husart3.rx_len = husart3.rx_counter; //Узнаем, сколько байт получили
         husart3.rx_counter = 0; //сбросим счетчик приходящих данных
     }
 }
 
 /**
  ******************************************************************************
  *  @breif Прерывание по TIM3
  ******************************************************************************
  */
 
 void TIM3_IRQHandler(void) {
     if (READ_BIT(TIM3->INTFR, TIM_UIF)) {
         CLEAR_BIT(TIM3->INTFR, TIM_UIF); //Сбросим флаг прерывания
     }
 }
 