/* humidity_control.h - heater-plate humidity control
 *
 * SRS-6: Control the heater plate to the humidity setpoint.
 */
#ifndef HUMIDITY_CONTROL_H
#define HUMIDITY_CONTROL_H

#include <stdint.h>

typedef struct {
    float setpoint_rh;   /* target relative humidity, percent */
    float integral;      /* PI integral accumulator */
} humidity_control_t;

/* Heater duty cycle is expressed 0..1000 (tenths of a percent) for the PWM. */
#define HEATER_DUTY_MAX 1000u

void humidity_control_init(humidity_control_t *h, float setpoint_rh);

/* Run one control step given the measured chamber humidity. Returns the
 * heater-plate PWM duty (0..HEATER_DUTY_MAX). */
uint16_t humidity_control_step(humidity_control_t *h, float measured_rh, float dt_s);

#endif /* HUMIDITY_CONTROL_H */
