/* pressure_clamp.c - safety clamp on commanded therapy pressure
 *
 * SRS-5: Clamp commanded pressure to [min,max] and never exceed 20 cmH2O.
 *
 * This is the last stage before the blower setpoint. It enforces two
 * things: the clinician-configured window, and the absolute device safety
 * envelope of 4..20 cmH2O. The absolute ceiling wins even if a bad config
 * or upstream fault asks for more. This function is deliberately tiny and
 * side-effect free so it is easy to reason about and unit test (TC-5).
 */
#include "pressure_clamp.h"

float pressure_clamp(float commanded, float configured_min, float configured_max)
{
    /* Normalize a possibly-inverted configured window. */
    float lo = configured_min;
    float hi = configured_max;
    if (lo > hi) {
        float tmp = lo;
        lo = hi;
        hi = tmp;
    }

    /* Fold in the absolute safety envelope. */
    if (lo < ABSOLUTE_MIN_PRESSURE_CMH2O) {
        lo = ABSOLUTE_MIN_PRESSURE_CMH2O;
    }
    if (hi > ABSOLUTE_MAX_PRESSURE_CMH2O) {
        hi = ABSOLUTE_MAX_PRESSURE_CMH2O;
    }

    /* Clamp the command into the resulting safe window. */
    float safe = commanded;
    if (safe < lo) {
        safe = lo;
    }
    if (safe > hi) {
        safe = hi;
    }

    /* Belt and suspenders: never return above the absolute ceiling. */
    if (safe > ABSOLUTE_MAX_PRESSURE_CMH2O) {
        safe = ABSOLUTE_MAX_PRESSURE_CMH2O;
    }
    return safe;
}
