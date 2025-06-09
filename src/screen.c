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

/** @file screen.c
 ** @brief Implementación de funciones y macros para el control de una pantalla multiplexada de 7 segmentos
 **/

/* === Headers files inclusions ==================================================================================== */

#include "screen.h"

/* === Macros definitions ========================================================================================== */

#ifndef SCREEN_MAX_DIGITS
#define SCREEN_MAX_DIGITS 8 // Número máximo de dígitos soportados por la pantalla
#endif

/* === Private data type declarations ============================================================================== */

struct screen_s {
    uint8_t digits;                   // Número de dígitos en la pantalla
    uint8_t currente_digit;           // Dígito actual a mostrar
    screen_driver_t driver;           // Estructura con funciones de control de pantalla
    uint8_t value[SCREEN_MAX_DIGITS]; // Valores a mostrar
};

/* === Private function declarations =============================================================================== */

static const uint8_t IMAGES[10] = {
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F,             // 0
    SEGMENT_B | SEGMENT_C,                                                             // 1
    SEGMENT_A | SEGMENT_B | SEGMENT_D | SEGMENT_E | SEGMENT_G,                         // 2
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_G,                         // 3
    SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G,                                     // 4
    SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G,                         // 5
    SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G,             // 6
    SEGMENT_A | SEGMENT_B | SEGMENT_C,                                                 // 7
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G, // 8
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G              // 9
};

/* === Private variable definitions ================================================================================ */

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */

/* === Public function definitions ================================================================================= */

screen_t ScreenCreate(uint8_t digits, screen_driver_t driver) {
    // Crear una nueva instancia de pantalla}
    screen_t self = malloc(sizeof(struct screen_s));
    if (digits > SCREEN_MAX_DIGITS) {
        digits = SCREEN_MAX_DIGITS; // Limitar a máximo de dígitos
    }
    if (self != NULL) {
        self->digits = digits;    // Inicializar número de dígitos
        self->driver = driver;    // Asignar controlador de pantalla
        self->currente_digit = 0; // Inicializar dígito actual
    }
    return self;
}

void ScreenWriteBCD(screen_t self, uint8_t value[], uint8_t size) {
    memset(self->value, 0, sizeof(self->value)); // Limpiar valores previos
    if (size > self->digits) {
        size = self->digits; // Limitar al número de dígitos de la pantalla
    }

    for (uint8_t i = 0; i < size; i++) {
        self->value[i] = IMAGES[value[i]]; // Copiar valores a la pantalla
    }
}

void ScreenRefresh(screen_t self) {
    self->driver->DigitsTurnOff();                                    // Apagar todos los dígitos
    self->currente_digit = (self->currente_digit + 1) % self->digits; // Avanzar al siguiente dígito
    self->driver->SegmentsUpdate(self->value[self->currente_digit]);  // Actualizar segmentos del dígito actual
    self->driver->DigitTurnOn(self->currente_digit);                  // Encender el dígito actual
}

/* === End of documentation ======================================================================================== */
