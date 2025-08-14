/* Copyright 2022, Laboratorio de Microprocesadores
 * Facultad de Ciencias Exactas y Tecnología
 * Universidad Nacional de Tucuman
 * http://www.microprocesadores.unt.edu.ar/
 * Copyright 2022, Esteban Volentini <evolentini@herrera.unt.edu.ar>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** \brief Simple sample of use LPC HAL gpio functions
 **
 ** \addtogroup samples Sample projects
 ** \brief Sample projects to use as a starting point
 ** @{ */

/* === Headers files inclusions =============================================================== */

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "key.h"
#include "timeMEF.h"
#include "Mybsp.h"
#include "chip.h"
#include "clock.h"
#include <stdbool.h>

/* === Macros definitions ====================================================================== */

#define TECLA_ACCEPT    KEY_EVENT_KEY_0
#define TECLA_CANCEL    KEY_EVENT_KEY_1
#define TECLA_INCREMENT KEY_EVENT_KEY_2
#define TECLA_DECREMENT KEY_EVENT_KEY_3
#define TECLA_SET_TIME  KEY_EVENT_KEY_4
#define TECLA_SET_ALARM KEY_EVENT_KEY_5

/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/* === Public function implementation ========================================================= */

int main(void) {
    EventGroupHandle_t keys_events;
    BaseType_t result;

    board = BoardCreate();
    clock = ClockCreate();

    keys_events = xEventGroupCreate();

    clock_time_t hora;

    bool valid_time;
    bool flanco_increment = true;
    bool flanco_decrement = true;
    bool flanco_accept = true;
    bool flanco_cancel = true;
    bool alarm_is_active = false;

    DigitalOutputDeactivate(board->led_R);

    if (keys_events) {
        key_task_args_t key_args = malloc(sizeof(*key_args));
        key_args->event_group = keys_events;
        key_args->event_bit = TECLA_ACCEPT;
        key_args->gpio = board->accept;
        result = xTaskCreate(KeyTask, "KeyAccept", KEY_TASK_STACK_SIZE, key_args, tskIDLE_PRIORITY + 1, NULL);
    }
    if (result == pdPASS) {
        key_task_args_t key_args = malloc(sizeof(*key_args));
        key_args->event_group = keys_events;
        key_args->event_bit = TECLA_CANCEL;
        key_args->gpio = board->cancel;
        result = xTaskCreate(KeyTask, "KeyCancel", KEY_TASK_STACK_SIZE, key_args, tskIDLE_PRIORITY + 1, NULL);
    }
    if (result == pdPASS) {
        key_task_args_t key_args = malloc(sizeof(*key_args));
        key_args->event_group = keys_events;
        key_args->event_bit = TECLA_INCREMENT;
        key_args->gpio = board->increment;
        result = xTaskCreate(KeyTask, "KeyIncrement", KEY_TASK_STACK_SIZE, key_args, tskIDLE_PRIORITY + 1, NULL);
    }
    if (result == pdPASS) {
        key_task_args_t key_args = malloc(sizeof(*key_args));
        key_args->event_group = keys_events;
        key_args->event_bit = TECLA_DECREMENT;
        key_args->gpio = board->decrement;
        result = xTaskCreate(KeyTask, "KeyDecrement", KEY_TASK_STACK_SIZE, key_args, tskIDLE_PRIORITY + 1, NULL);
    }
    if (result == pdPASS) {
        key_task_args_t key_args = malloc(sizeof(*key_args));
        key_args->event_group = keys_events;
        key_args->event_bit = TECLA_SET_TIME;
        key_args->gpio = board->set_time;
        result = xTaskCreate(KeyTask, "KeySetTime", KEY_TASK_STACK_SIZE, key_args, tskIDLE_PRIORITY + 1, NULL);
    }
    if (result == pdPASS) {
        key_task_args_t key_args = malloc(sizeof(*key_args));
        key_args->event_group = keys_events;
        key_args->event_bit = TECLA_SET_ALARM;
        key_args->gpio = board->set_alarm;
        result = xTaskCreate(KeyTask, "KeySetAlarm", KEY_TASK_STACK_SIZE, key_args, tskIDLE_PRIORITY + 1, NULL);
    }
    if (result == pdPASS) {
        time_task_args_t time_args = malloc(sizeof(*time_args));
        time_args->event_group = keys_events;
        time_args->accept = TECLA_ACCEPT;
        time_args->cancel = TECLA_CANCEL;
        time_args->increment = TECLA_INCREMENT;
        time_args->decrement = TECLA_DECREMENT;
        time_args->set_time = TECLA_SET_TIME;
        time_args->set_alarm = TECLA_SET_ALARM;
        result = xTaskCreate(MEFTask, "MEF", 2 * configMINIMAL_STACK_SIZE, time_args, tskIDLE_PRIORITY + 1, NULL);
    }

    vTaskStartScheduler();
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
