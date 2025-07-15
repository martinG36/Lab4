/*********************************************************************************************************************
Copyright (c) 2025, Martín Gareca <mfgareca@gmail.com>

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

#ifndef CLOCK_H_
#define CLOCK_H_

/** @file clock.h
 ** @brief  Declaración de funciones y macros para las pruebas del reloj
 **/

/* === Headers files inclusions ==================================================================================== */

/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

#include <stdint.h>
#include <stdbool.h>

/* === Public data type declarations =============================================================================== */

typedef union {
    struct {
        uint8_t seconds[2];
        uint8_t minutes[2];
        uint8_t hours[2];
    } time;
    uint8_t bcd[6];
} clock_time_t;

typedef struct clock_s * clock_t;

/**
 * @brief Función para crear un reloj.
 * Inicializa el reloj con una hora inválida (00:00) y un indicador de validez en falso.
 * @return Un puntero al reloj creado.
 */
clock_t ClockCreate(void);

/**
 * @brief Función para obtener la hora actual del reloj.
 * Copia en result la hora almacenada en self->current_time.
 * @param self Puntero al reloj.
 * @param result Puntero donde se almacenará la hora actual.
 * @return Verdadero si se obtuvo la hora correctamente, falso si el reloj no es válido.
 */
bool ClockGetTime(clock_t clock, clock_time_t * result);

/**
 * @brief Función para establecer una nueva hora en el reloj.
 * Actualiza self->current_time con la nueva hora proporcionada en new_time y marca el reloj como válido.
 * @param self Puntero al reloj.
 * @param new_time Puntero a la nueva hora que se desea establecer.
 * @return Verdadero si se estableció la hora correctamente, falso si el reloj no es válido.
 */
bool ClockSetTime(clock_t clock, const clock_time_t * new_time);

/**
 * @brief Función para simular un nuevo tick del reloj.
 * Incrementa el contador de ticks por segundo y actualiza la hora actual en consecuencia.
 * Si se alcanza el límite de ticks por segundo, se incrementa la hora, minutos y segundos según corresponda.
 * @param self Puntero al reloj.
 */
void ClockNewTick(clock_t clock);

/**
 * @brief Función para establecer una alarma en el reloj.
 * Copia la hora de la alarma proporcionada en alarm_time a self->alarm_time y marca la alarma como válida.
 * @param self Puntero al reloj.
 * @param alarm_time Puntero a la hora de la alarma que se desea establecer.
 * @return Verdadero si se estableció la alarma correctamente, falso si el reloj no es válido.
 */
bool ClockSetAlarm(clock_t self, const clock_time_t * alarm_time);

/**
 * @brief Función para obtener la hora de la alarma del reloj.
 * Copia en alarm_time la hora de la alarma almacenada en self->alarm_time.
 * @param self Puntero al reloj.
 * @param alarm_time Puntero donde se almacenará la hora de la alarma.
 * @return Verdadero si se obtuvo la hora de la alarma correctamente, falso si el reloj no es válido.
 */
bool ClockGetAlarm(clock_t self, clock_time_t * alarm_time);

/**
 * @brief Función para verificar si la alarma está sonando.
 * Compara la hora actual del reloj con la hora de la alarma y actualiza el estado de self->alarm_ringing.
 * @param self Puntero al reloj.
 * @return Verdadero si la alarma está sonando, falso en caso contrario.
 */
bool ClockAlarmIsRinging(clock_t self);

/**
 * @brief Función para habilitar o deshabilitar la alarma.
 * Actualiza el estado de self->alarm_enabled según el valor de enable.
 * @param self Puntero al reloj.
 * @param enable Verdadero para habilitar la alarma, falso para deshabilitarla.
 */
void ClockSetStateAlarm(clock_t self, bool enable);

/**
 * @brief Función para posponer la alarma.
 * Incrementa la hora de la alarma en 1 minuto y ajusta los valores de horas y minutos si es necesario.
 * Resetea el estado de self->alarm_ringing a falso.
 * @param self Puntero al reloj.
 */
void ClockPostponeAlarm(clock_t self);

/**
 * @brief Función para reiniciar la alarma.
 * Resetea el estado de self->alarm_ringing a falso y habilita la alarma.
 * Si la hora actual es 00:00, habilita la alarma automáticamente.
 * @param self Puntero al reloj.
 */
void ClockResetAlarm(clock_t self);

/**
 * @brief Función para reiniciar la alarma.
 * Resetea el estado de self->alarm_ringing a falso, habilita la alarma y verifica si la hora actual es 00:00.
 * Si es así, habilita la alarma automáticamente.
 * @param self Puntero al reloj.
 */
void ClockRestartAlarm(clock_t self);

/**
 * @brief Función para posponer la alarma en minutos aleatorios.
 * Incrementa la hora de la alarma en un número aleatorio de minutos entre 1 y 59.
 * Resetea el estado de self->alarm_ringing a falso.
 * @param self Puntero al reloj.
 * @param minutes Número de minutos a posponer (debe ser entre 1 y 59).
 * @return Verdadero si se pospuso la alarma correctamente, falso si el número de minutos es inválido.
 */
bool ClockPostponeAlarmRandomMinutes(clock_t self, uint8_t minutes);

/**
 * @brief Función para posponer la alarma un día.
 * Incrementa la hora de la alarma en 24 horas (1 día).
 * Resetea el estado de self->alarm_ringing a falso.
 * @param self Puntero al reloj.
 * @return Verdadero si se pospuso la alarma correctamente, falso si el reloj no es válido.
 */
void ClockPostponeAlarmOneDay(clock_t self);

/**
 * @brief Función para aumentar la cantidad de minutos.
 * Incrementa los minutos de la hora actual del reloj.
 * Si los minutos superan el límite, se ajustan los valores de horas y minutos.
 * @param clock Puntero al reloj.
 */
void IncrementMinutes(clock_time_t * clock);

/**
 * @brief Función para disminuir la cantidad de minutos.
 * Decrementa los minutos de la hora actual del reloj.
 * Si los minutos llegan a cero, se ajustan los valores de horas y minutos.
 * @param clock Puntero al reloj.
 */
void DecrementMinutes(clock_time_t * clock);

/**
 * @brief Función para incrementar las horas.
 * Incrementa las horas de la hora actual del reloj.
 * Si las horas superan el límite, se ajustan los valores de horas y minutos.
 * @param clock Puntero al reloj.
 */
void IncrementHours(clock_time_t * clock);

/**
 * @brief Función para decrementar las horas.
 * Decrementa las horas de la hora actual del reloj.
 * Si las horas llegan a cero, se ajustan los valores de horas y minutos.
 * @param clock Puntero al reloj.
 */
void DecrementHours(clock_time_t * clock);

/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */

/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* CLOCK_H_ */
