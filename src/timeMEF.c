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

/** @file timeMEF.c
 ** @brief Implementación de funciones y macros para la gestión de la MEF del reloj.
 **/

/* === Headers files inclusions ==================================================================================== */

#include "timeMEF.h"
#include <stdbool.h>

/* === Macros definitions ========================================================================================== */

/* === Private data type declarations ============================================================================== */

/* === Private function declarations =============================================================================== */

/* === Private variable definitions ================================================================================ */

typedef enum {
    STATE_SHOW_TIME,
    STATE_ADJUST_TIME_MINUTES,
    STATE_ADJUST_TIME_HOURS,
    STATE_ADJUST_ALARM_MINUTES,
    STATE_ADJUST_ALARM_HOURS,
    STATE_CONTROL_ALARM
} clock_state_t;

/* === Public variable definitions ================================================================================= */

bool adjusting_time = false;

uint32_t set_time_pressed_ticks = 0;
uint32_t last_activity_ticks_time = 0;
clock_time_t time_backup;
clock_time_t editable_time;
clock_time_t editable_alarm;

bool adjusting_alarm = false;
uint32_t set_alarm_pressed_ticks = 0;
uint32_t last_activity_ticks_alarm = 0;

volatile uint32_t ticks = 0;

static clock_state_t current_state = STATE_SHOW_TIME;

/* === Private function definitions ================================================================================ */

/* === Public function definitions ================================================================================= */

/* === Public function implementation ============================================================================== */

void MEFTask(void * pointer) {
    time_task_args_t args = pointer;
    EventBits_t events;

    clock_time_t hora;

    bool valid_time;
    bool flanco_increment = true;
    bool flanco_decrement = true;
    bool flanco_accept = true;
    bool flanco_cancel = true;
    bool alarm_is_active = false;

    DigitalOutputDeactivate(args->board->led_R);

    while (1) {
        xEventGroupClearBits(args->event_group, args->accept | args->cancel | args->increment | args->decrement |
                                                    args->set_time | args->set_alarm);
        events = xEventGroupWaitBits(args->event_group,
                                     args->accept | args->cancel | args->increment | args->decrement | args->set_time |
                                         args->set_alarm,
                                     pdTRUE, pdFALSE, pdMS_TO_TICKS(1));

        switch (current_state) {
            /*-------------------Funcionamiento Normal-------------------------------------------*/
        case STATE_SHOW_TIME:
            valid_time = ClockGetTime(args->clock, &hora);
            ScreenWriteBCD(args->board->screen, hora.bcd, 4);

            if (valid_time) {
                DisplayFlashDigits(args->board->screen, 0, 3, 0);
                DisplayFlashDot(args->board->screen, 0, 100, false);
                DisplayFlashDot(args->board->screen, 1, 100, true);
                DisplayFlashDot(args->board->screen, 2, 100, false);
                if (alarm_is_active) {
                    DisplayFlashDot(args->board->screen, 3, 0, true);
                    ClockSetStateAlarm(args->clock, true);
                    current_state = STATE_CONTROL_ALARM;
                } else {
                    DisplayFlashDot(args->board->screen, 3, 100, false);
                    ClockSetStateAlarm(args->clock, false);
                }
            } else {
                DisplayFlashDigits(args->board->screen, 0, 3, 100);
                DisplayFlashDot(args->board->screen, 1, 100, true);
            }

            if (events & args->set_time) {
                adjusting_time = true;
            } else {
                adjusting_time = false;
            }
            if (events & args->set_alarm) {
                adjusting_alarm = true;
            } else {
                adjusting_alarm = false;
            }

            if (adjusting_time) {
                current_state = STATE_ADJUST_TIME_MINUTES;
            }
            if (adjusting_alarm) {
                current_state = STATE_ADJUST_ALARM_MINUTES;
            }

            if (!(events & args->accept)) {
                flanco_accept = true;
            }
            if (((events & args->accept) && flanco_accept) && !ClockAlarmIsRinging(args->clock)) {
                alarm_is_active = true;
                flanco_accept = false;
            }
            if (!(events & args->cancel)) {
                flanco_cancel = true;
            }
            if (((events & args->cancel) && flanco_cancel) && !ClockAlarmIsRinging(args->clock)) {
                alarm_is_active = false;
                flanco_cancel = false;
            }

            break;

            /*-------------------Puesta en Hora------------------------------------------------*/
        case STATE_ADJUST_TIME_MINUTES:
            if (adjusting_time) {
                ScreenWriteBCD(args->board->screen, editable_time.bcd, 4);
                DisplayFlashDigits(args->board->screen, 2, 3, 100);

                if (!(events & args->increment)) {
                    flanco_increment = true;
                }
                if ((events & args->increment) && flanco_increment) {
                    IncrementMinutes(&editable_time);
                    last_activity_ticks_time = ticks;
                    flanco_increment = false;
                }

                if (!(events & args->decrement)) {
                    flanco_decrement = true;
                }
                if ((events & args->decrement) && flanco_decrement) {
                    DecrementMinutes(&editable_time);
                    last_activity_ticks_time = ticks;
                    flanco_decrement = false;
                }

                if (!(events & args->accept)) {
                    flanco_accept = true;
                }
                if ((events & args->accept) && flanco_accept) {
                    current_state = STATE_ADJUST_TIME_HOURS;
                    last_activity_ticks_time = ticks;
                    flanco_accept = false;
                }
            }
            if (!adjusting_time || (events & args->cancel)) {
                current_state = STATE_SHOW_TIME;
                adjusting_time = false;
            }
            break;

        case STATE_ADJUST_TIME_HOURS:
            if (adjusting_time) {
                ScreenWriteBCD(args->board->screen, editable_time.bcd, 4);
                DisplayFlashDigits(args->board->screen, 0, 1, 100);

                if (!(events & args->increment)) {
                    flanco_increment = true;
                }
                if ((events & args->increment) && flanco_increment) {
                    IncrementHours(&editable_time);
                    last_activity_ticks_time = ticks;
                    flanco_increment = false;
                }

                if (!(events & args->decrement)) {
                    flanco_decrement = true;
                }
                if ((events & args->decrement) && flanco_decrement) {
                    DecrementHours(&editable_time);
                    last_activity_ticks_time = ticks;
                    flanco_decrement = false;
                }

                if (!(events & args->accept)) {
                    flanco_accept = true;
                }
                if ((events & args->accept) && flanco_accept) {
                    editable_time.time.seconds[0] = 0; // Aseguramos que los segundos sean 00
                    editable_time.time.seconds[1] = 0;
                    ClockSetTime(args->clock, &editable_time);
                    current_state = STATE_SHOW_TIME;
                    adjusting_time = false;
                }
            }
            if (!adjusting_time || (events & args->cancel)) {
                current_state = STATE_SHOW_TIME;
                adjusting_time = false;
            }

            break;

            /*-------------------Seteo de
             * Alarma-----------------------------------------------*/
        case STATE_ADJUST_ALARM_MINUTES:
            if (adjusting_alarm) {

                DisplayFlashDot(args->board->screen, 0, 100, true);
                DisplayFlashDot(args->board->screen, 1, 100, true);
                DisplayFlashDot(args->board->screen, 2, 100, true);
                DisplayFlashDot(args->board->screen, 3, 100, true);
                DisplayFlashDigits(args->board->screen, 2, 3, 100);

                ScreenWriteBCD(args->board->screen, editable_alarm.bcd, 4);

                if (!(events & args->increment)) {
                    flanco_increment = true;
                }
                if ((events & args->increment) && flanco_increment) {
                    IncrementMinutes(&editable_alarm);
                    last_activity_ticks_alarm = ticks;
                    flanco_increment = false;
                }

                if (!(events & args->decrement)) {
                    flanco_decrement = true;
                }
                if ((events & args->decrement) && flanco_decrement) {
                    DecrementMinutes(&editable_alarm);
                    last_activity_ticks_alarm = ticks;
                    flanco_decrement = false;
                }

                if (!(events & args->accept)) {
                    flanco_accept = true;
                }
                if ((events & args->accept) && flanco_accept) {
                    current_state = STATE_ADJUST_ALARM_HOURS;
                    last_activity_ticks_alarm = ticks;
                    flanco_accept = false;
                }
            }
            if (!adjusting_alarm || (events & args->cancel)) {
                current_state = STATE_SHOW_TIME;
                adjusting_alarm = false;
            }
            break;

        case STATE_ADJUST_ALARM_HOURS:
            if (adjusting_alarm) {

                ScreenWriteBCD(args->board->screen, editable_alarm.bcd, 4);
                DisplayFlashDigits(args->board->screen, 0, 1, 100);

                if (!(events & args->increment)) {
                    flanco_increment = true;
                }
                if ((events & args->increment) && flanco_increment) {
                    IncrementHours(&editable_alarm);
                    last_activity_ticks_alarm = ticks;
                    flanco_increment = false;
                }

                if (!(events & args->decrement)) {
                    flanco_decrement = true;
                }
                if ((events & args->decrement) && flanco_decrement) {
                    DecrementHours(&editable_alarm);
                    last_activity_ticks_alarm = ticks;
                    flanco_decrement = false;
                }

                if (!(events & args->accept)) {
                    flanco_accept = true;
                }
                if ((events & args->accept) && flanco_accept) {
                    editable_alarm.time.seconds[0] = 0; // Aseguramos que los segundos sean 00
                    editable_alarm.time.seconds[1] = 0;
                    ClockSetAlarm(args->clock, &editable_alarm);
                    current_state = STATE_SHOW_TIME;
                    adjusting_alarm = false;
                }
            }
            if (!adjusting_alarm || (events & args->cancel)) {
                current_state = STATE_SHOW_TIME;
                adjusting_alarm = false;
            }

            break;

            /*-------------------Contol de
             * Alarma-----------------------------------------------*/
        case STATE_CONTROL_ALARM:
            if (ClockAlarmIsRinging(args->clock) && alarm_is_active) {
                DigitalOutputActivate(args->board->led_R);
            } else if (!ClockAlarmIsRinging(args->clock) || !alarm_is_active) {
                DigitalOutputDeactivate(args->board->led_R);
            }

            if (!(events & args->accept)) {
                flanco_accept = true;
            }
            if ((events & args->accept) && flanco_accept) {
                ClockPostponeAlarmRandomMinutes(args->clock, 5);
                flanco_accept = false;
            }

            if (!(events & args->cancel)) {
                flanco_cancel = true;
            }
            if ((events & args->cancel) && flanco_cancel) {
                ClockPostponeAlarmOneDay(args->clock);
                DigitalOutputDeactivate(args->board->led_R);
                flanco_cancel = false;
            }

            current_state = STATE_SHOW_TIME;
            break;
        }
    }
}

/* === End of documentation ======================================================================================== */
