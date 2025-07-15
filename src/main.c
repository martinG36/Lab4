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

#include "bsp.h"
#include "chip.h"
#include "clock.h"
#include <stdbool.h>

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

typedef enum {
    STATE_SHOW_TIME,
    STATE_ADJUST_TIME_MINUTES,
    STATE_ADJUST_TIME_HOURS,
    STATE_ADJUST_ALARM_MINUTES,
    STATE_ADJUST_ALARM_HOURS,
    STATE_CONTROL_ALARM
} clock_state_t;

static clock_state_t current_state = STATE_SHOW_TIME;

static const struct board_s * board = NULL;
static struct clock_s * clock = NULL;

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

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/* === Public function implementation ========================================================= */

void SysTick_Handler(void) {
    set_time_pressed_ticks++;
    ticks++;

    if (board) {
        ScreenRefresh(board->screen);
    }

    if (clock) {
        ClockNewTick(clock);
    }

    // Detección de pulsación larga del botón 'set_time'
    if (!adjusting_time) {
        if (DigitalInputGetIsActive(board->set_time)) {
            set_time_pressed_ticks++;
            if (set_time_pressed_ticks >= 30000) {
                adjusting_time = true;
                set_time_pressed_ticks = 0;

                // Hacemos una copia de la hora actual como backup y editable
                ClockGetTime(clock, &time_backup);
                editable_time = time_backup;
                last_activity_ticks_time = ticks;
            }
        } else {
            set_time_pressed_ticks = 0;
        }
    } else {
        // En modo ajuste, verificar inactividad de 30 segundos
        if ((ticks - last_activity_ticks_time) >= 300000) {
            // Salir del modo ajuste y descartar cambios
            adjusting_time = false;
        }
    }

    // Detección de pulsación larga del botón 'set_alarm'
    if (!adjusting_alarm) {
        if (DigitalInputGetIsActive(board->set_alarm)) {
            set_alarm_pressed_ticks++;
            if (set_alarm_pressed_ticks >= 30000) {
                adjusting_alarm = true;
                set_alarm_pressed_ticks = 0;

                last_activity_ticks_alarm = ticks;
            }
        } else {
            set_alarm_pressed_ticks = 0;
        }
    } else {
        // En modo ajuste, verificar inactividad de 30 segundos
        if ((ticks - last_activity_ticks_alarm) >= 300000) {
            // Salir del modo ajuste y descartar cambios
            adjusting_alarm = false;
        }
    }
}

void ConfigSysTick(void) {
    // Configura el SysTick para generar una interrupción cada 1 ms
    /* Activate SysTick */
    SystemCoreClockUpdate();
    SysTick_Config((SystemCoreClock / 10000) - 1);

    /* Update priority set by SysTick_Config */
    NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
}

int main(void) {

    ConfigSysTick();

    board = BoardCreate();
    clock = ClockCreate();

    clock_time_t hora;

    bool valid_time;
    bool flanco_increment = true;
    bool flanco_decrement = true;
    bool flanco_accept = true;
    bool flanco_cancel = true;
    bool alarm_is_active = false;

    DigitalOutputDeactivate(board->led_R);

    while (true) {

        switch (current_state) {
            /*-------------------Funcionamiento Normal-------------------------------------------*/
        case STATE_SHOW_TIME:
            valid_time = ClockGetTime(clock, &hora);
            ScreenWriteBCD(board->screen, hora.bcd, 4);

            if (valid_time) {
                DisplayFlashDigits(board->screen, 0, 3, 0);
                DisplayFlashDot(board->screen, 0, 1000, false);
                DisplayFlashDot(board->screen, 1, 1000, true);
                DisplayFlashDot(board->screen, 2, 1000, false);
                if (alarm_is_active) {
                    DisplayFlashDot(board->screen, 3, 0, true);
                    ClockSetStateAlarm(clock, true);
                    current_state = STATE_CONTROL_ALARM;
                } else {
                    DisplayFlashDot(board->screen, 3, 1000, false);
                    ClockSetStateAlarm(clock, false);
                }
            } else {
                DisplayFlashDigits(board->screen, 0, 3, 1000);
                DisplayFlashDot(board->screen, 1, 1000, true);
            }

            if (adjusting_time) {
                current_state = STATE_ADJUST_TIME_MINUTES;
            }
            if (adjusting_alarm) {
                current_state = STATE_ADJUST_ALARM_MINUTES;
            }

            if (DigitalInputGetIsActive(board->accept) == 0) {
                flanco_accept = true;
            }
            if ((DigitalInputGetIsActive(board->accept) == 1 && flanco_accept) && !ClockAlarmIsRinging(clock)) {
                alarm_is_active = true;
                flanco_accept = false;
            }
            if (DigitalInputGetIsActive(board->cancel) == 0) {
                flanco_cancel = true;
            }
            if ((DigitalInputGetIsActive(board->cancel) == 1 && flanco_cancel) && !ClockAlarmIsRinging(clock)) {
                alarm_is_active = false;
                flanco_cancel = false;
            }

            break;

            /*-------------------Puesta en Hora------------------------------------------------*/
        case STATE_ADJUST_TIME_MINUTES:
            if (adjusting_time) {
                ScreenWriteBCD(board->screen, editable_time.bcd, 4);
                DisplayFlashDigits(board->screen, 2, 3, 1000);

                if (DigitalInputGetIsActive(board->increment) == 0) {
                    flanco_increment = true;
                }
                if (DigitalInputGetIsActive(board->increment) == 1 && flanco_increment) {
                    IncrementMinutes(&editable_time);
                    last_activity_ticks_time = ticks;
                    flanco_increment = false;
                }

                if (DigitalInputGetIsActive(board->decrement) == 0) {
                    flanco_decrement = true;
                }
                if (DigitalInputGetIsActive(board->decrement) == 1 && flanco_decrement) {
                    DecrementMinutes(&editable_time);
                    last_activity_ticks_time = ticks;
                    flanco_decrement = false;
                }

                if (DigitalInputGetIsActive(board->accept) == 0) {
                    flanco_accept = true;
                }
                if (DigitalInputGetIsActive(board->accept) == 1 && flanco_accept) {
                    current_state = STATE_ADJUST_TIME_HOURS;
                    last_activity_ticks_time = ticks;
                    flanco_accept = false;
                }
            }
            if (!adjusting_time || DigitalInputGetIsActive(board->cancel) != 0) {
                current_state = STATE_SHOW_TIME;
                adjusting_time = false;
            }
            break;

        case STATE_ADJUST_TIME_HOURS:
            if (adjusting_time) {
                ScreenWriteBCD(board->screen, editable_time.bcd, 4);
                DisplayFlashDigits(board->screen, 0, 1, 1000);

                if (DigitalInputGetIsActive(board->increment) == 0) {
                    flanco_increment = true;
                }
                if (DigitalInputGetIsActive(board->increment) == 1 && flanco_increment) {
                    IncrementHours(&editable_time);
                    last_activity_ticks_time = ticks;
                    flanco_increment = false;
                }

                if (DigitalInputGetIsActive(board->decrement) == 0) {
                    flanco_decrement = true;
                }
                if (DigitalInputGetIsActive(board->decrement) == 1 && flanco_decrement) {
                    DecrementHours(&editable_time);
                    last_activity_ticks_time = ticks;
                    flanco_decrement = false;
                }

                if (DigitalInputGetIsActive(board->accept) == 0) {
                    flanco_accept = true;
                }
                if (DigitalInputGetIsActive(board->accept) == 1 && flanco_accept) {
                    editable_time.time.seconds[0] = 0; // Aseguramos que los segundos sean 00
                    editable_time.time.seconds[1] = 0;
                    ClockSetTime(clock, &editable_time);
                    current_state = STATE_SHOW_TIME;
                    adjusting_time = false;
                }
            }
            if (!adjusting_time || DigitalInputGetIsActive(board->cancel) != 0) {
                current_state = STATE_SHOW_TIME;
                adjusting_time = false;
            }

            break;

            /*-------------------Seteo de Alarma-----------------------------------------------*/
        case STATE_ADJUST_ALARM_MINUTES:
            if (adjusting_alarm) {

                DisplayFlashDot(board->screen, 0, 1000, true);
                DisplayFlashDot(board->screen, 1, 1000, true);
                DisplayFlashDot(board->screen, 2, 1000, true);
                DisplayFlashDot(board->screen, 3, 1000, true);
                DisplayFlashDigits(board->screen, 2, 3, 1000);

                ScreenWriteBCD(board->screen, editable_alarm.bcd, 4);

                if (DigitalInputGetIsActive(board->increment) == 0) {
                    flanco_increment = true;
                }
                if (DigitalInputGetIsActive(board->increment) == 1 && flanco_increment) {
                    IncrementMinutes(&editable_alarm);
                    last_activity_ticks_alarm = ticks;
                    flanco_increment = false;
                }

                if (DigitalInputGetIsActive(board->decrement) == 0) {
                    flanco_decrement = true;
                }
                if (DigitalInputGetIsActive(board->decrement) == 1 && flanco_decrement) {
                    DecrementMinutes(&editable_alarm);
                    last_activity_ticks_alarm = ticks;
                    flanco_decrement = false;
                }

                if (DigitalInputGetIsActive(board->accept) == 0) {
                    flanco_accept = true;
                }
                if (DigitalInputGetIsActive(board->accept) == 1 && flanco_accept) {
                    current_state = STATE_ADJUST_ALARM_HOURS;
                    last_activity_ticks_alarm = ticks;
                    flanco_accept = false;
                }
            }
            if (!adjusting_alarm || DigitalInputGetIsActive(board->cancel) != 0) {
                current_state = STATE_SHOW_TIME;
                adjusting_alarm = false;
            }
            break;

        case STATE_ADJUST_ALARM_HOURS:
            if (adjusting_alarm) {

                ScreenWriteBCD(board->screen, editable_alarm.bcd, 4);
                DisplayFlashDigits(board->screen, 0, 1, 1000);

                if (DigitalInputGetIsActive(board->increment) == 0) {
                    flanco_increment = true;
                }
                if (DigitalInputGetIsActive(board->increment) == 1 && flanco_increment) {
                    IncrementHours(&editable_alarm);
                    last_activity_ticks_alarm = ticks;
                    flanco_increment = false;
                }

                if (DigitalInputGetIsActive(board->decrement) == 0) {
                    flanco_decrement = true;
                }
                if (DigitalInputGetIsActive(board->decrement) == 1 && flanco_decrement) {
                    DecrementHours(&editable_alarm);
                    last_activity_ticks_alarm = ticks;
                    flanco_decrement = false;
                }

                if (DigitalInputGetIsActive(board->accept) == 0) {
                    flanco_accept = true;
                }
                if (DigitalInputGetIsActive(board->accept) == 1 && flanco_accept) {
                    editable_alarm.time.seconds[0] = 0; // Aseguramos que los segundos sean 00
                    editable_alarm.time.seconds[1] = 0;
                    ClockSetAlarm(clock, &editable_alarm);
                    current_state = STATE_SHOW_TIME;
                    adjusting_alarm = false;
                }
            }
            if (!adjusting_alarm || DigitalInputGetIsActive(board->cancel) != 0) {
                current_state = STATE_SHOW_TIME;
                adjusting_alarm = false;
            }

            break;

            /*-------------------Contol de Alarma-----------------------------------------------*/
        case STATE_CONTROL_ALARM:
            if (ClockAlarmIsRinging(clock) && alarm_is_active) {
                DigitalOutputActivate(board->led_R);
            } else if (!ClockAlarmIsRinging(clock) || !alarm_is_active) {
                DigitalOutputDeactivate(board->led_R);
            }

            if (DigitalInputGetIsActive(board->accept) == 0) {
                flanco_accept = true;
            }
            if (DigitalInputGetIsActive(board->accept) == 1 && flanco_accept) {
                ClockPostponeAlarmRandomMinutes(clock, 5);
                flanco_accept = false;
            }
            // if (DigitalInputGetIsActive(board->cancel) == 0) {
            //     flanco_cancel = true;
            // }
            // if (DigitalInputGetIsActive(board->cancel) == 1 && flanco_cancel) {
            //     ClockResetAlarm(clock);
            //     DigitalOutputDeactivate(board->led_R);
            //     flanco_cancel = false;
            // }

            current_state = STATE_SHOW_TIME;
            break;
        }
    }
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
