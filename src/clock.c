/*********************************************************************************************************************
Copyright (c) 2025, Martín Gareca <mfgareca36@gmail.com>

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

/** @file clock.c
 ** @brief Implementación de funciones del reloj
 **/

/* === Headers files inclusions ==================================================================================== */

#include "clock.h"
#include <stddef.h>
#include <string.h>

/* === Macros definitions ========================================================================================== */

/* === Private data type declarations ============================================================================== */

/* === Private function declarations =============================================================================== */

/* === Private variable definitions ================================================================================ */

/* === Public variable definitions ================================================================================= */

/**
 * @brief Estructura que representa el reloj.
 * Esta estructura contiene la hora actual y un indicador de validez.
 */
struct clock_s {
    uint16_t ticks_per_second; /**< Número de ticks por segundo del reloj */
    clock_time_t current_time; /**< Hora actual del reloj */
    bool valid;                /**< Indicador de validez del reloj */

    clock_time_t alarm_time; /**< Hora de la alarma */
    bool alarm_valid;        /**< Indicador de validez de la alarma */
    bool alarm_ringing;      /**< Indicador de si la alarma está sonando */
    bool alarm_enabled;      /**< Indicador de si la alarma está habilitada */
};

/* === Private function definitions ================================================================================ */

/* === Public function implementation ============================================================================== */

/**
 * @brief Función para crear un reloj.
 * Inicializa el reloj con una hora inválida (00:00) y un indicador de validez en falso.
 * @return Un puntero al reloj creado.
 */
clock_t ClockCreate(uint16_t ticks_per_second) {
    (void)ticks_per_second;
    static struct clock_s self[1];
    memset(self, 0, sizeof(struct clock_s));
    self->valid = false;
    return self;
}

/**
 * @brief Función para obtener la hora actual del reloj.
 * Copia en result la hora almacenada en self->current_time.
 * @param self Puntero al reloj.
 * @param result Puntero donde se almacenará la hora actual.
 * @return Verdadero si se obtuvo la hora correctamente, falso si el reloj no es válido.
 */
bool ClockGetTime(clock_t self, clock_time_t * result) {
    memcpy(result, &self->current_time, sizeof(clock_time_t));
    return self->valid;
}

/**
 * @brief Función para establecer una nueva hora en el reloj.
 * Actualiza self->current_time con la nueva hora proporcionada en new_time y marca el reloj como válido.
 * @param self Puntero al reloj.
 * @param new_time Puntero a la nueva hora que se desea establecer.
 * @return Verdadero si se estableció la hora correctamente, falso si el reloj no es válido.
 */
bool ClockSetTime(clock_t self, const clock_time_t * new_time) {
    if (new_time == NULL || new_time->time.hours[0] > 9 || new_time->time.hours[1] > 2 ||
        (new_time->time.hours[1] == 2 && new_time->time.hours[0] > 3) || new_time->time.minutes[0] > 9 ||
        new_time->time.minutes[1] > 5 || new_time->time.seconds[0] > 9 || new_time->time.seconds[1] > 5) {
        self->valid = false;
    } else {
        self->valid = true;
        memcpy(&self->current_time, new_time, sizeof(clock_time_t));
    }

    return self->valid;
}

/**
 * @brief Función para simular un nuevo tick del reloj.
 * Incrementa el contador de ticks por segundo y actualiza la hora actual en consecuencia.
 * Si se alcanza el límite de ticks por segundo, se incrementa la hora, minutos y segundos según corresponda.
 * @param self Puntero al reloj.
 */
void ClockNewTick(clock_t self) {
    self->ticks_per_second++;
    if (self->ticks_per_second == 1000) {
        self->ticks_per_second = 0;
        self->current_time.time.seconds[0]++;
        if (self->current_time.time.seconds[0] > 9) {
            self->current_time.time.seconds[0] = 0;
            self->current_time.time.seconds[1]++;
            if (self->current_time.time.seconds[1] > 5) {
                self->current_time.time.seconds[1] = 0;
                self->current_time.time.minutes[0]++;
                if (self->current_time.time.minutes[0] > 9) {
                    self->current_time.time.minutes[0] = 0;
                    self->current_time.time.minutes[1]++;
                    if (self->current_time.time.minutes[1] > 5) {
                        self->current_time.time.minutes[1] = 0;
                        self->current_time.time.hours[0]++;
                        if (self->current_time.time.hours[0] > 9) {
                            self->current_time.time.hours[0] = 0;
                            self->current_time.time.hours[1]++;
                        }
                        if (self->current_time.time.hours[1] > 2 ||
                            (self->current_time.time.hours[1] == 2 && self->current_time.time.hours[0] > 3)) {
                            self->current_time.time.hours[1] = 0;
                            self->current_time.time.hours[0] = 0;
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief Función para establecer una alarma en el reloj.
 * Copia la hora de la alarma proporcionada en alarm_time a self->alarm_time y marca la alarma como válida.
 * @param self Puntero al reloj.
 * @param alarm_time Puntero a la hora de la alarma que se desea establecer.
 * @return Verdadero si se estableció la alarma correctamente, falso si el reloj no es válido.
 */
bool ClockSetAlarm(clock_t self, const clock_time_t * alarm_time) {
    self->valid = true;
    memcpy(&self->alarm_time, alarm_time, sizeof(clock_time_t));
    return self->valid;
}

/**
 * @brief Función para obtener la hora de la alarma del reloj.
 * Copia en alarm_time la hora de la alarma almacenada en self->alarm_time.
 * @param self Puntero al reloj.
 * @param alarm_time Puntero donde se almacenará la hora de la alarma.
 * @return Verdadero si se obtuvo la hora de la alarma correctamente, falso si el reloj no es válido.
 */
bool ClockGetAlarm(clock_t self, clock_time_t * alarm_time) {
    memcpy(alarm_time, &self->alarm_time, sizeof(clock_time_t));
    return self->valid;
}

/**
 * @brief Función para verificar si la alarma está sonando.
 * Compara la hora actual del reloj con la hora de la alarma y actualiza el estado de self->alarm_ringing.
 * @param self Puntero al reloj.
 * @return Verdadero si la alarma está sonando, falso en caso contrario.
 */
bool ClockAlarmIsRinging(clock_t self) {
    if (self->current_time.time.hours[0] == self->alarm_time.time.hours[0] &&
        self->current_time.time.hours[1] == self->alarm_time.time.hours[1] &&
        self->current_time.time.minutes[0] == self->alarm_time.time.minutes[0] &&
        self->current_time.time.minutes[1] == self->alarm_time.time.minutes[1] &&
        self->current_time.time.seconds[0] == self->alarm_time.time.seconds[0] &&
        self->current_time.time.seconds[1] == self->alarm_time.time.seconds[1] && self->alarm_enabled) {
        self->alarm_ringing = true;
    } else {
        self->alarm_ringing = false;
    }

    return self->alarm_ringing;
}

/**
 * @brief Función para habilitar o deshabilitar la alarma.
 * Actualiza el estado de self->alarm_enabled según el valor de enable.
 * @param self Puntero al reloj.
 * @param enable Verdadero para habilitar la alarma, falso para deshabilitarla.
 */
void ClockSetStateAlarm(clock_t self, bool enable) {
    if (enable) {
        self->alarm_enabled = true;
    } else {
        self->alarm_enabled = false;
    }
}

/**
 * @brief Función para posponer la alarma.
 * Incrementa la hora de la alarma en 1 minuto y ajusta los valores de horas y minutos si es necesario.
 * Resetea el estado de self->alarm_ringing a falso.
 * @param self Puntero al reloj.
 */
void ClockPostponeAlarm(clock_t self) {
    self->alarm_ringing = false;

    self->alarm_time.time.minutes[1]++;
    if (self->alarm_time.time.minutes[1] > 5) {
        self->alarm_time.time.minutes[1] = 0;
        self->alarm_time.time.hours[0]++;
        if (self->alarm_time.time.hours[0] > 9) {
            self->alarm_time.time.hours[0] = 0;
            self->alarm_time.time.hours[1]++;
        }
        if (self->alarm_time.time.hours[1] > 2 ||
            (self->alarm_time.time.hours[1] == 2 && self->alarm_time.time.hours[0] > 3)) {
            self->alarm_time.time.hours[1] = 0;
            self->alarm_time.time.hours[0] = 0;
        }
    }
}

/**
 * @brief Función para reiniciar la alarma.
 * Resetea el estado de self->alarm_ringing a falso y habilita la alarma.
 * Si la hora actual es 00:00, habilita la alarma automáticamente.
 * @param self Puntero al reloj.
 */
void ClockResetAlarm(clock_t clock) {
    clock->alarm_valid = false;
    clock->alarm_ringing = false;
    clock->alarm_enabled = false;
    memset(&clock->alarm_time, 0, sizeof(clock_time_t));
}

/**
 * @brief Función para reiniciar la alarma.
 * Resetea el estado de self->alarm_ringing a falso, habilita la alarma y verifica si la hora actual es 00:00.
 * Si es así, habilita la alarma automáticamente.
 * @param self Puntero al reloj.
 */
void ClockRestartAlarm(clock_t self) {
    self->alarm_ringing = false;
    self->alarm_enabled = true;

    if (self->current_time.bcd[0] == 0 && self->current_time.bcd[1] == 0 && self->current_time.bcd[2] == 0 &&
        self->current_time.bcd[3] == 0 && self->current_time.bcd[4] == 0 && self->current_time.bcd[5] == 0) {
        self->alarm_enabled = true; // Si la hora actual es 00:00, habilitamos la alarma
    }
}

/**
 * @brief Función para posponer la alarma en minutos aleatorios.
 * Incrementa la hora de la alarma en un número aleatorio de minutos entre 1 y 59.
 * Resetea el estado de self->alarm_ringing a falso.
 * @param self Puntero al reloj.
 * @param minutes Número de minutos a posponer (debe ser entre 1 y 59).
 * @return Verdadero si se pospuso la alarma correctamente, falso si el número de minutos es inválido.
 */
bool ClockPostponeAlarmRandomMinutes(clock_t self, uint8_t minutes) {
    if (minutes == 0 || minutes >= 60) {
        return false;
    }
    self->alarm_ringing = false;

    uint8_t hours_tens = self->alarm_time.time.hours[1];
    uint8_t hours_units = self->alarm_time.time.hours[0];
    uint8_t minutes_tens = self->alarm_time.time.minutes[1];
    uint8_t minutes_units = self->alarm_time.time.minutes[0];

    uint16_t total_minutes = (hours_tens * 10 + hours_units) * 60 + (minutes_tens * 10 + minutes_units);
    total_minutes = (total_minutes + minutes) % 1440; // Asegura que no supere 24h

    // Convertir de vuelta a BCD
    uint8_t new_hours = total_minutes / 60;
    uint8_t new_minutes = total_minutes % 60;

    self->alarm_time.time.hours[1] = new_hours / 10;
    self->alarm_time.time.hours[0] = new_hours % 10;
    self->alarm_time.time.minutes[1] = new_minutes / 10;
    self->alarm_time.time.minutes[0] = new_minutes % 10;

    return true;
}

/* === End of documentation ======================================================================================== */
