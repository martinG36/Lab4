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

#ifndef DIGITAL_H_
#define DIGITAL_H_

/** @file digital.h
 ** @brief Declaración de funciones y macros para el control de pines digitales
 **/

/* === Headers files inclusions ==================================================================================== */

#include <stdint.h>
#include <stdbool.h>

/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

/* === Public data type declarations =============================================================================== */

typedef enum digital_state_e {
    DIGITAL_INPUT_WAS_DEACTIVATED = -1,
    DIGITAL_INPUT_NOT_CHANGED = 0,
    DIGITAL_INPUT_WAS_ACTIVATED = 1
} digital_state_t;

//! Estructura que representa una salida digital
typedef struct digital_output_s * digital_output_t;

//! Estructura que representa una entrada digital
typedef struct digital_input_s * digital_input_t;

/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */

/**
 * @brief Crea una salida digital
 *
 * @param gpio El puerto del pin
 * @param bit El número del pin
 * @return digital_output_t
 */
digital_output_t DigitalOutputCreate(uint8_t gpio, uint8_t bit, bool state);

/**
 * @brief Activa una salida digital
 * @param self La salida digital
 * @return void
 */
void DigitalOutputActivate(digital_output_t self);

/**
 * @brief Desactiva una salida digital
 * @param self La salida digital
 * @return void
 */
void DigitalOutputDeactivate(digital_output_t self);

/**
 * @brief Cambia el estado de la salida digital
 *
 * @param self La salida digital
 */
void DigitalOutputToggle(digital_output_t self);

/**
 * @brief Fución que crea una entrada digital
 *
 * @param gpio El puerto del pin
 * @param bit El número del pin
 * @param inverted Indica si la entrada digital está invertida
 * @return ditial_input_t
 */
digital_input_t DigitalInputCreate(uint8_t gpio, uint8_t bit, bool inverted);

/**
 * @brief Obtiene el estado de una entrada digital
 *
 * @param input La entrada digital
 * @return bool true si la entrada está activa, false si no lo está
 */
bool DigitalInputGetIsActive(digital_input_t input);

/**
 * @brief Verifica si el estado de la entrada digital ha cambiado
 *
 * @param input La entrada digital
 * @return digital_state_t Indica si la entrada fue activada, desactivada o no cambió
 */
digital_state_t DigitalWasChanged(digital_input_t input);

/**
 * @brief Verifica si la entrada digital fue activada
 *
 * @param input La entrada digital
 * @return bool true si la entrada fue activada, false en caso contrario
 */
bool DigitalWasActivated(digital_input_t input);

/**
 * @brief Verifica si la entrada digital fue desactivada
 *
 * @param input La entrada digital
 * @return bool true si la entrada fue desactivada, false en caso contrario
 */
bool DigitalWasDeactivated(digital_input_t input);

/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* DIGITAL_H_ */
