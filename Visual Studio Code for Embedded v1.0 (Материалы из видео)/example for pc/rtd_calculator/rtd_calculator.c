/**
 ******************************************************************************
 *  @file rtd_calculator.c
 *  @brief Калькулятор: термопреобразователи сопротивления из платины,
 *  меди и никеля (ГОСТ 6651-2009)
 *  @author Волков Олег
 *  @date 23.07.2023
 ******************************************************************************
 * @attention
 *
 * Расчеты ведутся согласно ГОСТ 6651-2009
 * Поддерживаются следующие ТС:
 *  - Pt100 (385)
 *  - Pt500 (385)
 *  - Pt1000 (385)
 *  - 50П (391)
 *  - 100П (391)
 *  - 50М (428)
 *  - 100М (428)
 *  - 100Н (617)
 *  - 500Н (617)
 *  - 1000Н (617)
 *
 * Разрешение:
 *  - Платиновые ТС и ЧЭ: от -200 °C до 850 °С
 *  - Медные ТС и ЧЭ: от -180 °C до 200 °С
 *  - Никелевые ТС и ЧЭ: от -60 °C до 180 °С
 * Диапазон сопротивлений:
 *  - Pt100: от 18.52 Ом до 390.48 Ом
 *  - Pt500: от 92.60 Ом до 1952.41 Ом
 *  - Pt1000: от 185.20 Ом до 3904.81 Ом
 *  - 50П: от 8.62 Ом до 197.58 Ом
 *  - 100П: от 17.24 Ом до 395.16 Ом
 *  - 50М: от 10.26 Ом до 92.80 Ом
 *  - 100М: от 20.53 Ом до 185.60 Ом
 *  - 100Н: от 69.45 Ом до 223.21 Ом
 *  - 500Н: от 347.27 Ом до 1116.03 Ом
 *  - 1000Н: от 694.54 Ом до 2232.06 Ом
 *
 *  ГОСТ: https://docs.cntd.ru/document/1200082733
 *  GitHub: https://github.com/Solderingironspb
 *  Моя программа для ПК для расчета CRC-8, CRC-16, CRC-32 + мини-программы(Данный калькулятор там тоже есть):
 *  https://github.com/Solderingironspb/CRC-Calculator-by-Solderingiron/tree/main
 ******************************************************************************
 */

#include "rtd_calculator.h"

// Коэффициенты из ГОСТ 6651-2009(Платиновые ТС И ЧЭ, 0.00385°С^-1)
double PT_D_385[4] = {255.819, 9.14550, -2.92363, 1.79090};

// Коэффициенты из ГОСТ 6651-2009(Платиновые ТС и ЧЭ, 0,00391°С^-1)
double PT_D_391[4] = {251.903, 8.80035, -2.91506, 1.67611};

// Коэффициенты из ГОСТ 6651-2009(Медные ТС и ЧЭ, 0,00428°С^-1)
double M_D_428[4] = {233.87, 7.9370, -2.0062, -0.3953};

// Коэффициенты из ГОСТ 6651-2009(Никелевые ТС и ЧЭ, 0,00617°С^-1)
double N_D_617[3] = {144.096, -25.502, 4.4876};

/*
 **************************************************************************************************
 *  @breif Функция для расчета температуры по сопротивлению термопреобразователей сопротивления
 *  @attention Платиновые ТС и ЧЭ
 *  @param  Resistance - сопротивление в Ом
 *  @param  R0 - сопротивление ТС при 0°C
 *  @param  Type - PT_385 или PT_391
 *  @retval Возвращает преобразованную температуру ТС
 **************************************************************************************************
 */
double Get_Temperature_PT(double Resistance, double R0, uint8_t Type) {
    double Temperature = 0;
    if (Resistance < R0) {
        for (uint8_t i = 1; i <= 4; i++) {
            if (Type == PT_385) {
                Temperature += (PT_D_385[i - 1] * pow((Resistance / R0 - 1), i));
            } else if (Type == PT_391) {
                Temperature += (PT_D_391[i - 1] * pow((Resistance / R0 - 1), i));
            }
        }
    } else {
        if (Type == PT_385) {
            Temperature = (sqrt((pow(PT_A_385, 2) - 4 * PT_B_385 * (1 - Resistance / R0))) - PT_A_385) / (2 * PT_B_385);
        } else if (Type == PT_391) {
            Temperature = (sqrt((pow(PT_A_391, 2) - 4 * PT_B_391 * (1 - Resistance / R0))) - PT_A_391) / (2 * PT_B_391);
        }
    }
    return Temperature;
}

/*
 **************************************************************************************************
 *  @breif Функция для расчета сопротивления по температуре термопреобразователей сопротивления
 *  @attention Платиновые ТС и ЧЭ
 *  @param  Temperature - температура в °C
 *  @param  R0 - сопротивление ТС при 0°C
 *  @param  Type - PT_385 или PT_391
 *  @retval Возвращает преобразованное сопротивление ТС
 **************************************************************************************************
 */
double Get_Resistance_PT(double Temperature, double R0, uint8_t Type) {
    double Resistance = 0;
    if (Temperature < 0) {
        if (Type == PT_385) {
            Resistance = R0 * (1 + PT_A_385 * Temperature + PT_B_385 * pow(Temperature, 2) + PT_C_385 * (Temperature - 100) * pow(Temperature, 3));
        } else if (Type == PT_391) {
            Resistance = R0 * (1 + PT_A_391 * Temperature + PT_B_391 * pow(Temperature, 2) + PT_C_391 * (Temperature - 100) * pow(Temperature, 3));
        }
    } else {
        if (Type == PT_385) {
            Resistance = R0 * (1 + PT_A_385 * Temperature + PT_B_385 * pow(Temperature, 2));
        } else if (Type == PT_391) {
            Resistance = R0 * (1 + PT_A_391 * Temperature + PT_B_391 * pow(Temperature, 2));
        }
    }
    return Resistance;
}

/*
 **************************************************************************************************
 *  @breif Функция для расчета температуры по сопротивлению термопреобразователей сопротивления
 *  @attention Медные ТС и ЧЭ
 *  @param  Resistance - сопротивление в Ом
 *  @param  R0 - сопротивление ТС при 0°C
 *  @param  Type - M_428
 *  @retval Возвращает преобразованную температуру ТС
 **************************************************************************************************
 */
double Get_Temperature_M(double Resistance, double R0, uint8_t Type) {
    double Temperature = 0;
    if (Resistance < R0) {
        for (uint8_t i = 1; i <= 4; i++) {
            if (Type == M_428) {
                Temperature += (M_D_428[i - 1] * pow((Resistance / R0 - 1), i));
            }
        }
    } else {
        if (Type == M_428) {
            Temperature = (Resistance / R0 - 1) / M_A_428;
        }
    }
    return Temperature;
}

/*
 **************************************************************************************************
 *  @breif Функция для расчета сопротивления по температуре термопреобразователей сопротивления
 *  @attention Медные ТС и ЧЭ
 *  @param  Temperature - температура в °C
 *  @param  R0 - сопротивление ТС при 0°C
 *  @param  Type - M_428
 *  @retval Возвращает преобразованное сопротивление ТС
 **************************************************************************************************
 */
double Get_Resistance_M(double Temperature, double R0, uint8_t Type) {
    double Resistance = 0;
    if (Temperature < 0) {
        if (Type == M_428) {
            Resistance = R0 * (1 + M_A_428 * Temperature + M_B_428 * Temperature * (Temperature + 6.7) + M_C_428 * pow(Temperature, 3));
        }
    } else {
        if (Type == M_428) {
            Resistance = R0 * (1 + M_A_428 * Temperature);
        }
    }
    return Resistance;
}

/*
 **************************************************************************************************
 *  @breif Функция для расчета температуры по сопротивлению термопреобразователей сопротивления
 *  @attention Никелевые ТС и ЧЭ
 *  @param  Resistance - сопротивление в Ом
 *  @param  R0 - сопротивление ТС при 0°C
 *  @param  Type - N_617
 *  @retval Возвращает преобразованную температуру ТС
 **************************************************************************************************
 */
double Get_Temperature_N(double Resistance, double R0, uint8_t Type) {
    double Temperature = 0;
    double Temp_start = 0.0f;
    if (Type == N_617) {
        if (R0 == 100) {
            Temp_start = 161.72;
        } else if (R0 == 500) {
            Temp_start = 808.59;
        } else if (R0 == 1000) {
            Temp_start = 1617.2;
        }
        if (Resistance < Temp_start) {
            Temperature = (sqrt((pow(N_A_617, 2) - 4 * N_B_617 * (1 - Resistance / R0))) - N_A_617) / (2 * N_B_617);
        } else {
            for (uint8_t i = 1; i <= 3; i++) {
                Temperature += (N_D_617[i - 1] * pow((Resistance / R0 - 1.6172), i));
            }
            Temperature += 100;
        }
    }
    return Temperature;
}

/*
 **************************************************************************************************
 *  @breif Функция для расчета сопротивления по температуре термопреобразователей сопротивления
 *  @attention Никелевые ТС и ЧЭ
 *  @param  Temperature - температура в °C
 *  @param  R0 - сопротивление ТС при 0°C
 *  @param  Type - N_617
 *  @retval Возвращает преобразованное сопротивление ТС
 **************************************************************************************************
 */
double Get_Resistance_N(double Temperature, double R0, uint8_t Type) {
    double Resistance = 0;
    if (Type == N_617) {
        if (Temperature < 100) {
            Resistance = R0 * (1 + N_A_617 * Temperature + N_B_617 * pow(Temperature, 2));
        } else {
            Resistance = R0 * (1 + N_A_617 * Temperature + N_B_617 * pow(Temperature, 2) + N_C_617 * (Temperature - 100) * pow(Temperature, 2));
        }
    }
    return Resistance;
}
