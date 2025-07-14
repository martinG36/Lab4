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

typedef enum { STATE_SHOW_TIME, STATE_ADJUST_MINUTES, STATE_ADJUST_HOURS } clock_state_t;

static clock_state_t current_state = STATE_SHOW_TIME;

static const struct board_s * board = NULL;
static struct clock_s * clock = NULL;

bool adjusting_time = false;
uint32_t set_time_pressed_ticks = 0;
uint32_t last_activity_ticks = 0;
clock_time_t time_backup;
clock_time_t editable_time;

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
                last_activity_ticks = ticks;
            }
        } else {
            set_time_pressed_ticks = 0;
        }
    } else {
        // En modo ajuste, verificar inactividad de 30 segundos
        if ((ticks - last_activity_ticks) >= 300000) {
            // Salir del modo ajuste y descartar cambios
            adjusting_time = false;
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

    while (true) {

        switch (current_state) {
        case STATE_SHOW_TIME:
            valid_time = ClockGetTime(clock, &hora);
            ScreenWriteBCD(board->screen, hora.bcd, 4);

            if (valid_time) {
                DisplayFlashDigits(board->screen, 0, 3, 0);
            } else {
                DisplayFlashDigits(board->screen, 0, 3, 1000);
                DisplayFlashDot(board->screen, 1, 1000, true);
            }

            if (adjusting_time) {
                current_state = STATE_ADJUST_MINUTES;
            }
            break;

        case STATE_ADJUST_MINUTES:
            if (adjusting_time) {
                ScreenWriteBCD(board->screen, editable_time.bcd, 4);
                DisplayFlashDigits(board->screen, 2, 3, 1000);

                if (DigitalInputGetIsActive(board->increment) == 0) {
                    flanco_increment = true;
                }
                if (DigitalInputGetIsActive(board->increment) == 1 && flanco_increment) {
                    IncrementMinutes(&editable_time);
                    last_activity_ticks = ticks;
                    flanco_increment = false;
                }

                if (DigitalInputGetIsActive(board->decrement) == 0) {
                    flanco_decrement = true;
                }
                if (DigitalInputGetIsActive(board->decrement) == 1 && flanco_decrement) {
                    DecrementMinutes(&editable_time);
                    last_activity_ticks = ticks;
                    flanco_decrement = false;
                }

                if (DigitalInputGetIsActive(board->accept) == 0) {
                    flanco_accept = true;
                }
                if (DigitalInputGetIsActive(board->accept) == 1 && flanco_accept) {
                    current_state = STATE_ADJUST_HOURS;
                    last_activity_ticks = ticks;
                    flanco_accept = false;
                }
            }
            if (!adjusting_time || DigitalInputGetIsActive(board->cancel) != 0) {
                current_state = STATE_SHOW_TIME;
                adjusting_time = false;
            }
            break;

        case STATE_ADJUST_HOURS:
            if (adjusting_time) {
                ScreenWriteBCD(board->screen, editable_time.bcd, 4);
                DisplayFlashDigits(board->screen, 0, 1, 1000);

                if (DigitalInputGetIsActive(board->increment) == 0) {
                    flanco_increment = true;
                }
                if (DigitalInputGetIsActive(board->increment) == 1 && flanco_increment) {
                    IncrementHours(&editable_time);
                    last_activity_ticks = ticks;
                    flanco_increment = false;
                }

                if (DigitalInputGetIsActive(board->decrement) == 0) {
                    flanco_decrement = true;
                }
                if (DigitalInputGetIsActive(board->decrement) == 1 && flanco_decrement) {
                    DecrementHours(&editable_time);
                    last_activity_ticks = ticks;
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
        }
    }
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
