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

#ifndef KEY_H_
#define KEY_H_

/** @file key.h
 ** @brief Declaración de funciones y macros para el control de teclas del reloj.
 **/

/* === Headers files inclusions ==================================================================================== */

#include "FreeRTOS.h"
#include "digital.h"
#include "event_groups.h"

/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

#define KEY_EVENT_KEY_0     (1 << 0)
#define KEY_EVENT_KEY_1     (1 << 1)
#define KEY_EVENT_KEY_2     (1 << 2)
#define KEY_EVENT_KEY_3     (1 << 3)
#define KEY_EVENT_KEY_4     (1 << 4)
#define KEY_EVENT_KEY_5     (1 << 5)
#define KEY_EVENT_KEY_6     (1 << 6)
#define KEY_EVENT_KEY_7     (1 << 7)

#define KEY_EVENT_ANY_KEY   0xFF

#define KEY_TASK_STACK_SIZE (2 * configMINIMAL_STACK_SIZE)

/* === Public data type declarations =============================================================================== */

typedef struct key_task_args_s {
    EventGroupHandle_t event_group;
    uint8_t event_bit;
    digital_input_t gpio;
} * key_task_args_t;

/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */

void KeyTask(void * args);

void LargeKeyTask(void * args);

/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* KEY_H_ */
