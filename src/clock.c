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

    clock_time_t alarm_time;         /**< Hora de la alarma */
    clock_time_t snoozed_alarm_time; /**< Hora de la alarma pospuesta */
    bool alarm_valid;                /**< Indicador de validez de la alarma */
    bool alarm_ringing;              /**< Indicador de si la alarma está sonando */
    bool alarm_enabled;              /**< Indicador de si la alarma está habilitada */
};

/* === Private function definitions ================================================================================ */

/* === Public function implementation ============================================================================== */

clock_t ClockCreate(void) {
    static struct clock_s self[1];
    memset(self, 0, sizeof(struct clock_s));
    self->valid = false;
    return self;
}

bool ClockGetTime(clock_t self, clock_time_t * result) {
    memcpy(result, &self->current_time, sizeof(clock_time_t));
    return self->valid;
}

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

void ClockNewTick(clock_t self) {
    self->ticks_per_second++;
    if (self->ticks_per_second == 10000) { /**< 10000 ticks por segundo = 1 segundo */
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

bool ClockSetAlarm(clock_t self, const clock_time_t * alarm_time) {
    self->valid = true;
    memcpy(&self->alarm_time, alarm_time, sizeof(clock_time_t));
    memcpy(&self->snoozed_alarm_time, alarm_time, sizeof(clock_time_t));
    return self->valid;
}

bool ClockGetAlarm(clock_t self, clock_time_t * alarm_time) {
    memcpy(alarm_time, &self->alarm_time, sizeof(clock_time_t));
    return self->valid;
}

bool ClockAlarmIsRinging(clock_t self) {
    if (self->current_time.time.hours[0] == self->snoozed_alarm_time.time.hours[0] &&
        self->current_time.time.hours[1] == self->snoozed_alarm_time.time.hours[1] &&
        self->current_time.time.minutes[0] == self->snoozed_alarm_time.time.minutes[0] &&
        self->current_time.time.minutes[1] == self->snoozed_alarm_time.time.minutes[1] &&
        self->current_time.time.seconds[0] == self->snoozed_alarm_time.time.seconds[0] &&
        self->current_time.time.seconds[1] == self->snoozed_alarm_time.time.seconds[1]) {
        if (self->alarm_enabled) {
            self->alarm_ringing = true;
        } else {
            memcpy(&self->snoozed_alarm_time, &self->alarm_time, sizeof(clock_time_t));
        }
    }

    return self->alarm_ringing;
}

void ClockSetStateAlarm(clock_t self, bool enable) {
    if (enable) {
        self->alarm_enabled = true;
    } else {
        self->alarm_enabled = false;
    }
}

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

void ClockResetAlarm(clock_t clock) {
    clock->alarm_valid = false;
    clock->alarm_ringing = false;
    clock->alarm_enabled = false;
    memset(&clock->alarm_time, 0, sizeof(clock_time_t));
}

void ClockRestartAlarm(clock_t self) {
    self->alarm_ringing = false;
    self->alarm_enabled = true;

    if (self->current_time.bcd[0] == 0 && self->current_time.bcd[1] == 0 && self->current_time.bcd[2] == 0 &&
        self->current_time.bcd[3] == 0 && self->current_time.bcd[4] == 0 && self->current_time.bcd[5] == 0) {
        self->alarm_enabled = true; /**< Si la hora actual es 00:00, habilitamos la alarma */
    }
}

bool ClockPostponeAlarmRandomMinutes(clock_t self, uint8_t minutes) {
    if (minutes == 0 || minutes >= 60) {
        return false;
    }

    self->alarm_ringing = false;

    /**< Convertir la hora actual de la alarma a minutos totales */
    uint8_t hours = self->current_time.time.hours[1] * 10 + self->current_time.time.hours[0];
    uint8_t mins = self->current_time.time.minutes[1] * 10 + self->current_time.time.minutes[0];

    uint16_t total_minutes = hours * 60 + mins;

    /**< Sumar los minutos */
    total_minutes = (total_minutes + minutes) % 1440; /**< 1440 = 24 * 60 minutos */

    /**< Convertir de nuevo a horas y minutos */
    uint8_t new_hours = total_minutes / 60;
    uint8_t new_minutes = total_minutes % 60;

    /**< Guardar en formato BCD nuevamente */
    self->snoozed_alarm_time.time.hours[1] = new_hours / 10;
    self->snoozed_alarm_time.time.hours[0] = new_hours % 10;
    self->snoozed_alarm_time.time.minutes[1] = new_minutes / 10;
    self->snoozed_alarm_time.time.minutes[0] = new_minutes % 10;

    return true;
}

void ClockPostponeAlarmOneDay(clock_t self) {
    self->alarm_ringing = false;
    self->snoozed_alarm_time = self->alarm_time; /**< Guardar la hora de la alarma original */
}

void IncrementMinutes(clock_time_t * clock) {
    clock->time.minutes[0]++;
    if (clock->time.minutes[0] > 9) {
        clock->time.minutes[0] = 0;
        clock->time.minutes[1]++;
        if (clock->time.minutes[1] > 5) {
            clock->time.minutes[1] = 0;
            clock->time.minutes[0] = 0;
        }
    }
}

void DecrementMinutes(clock_time_t * clock) {
    if (clock->time.minutes[0] == 0) {
        clock->time.minutes[0] = 9;
        if (clock->time.minutes[1] == 0) {
            clock->time.minutes[1] = 5;
            clock->time.minutes[0] = 9;
        } else {
            clock->time.minutes[1]--;
        }
    } else {
        clock->time.minutes[0]--;
    }
}

void IncrementHours(clock_time_t * clock) {
    if (clock->time.hours[0] == 3 && clock->time.hours[1] == 2) {
        clock->time.hours[0] = 0;
        clock->time.hours[1] = 0;
    } else if (clock->time.hours[0] == 9) {
        clock->time.hours[0] = 0;
        clock->time.hours[1]++;
    } else {
        clock->time.hours[0]++;
    }
}

void DecrementHours(clock_time_t * clock) {
    if (clock->time.hours[0] == 0 && clock->time.hours[1] == 0) {
        clock->time.hours[0] = 3;
        clock->time.hours[1] = 2;
    } else if (clock->time.hours[0] == 0) {
        clock->time.hours[0] = 9;
        if (clock->time.hours[1] == 0) {
            clock->time.hours[1] = 2;
        } else {
            clock->time.hours[1]--;
        }
    } else {
        clock->time.hours[0]--;
    }
}
/* === End of documentation ======================================================================================== */
