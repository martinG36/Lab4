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
#include <stdint.h>
#include <stdbool.h>

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/* === Public function implementation ========================================================= */

int main(void) {
    int state_RGB = 0;
    int divisor  = 0;
    int divisor_RGB = 0;
    board_t board = BoardCreate();

    while (true) {
        divisor_RGB++;
        if (DigitalInputGetIsActive(board->key_push) && divisor_RGB == 5 && state_RGB == 0) {
            state_RGB = 1;
            DigitalOutputDeactivate(board->led_B);
            DigitalOutputActivate(board->led_R);
        }else if (DigitalInputGetIsActive(board->key_push) && divisor_RGB == 10 && state_RGB == 1) {
            state_RGB = 2;
            DigitalOutputDeactivate(board->led_R);
            DigitalOutputActivate(board->led_G);
        }else if (DigitalInputGetIsActive(board->key_push) && divisor_RGB == 15 && state_RGB == 2) {
            state_RGB = 0;
            divisor_RGB = 0;
            DigitalOutputDeactivate(board->led_G);
            DigitalOutputActivate(board->led_B);
        }else if (!DigitalInputGetIsActive(board->key_push)) {
            DigitalOutputDeactivate(board->led_R);
            DigitalOutputDeactivate(board->led_G);
            DigitalOutputDeactivate(board->led_B);
            state_RGB = 0;
            divisor_RGB = 0;
        }

        if (DigitalInputGetIsActive(board->key_toggle)) {
            DigitalOutputToggle(board->led_red);
        } else {
            DigitalOutputDeactivate(board->led_red);
        }

        if (DigitalInputGetIsActive(board->key_turn_on)) {
            DigitalOutputActivate(board->led_yellow);
        }

        if (DigitalInputGetIsActive(board->key_turn_off)) {
            DigitalOutputDeactivate(board->led_yellow);
        }

        divisor++;
        if (divisor == 5) {
            divisor = 0;
            DigitalOutputToggle(board->led_green);
        }

        for (int index = 0; index < 50; index++) {
            for (int delay = 0; delay < 25000; delay++) {
                __asm("NOP");
            }
        }
    }
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
