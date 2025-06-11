/*********************************************************************************************************************
Copyright (c) 2025, Martín Fernando Gareca del autor <mfgareca36@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*********************************************************************************************************************/

/** @file bsp.c
 ** @brief Implementación de funciones y macros para el control de pines digitales
 **/

/* === Headers files inclusions ==================================================================================== */

#include "bsp.h"
#include "CIAA.h"
#include "chip.h"
#include "poncho.h"
#include <stddef.h>
#include <stdlib.h>

/* === Macros definitions ========================================================================================== */

/* === Private data type declarations ============================================================================== */

/* === Private function declarations =============================================================================== */

void DigitsTurnOff(void) {
    Chip_GPIO_ClearValue(LPC_GPIO_PORT, DIGITS_GPIO, DIGITS_MASK);
    Chip_GPIO_ClearValue(LPC_GPIO_PORT, SEGMENTS_GPIO, SEGMENTS_MASK);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_P_GPIO, SEGMENT_P_BIT, false); // Apagar el punto decimal
}

void SegmentsUpdate(uint8_t value) {
    Chip_GPIO_SetValue(LPC_GPIO_PORT, SEGMENTS_GPIO, (value & SEGMENTS_MASK));
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_P_GPIO, SEGMENT_P_BIT, (value & SEGMENT_P));
}

void DigitTurnOn(uint8_t digit) {
    Chip_GPIO_SetValue(LPC_GPIO_PORT, DIGITS_GPIO, (1 << (3 - digit)) & DIGITS_MASK);
}

/* === Private variable definitions ================================================================================ */

static const struct screen_driver_s screen_driver = {
    .DigitsTurnOff = DigitsTurnOff,   // Función para apagar todos los dígitos
    .SegmentsUpdate = SegmentsUpdate, // Función para actualizar los segmentos
    .DigitTurnOn = DigitTurnOn        // Función para encender un dígito específico
};

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */

void DigitalInit(void) {
    Chip_SCU_PinMuxSet(DIGIT_1_PORT, DIGIT_1_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_1_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_1_GPIO, DIGIT_1_BIT, false); // Inicializar el pin en estado bajo
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, DIGIT_1_GPIO, DIGIT_1_BIT, true);

    Chip_SCU_PinMuxSet(DIGIT_2_PORT, DIGIT_2_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_2_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_2_GPIO, DIGIT_2_BIT, false); // Inicializar el pin en estado bajo
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, DIGIT_2_GPIO, DIGIT_2_BIT, true);

    Chip_SCU_PinMuxSet(DIGIT_3_PORT, DIGIT_3_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_3_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_3_GPIO, DIGIT_3_BIT, false); // Inicializar el pin en estado bajo
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, DIGIT_3_GPIO, DIGIT_3_BIT, true);

    Chip_SCU_PinMuxSet(DIGIT_4_PORT, DIGIT_4_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_4_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_4_GPIO, DIGIT_4_BIT, false); // Inicializar el pin en estado bajo
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, DIGIT_4_GPIO, DIGIT_4_BIT, true);
}

void SegmentsInit(void) {
    Chip_SCU_PinMuxSet(SEGMENT_A_PORT, SEGMENT_A_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_A_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_A_GPIO, SEGMENT_A_BIT, false); // Inicializar el pin en estado bajo
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_A_GPIO, SEGMENT_A_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_B_PORT, SEGMENT_B_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_B_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_B_GPIO, SEGMENT_B_BIT, false); // Inicializar el pin en estado bajo
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_B_GPIO, SEGMENT_B_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_C_PORT, SEGMENT_C_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_C_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_C_GPIO, SEGMENT_C_BIT, false); // Inicializar el pin en estado bajo
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_C_GPIO, SEGMENT_C_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_D_PORT, SEGMENT_D_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_D_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_D_GPIO, SEGMENT_D_BIT, false); // Inicializar el pin en estado bajo
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_D_GPIO, SEGMENT_D_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_E_PORT, SEGMENT_E_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_E_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_E_GPIO, SEGMENT_E_BIT, false); // Inicializar el pin en estado bajo
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_E_GPIO, SEGMENT_E_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_F_PORT, SEGMENT_F_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_F_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_F_GPIO, SEGMENT_F_BIT, false); // Inicializar el pin en estado bajo
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_F_GPIO, SEGMENT_F_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_G_PORT, SEGMENT_G_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_G_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_G_GPIO, SEGMENT_G_BIT, false); // Inicializar el pin en estado bajo
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_G_GPIO, SEGMENT_G_BIT, true);

    Chip_SCU_PinMuxSet(SEGMENT_P_PORT, SEGMENT_P_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_P_FUNC);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_P_GPIO, SEGMENT_P_BIT, false); // Inicializar el pin en estado bajo
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, SEGMENT_P_GPIO, SEGMENT_P_BIT, true);
}

/* === Public function definitions ================================================================================= */

board_t BoardCreate(void) {
    struct board_s * board = malloc(sizeof(struct board_s));
    if (board != NULL) {
        DigitalInit();  // Inicializar pines de dígitos
        SegmentsInit(); // Inicializar pines de segmentos
        board->screen = ScreenCreate(4, &screen_driver);

        // Inicializar LEDs
        Chip_SCU_PinMuxSet(LED_R_PORT, LED_R_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_R_FUNC);
        board->led_R = DigitalOutputCreate(SHIELD_RGB_RED_GPIO, SHIELD_RGB_RED_BIT, false);

        Chip_SCU_PinMuxSet(LED_G_PORT, LED_G_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_G_FUNC);
        board->led_G = DigitalOutputCreate(SHIELD_RGB_GREEN_GPIO, SHIELD_RGB_GREEN_BIT, false);

        Chip_SCU_PinMuxSet(LED_B_PORT, LED_B_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_B_FUNC);
        board->led_B = DigitalOutputCreate(SHIELD_RGB_BLUE_GPIO, SHIELD_RGB_BLUE_BIT, false);

        // Inicializar entradas digitales
        Chip_SCU_PinMuxSet(KEY_F1_PORT, KEY_F1_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_F1_FUNC);
        board->set_time = DigitalInputCreate(KEY_F1_GPIO, KEY_F1_BIT, false);

        Chip_SCU_PinMuxSet(KEY_F2_PORT, KEY_F2_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_F2_FUNC);
        board->set_alarm = DigitalInputCreate(KEY_F2_GPIO, KEY_F2_BIT, false);

        Chip_SCU_PinMuxSet(KEY_F3_PORT, KEY_F3_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_F3_FUNC);
        board->decrement = DigitalInputCreate(KEY_F3_GPIO, KEY_F3_BIT, false);

        Chip_SCU_PinMuxSet(KEY_F4_PORT, KEY_F4_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_F4_FUNC);
        board->increment = DigitalInputCreate(KEY_F4_GPIO, KEY_F4_BIT, false);

        Chip_SCU_PinMuxSet(KEY_ACCEPT_PORT, KEY_ACCEPT_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_ACCEPT_FUNC);
        board->accept = DigitalInputCreate(KEY_ACCEPT_GPIO, KEY_ACCEPT_BIT, false);

        Chip_SCU_PinMuxSet(KEY_CANCEL_PORT, KEY_CANCEL_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_CANCEL_FUNC);
        board->cancel = DigitalInputCreate(KEY_CANCEL_GPIO, KEY_CANCEL_BIT, false);
    }
    return board;
}

/* === Public function implementation ==============================================================================
 */

/* === End of documentation ========================================================================================
 */
