/* pressure_controller.h - auto-titration pressure controller
 *
 * SRS-4: Raise pressure in 1 cmH2O steps up to the configured max.
 */
#ifndef PRESSURE_CONTROLLER_H
#define PRESSURE_CONTROLLER_H

#include <stdint.h>
#include "apnea_detector.h"

typedef struct {
    float min_pressure;   /* configured lower bound, cmH2O */
    float max_pressure;   /* configured upper bound, cmH2O */
    float commanded;      /* current commanded pressure, cmH2O */
} pressure_controller_t;

/* Titration step size mandated by SRS-4. */
#define TITRATION_STEP_CMH2O 1.0f

void pressure_controller_init(pressure_controller_t *pc,
                              float min_pressure,
                              float max_pressure);

/* React to a respiratory event. On an apnea/hypopnea the controller steps
 * pressure up by 1 cmH2O, never past the configured max. Returns the new
 * commanded pressure. */
float pressure_controller_on_event(pressure_controller_t *pc,
                                   resp_event_t event);

#endif /* PRESSURE_CONTROLLER_H */
