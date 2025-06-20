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
    int divisor = 0;
    int cont_R = 0;
    int cont_G = 0;
    int cont_B = 0;
    uint8_t value[4] = {1, 2, 3, 4};

    board_t board = BoardCreate();

    ScreenWriteBCD(board->screen, value, 4);
    DisplayFlashDigits(board->screen, 1, 2, 50);
    DisplayFlashDots(board->screen, 0, 0, 100);

    while (true) {
        divisor++;

        cont_R++;
        cont_G++;
        cont_B++;

        if (DigitalInputGetIsActive(board->set_alarm) != 0) {
            DigitalOutputActivate(board->led_R);
        }

        if ((DigitalInputGetIsActive(board->set_time) != 0) || (cont_R == 500)) {
            DigitalOutputDeactivate(board->led_R);
        }

        if (DigitalInputGetIsActive(board->increment) != 0) {
            DigitalOutputActivate(board->led_G);
        }

        if ((DigitalInputGetIsActive(board->decrement) != 0) || (cont_G == 800)) {
            DigitalOutputDeactivate(board->led_G);
        }

        if (DigitalInputGetIsActive(board->cancel) != 0) {
            DigitalOutputActivate(board->led_B);
        }

        if ((DigitalInputGetIsActive(board->accept) != 0) || (cont_B == 1200)) {
            DigitalOutputDeactivate(board->led_B);
        }

        if (divisor == 5) {
            divisor = 0;
        }

        if (cont_R == 500) {
            cont_R = 0;
        }
        if (cont_G == 800) {
            cont_G = 0;
        }
        if (cont_B == 1200) {
            cont_B = 0;
        }

        ScreenRefresh(board->screen);
        for (int delay = 0; delay < 25000; delay++) {
            __asm("NOP");
        }
    }
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
