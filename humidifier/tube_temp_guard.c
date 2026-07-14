/* tube_temp_guard.c - heated-tube over-temperature guard
 *
 * SRS-7: Disable the heated tube if tube temperature > 43 C.
 *
 * The heated tube carries a thermistor at the mask end. If the measured
 * temperature exceeds the 43 C patient-contact limit we cut tube power and
 * latch the fault. The latch prevents the heater from oscillating on and
 * off right at the threshold; it clears only on an explicit reset after the
 * tube has cooled. This module is intentionally small so TC-7 can exercise
 * the exact cutoff boundary.
 */
#include "tube_temp_guard.h"

void tube_temp_guard_init(tube_temp_guard_t *g)
{
    if (g == 0) {
        return;
    }
    g->tube_enabled = true;
    g->tripped = false;
}

bool tube_temp_guard_update(tube_temp_guard_t *g, float tube_temp_c)
{
    if (g == 0) {
        return false;
    }

    /* Strictly greater than the limit trips the guard. 43.0 C exactly is
     * still allowed; anything above cuts power. */
    if (tube_temp_c > TUBE_TEMP_LIMIT_C) {
        g->tripped = true;
    }

    if (g->tripped) {
        g->tube_enabled = false;   /* latched off */
    }

    return g->tube_enabled;
}

void tube_temp_guard_reset(tube_temp_guard_t *g)
{
    if (g == 0) {
        return;
    }
    g->tripped = false;
    g->tube_enabled = true;
}
