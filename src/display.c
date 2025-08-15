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

/** @file display.c
 ** @brief Implementación de funciones y macros para el refresco de pantalla.
 **/

/* === Headers files inclusions ==================================================================================== */

#include "display.h"

/* === Macros definitions ========================================================================================== */

/* === Private data type declarations ============================================================================== */

/* === Private function declarations =============================================================================== */

/* === Private variable definitions ================================================================================ */

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */

/* === Public function definitions ================================================================================= */

/* === Public function implementation ============================================================================== */

void RefreshScreenTask(void * pointer) {
    // screen_t args = pointer;
    TickType_t last_value = xTaskGetTickCount();

    while (1) {
        ScreenRefresh(pointer);
        xTaskDelayUntil(&last_value, pdMS_TO_TICKS(1));
    }
}

void TickTask(void * pointer) {
    TickType_t last_value = xTaskGetTickCount();

    while (1) {
        ClockNewTick(pointer);
        xTaskDelayUntil(&last_value, pdMS_TO_TICKS(1));
    }
}

/* === End of documentation ======================================================================================== */
