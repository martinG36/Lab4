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

/** @file key.c
 ** @brief Implementación de funciones y macros para el control de teclas del reloj.
 **/

/* === Headers files inclusions ==================================================================================== */

#include "key.h"

/* === Macros definitions ========================================================================================== */

#define KEY_TASK_DELAY 100

/* === Private data type declarations ============================================================================== */

/* === Private function declarations =============================================================================== */

/* === Private variable definitions ================================================================================ */

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */

/* === Public function definitions ================================================================================= */

/* === Public function implementation ============================================================================== */

void KeyTask(void * pointer) {
    key_task_args_t args = pointer;

    while (1) {
        while (!DigitalInputGetIsActive(args->gpio)) {
            vTaskDelay(pdMS_TO_TICKS(KEY_TASK_DELAY));
        }

        xEventGroupSetBits(args->event_group, args->event_bit);

        while (DigitalInputGetIsActive(args->gpio)) {
            vTaskDelay(pdMS_TO_TICKS(KEY_TASK_DELAY));
        }
    }
}

void LargeKeyTask(void * pointer) {
    key_task_args_t args = pointer;
    TickType_t pressKey;
    TickType_t xLastWakeTime;

    while (1) {
        while (!DigitalInputGetIsActive(args->gpio)) {
            vTaskDelay(pdMS_TO_TICKS(KEY_TASK_DELAY));
        }

        pressKey = xTaskGetTickCount();
        xLastWakeTime = pressKey;

        while (DigitalInputGetIsActive(args->gpio)) {
            vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10));

            if (xTaskGetTickCount() - pressKey >= pdMS_TO_TICKS(3000)) {
                xEventGroupSetBits(args->event_group, args->event_bit);
            }
        }
    }
}

/* === End of documentation ======================================================================================== */
