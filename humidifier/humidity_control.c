/* humidity_control.c - heater-plate humidity control
 *
 * SRS-6: Control the heater plate to the humidity setpoint.
 *
 * A PI loop drives the water-chamber heater plate toward the user's
 * humidity setpoint. Output is a PWM duty for the heater FET. The integral
 * term is clamped (anti-windup) so a cold start or an open chamber does not
 * accumulate an unbounded command.
 */
#include "humidity_control.h"

/* PI gains, tuned on the bench for the DS2 chamber thermal mass. */
#define KP 40.0f
#define KI 8.0f

/* Anti-windup bound on the integral accumulator. */
#define INTEGRAL_MAX 1000.0f

void humidity_control_init(humidity_control_t *h, float setpoint_rh)
{
    if (h == 0) {
        return;
    }
    h->setpoint_rh = setpoint_rh;
    h->integral = 0.0f;
}

uint16_t humidity_control_step(humidity_control_t *h, float measured_rh, float dt_s)
{
    if (h == 0) {
        return 0u;
    }

    float error = h->setpoint_rh - measured_rh;

    h->integral += error * dt_s;
    if (h->integral > INTEGRAL_MAX) {
        h->integral = INTEGRAL_MAX;
    } else if (h->integral < 0.0f) {
        h->integral = 0.0f;   /* heater cannot cool; no negative windup */
    }

    float duty = (KP * error) + (KI * h->integral);
    if (duty < 0.0f) {
        duty = 0.0f;
    } else if (duty > (float)HEATER_DUTY_MAX) {
        duty = (float)HEATER_DUTY_MAX;
    }

    return (uint16_t)duty;
}
