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

#ifndef SCREEN_H_
#define SCREEN_H_

/** @file screen.h
 ** @brief Declaración de funciones y macros para el control de una pantalla multiplexada de 7 segmentos
 **/

/* === Headers files inclusions ==================================================================================== */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

#define SEGMENT_A (1 << 0) // Segmento A
#define SEGMENT_B (1 << 1) // Segmento B
#define SEGMENT_C (1 << 2) // Segmento C
#define SEGMENT_D (1 << 3) // Segmento D
#define SEGMENT_E (1 << 4) // Segmento E
#define SEGMENT_F (1 << 5) // Segmento F
#define SEGMENT_G (1 << 6) // Segmento G
#define SEGMENT_P (1 << 7) // Punto decimal

/* === Public data type declarations =============================================================================== */

typedef struct screen_s * screen_t;

typedef void (*digits_turn_off_t)(void);

typedef void (*segments_update_t)(uint8_t);

typedef void (*digit_turn_on_t)(uint8_t);

typedef struct screen_driver_s {
    digits_turn_off_t DigitsTurnOff;  // Función para apagar todos los dígitos
    segments_update_t SegmentsUpdate; // Función para actualizar los segmentos del dígito actual
    digit_turn_on_t DigitTurnOn;      // Función para encender un dígito específico
} const * screen_driver_t;

/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */

/**
 * @brief Función para crear una nueva instancia de pantalla.
 * @param digits Número de dígitos que tendrá la pantalla.
 * @param driver Estructura con funciones de control de pantalla.
 * @return Puntero a la nueva instancia de pantalla.
 * @note Si el número de dígitos es mayor que SCREEN_MAX_DIGITS, se limitará al máximo permitido.
 */
screen_t ScreenCreate(uint8_t digits, screen_driver_t driver);

/**
 * @brief Función para escribir un valor BCD en la pantalla.
 * @param screen Puntero al descriptor de la pantalla con la que se quiere operar.
 * @param value Array de valores BCD a escribir en la pantalla.
 * @param size Tamaño del array de valores BCD.
 * @return void
 */
void ScreenWriteBCD(screen_t screen, uint8_t value[], uint8_t size);

/**
 * @brief Función para refrescar la pantalla.
 * @param screen Puntero al descriptor de la pantalla con la que se quiere operar.
 * @return void
 */
void ScreenRefresh(screen_t screen);

/**
 * @brief Función para hacer parpadear algunos dígitos de la pantalla.
 * @param display Puntero al descriptor de la pantalla con la que se quiere operar.
 * @param from  Posición del primer dígito que se quiere hacer parpadear.
 * @param to Posición del último dígito que se quiere hacer parpadear.
 * @param frecuency Factor de división de la frecuencia de refresco para el parpadeo.
 * @return void
 * @note Esta función hace parpadear los dígitos de la pantalla en el rango especificado
 */
int DisplayFlashDigits(screen_t display, uint8_t from, uint8_t to, uint16_t divisor);

/**
 * @brief Función para hacer parpadear los puntos decimales de la pantalla.
 * @param display Puntero al descriptor de la pantalla con la que se quiere operar.
 * @param dot Posición del punto decimal que se quiere hacer parpadear.
 * @param divisor Factor de división de la frecuencia de refresco para el parpadeo.
 * @return void
 * @note Esta función hace parpadear el punto decimal en la posición especificada
 */
int DisplayFlashDots(screen_t self, uint8_t from, uint8_t to, uint16_t divisor);

void DisplayWriteDots(screen_t self, uint8_t dot, bool turn);

/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* SCREEN_H_ */
