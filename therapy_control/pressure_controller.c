/* pressure_controller.c - auto-titration pressure controller
 *
 * SRS-4: Raise pressure in 1 cmH2O steps up to the configured max.
 *
 * The auto-CPAP algorithm responds to respiratory events by nudging the
 * commanded pressure upward one step at a time. Small steps keep therapy
 * comfortable and avoid overshoot. The step never carries the command past
 * the clinician-configured maximum; the downstream safety clamp (SRS-5)
 * enforces the absolute 20 cmH2O ceiling regardless.
 */
#include "pressure_controller.h"

void pressure_controller_init(pressure_controller_t *pc,
                              float min_pressure,
                              float max_pressure)
{
    if (pc == 0) {
        return;
    }
    pc->min_pressure = min_pressure;
    pc->max_pressure = max_pressure;
    pc->commanded = min_pressure;   /* therapy starts at the low end */
}

float pressure_controller_on_event(pressure_controller_t *pc,
                                   resp_event_t event)
{
    if (pc == 0) {
        return 0.0f;
    }

    if (event == RESP_EVENT_APNEA || event == RESP_EVENT_HYPOPNEA) {
        float next = pc->commanded + TITRATION_STEP_CMH2O;
        if (next > pc->max_pressure) {
            next = pc->max_pressure;   /* do not step past configured max */
        }
        pc->commanded = next;
    }

    return pc->commanded;
}
