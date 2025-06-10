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
    uint8_t digits;                    // Número de dígitos en la pantalla
    uint8_t current_digit;             // Dígito actual a mostrar
    uint8_t flashing_from;             // Dígito desde el cual comenzar a parpadear
    uint8_t flashing_to;               // Dígito hasta el cual parpadear
    uint8_t flashing_count;            // Contador para el parpadeo
    uint16_t flashing_frequency;       // Factor de división para el parpadeo
    screen_driver_t driver;            // Estructura con funciones de control de pantalla
    uint8_t value[SCREEN_MAX_DIGITS];  // Valores a mostrar
    uint8_t points[SCREEN_MAX_DIGITS]; // Puntos decimales para cada dígito
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
    // Crear una nueva instancia de pantalla
    screen_t self = malloc(sizeof(struct screen_s));
    if (digits > SCREEN_MAX_DIGITS) {
        digits = SCREEN_MAX_DIGITS; // Limitar a máximo de dígitos
    }
    if (self != NULL) {
        self->digits = digits;    // Inicializar número de dígitos
        self->driver = driver;    // Asignar controlador de pantalla
        self->current_digit = 0;  // Inicializar dígito actual
        self->flashing_count = 0; // Inicializar contador de parpadeo
        self->flashing_frequency = 0;
        memset(self->value, 0, sizeof(self->value));   // Limpiar valores previos
        memset(self->points, 0, sizeof(self->points)); // Limpiar puntos decimales
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
    uint8_t segments;

    self->driver->DigitsTurnOff();                                  // Apagar todos los dígitos
    self->current_digit = (self->current_digit + 1) % self->digits; // Avanzar al siguiente dígito

    segments = self->value[self->current_digit];
    if (self->flashing_frequency != 0) {
        if (self->current_digit == 0) {
            self->flashing_count = (self->flashing_count + 1) % (self->flashing_frequency);
        }
        if (self->flashing_count < (self->flashing_frequency / 2)) {
            if (self->current_digit >= self->flashing_from) {
                if (self->current_digit <= self->flashing_to) {
                    segments = 0; // Apagar segmentos
                }
            }
        }
    }

    self->driver->SegmentsUpdate(segments);         // Actualizar segmentos del dígito actual
    self->driver->DigitTurnOn(self->current_digit); // Encender el dígito actual
}

int DisplayFlashDigits(screen_t self, uint8_t from, uint8_t to, uint16_t divisor) {
    int result = 0;
    if ((from > to) || (from >= SCREEN_MAX_DIGITS) || (to >= SCREEN_MAX_DIGITS)) {
        result = -1;
    } else if (!self) {
        result = -1;
    } else {
        self->flashing_from = from;
        self->flashing_to = to;
        self->flashing_frequency = 2 * divisor;
        self->flashing_count = 0; // Reiniciar contador de parpadeo
    }

    return result;
}
/* === End of documentation ======================================================================================== */
