/* apnea_detector.c - respiratory-event detection
 *
 * SRS-2: Detect apnea (airflow < 10% of baseline for >= 10 s).
 *
 * Airflow is derived from the blower and pressure signals upstream and
 * handed to this detector one sample at a time. We track how long flow has
 * stayed below the apnea threshold; once that dwell reaches 10 s we report
 * an apnea and reset the timer.
 */
#include "apnea_detector.h"

/* Airflow at or below this fraction of baseline counts as "no flow". */
#define APNEA_FLOW_FRACTION   0.10f

/* Minimum dwell time before an event is declared. */
#define EVENT_MIN_DURATION_MS 10000u

void apnea_detector_init(apnea_detector_t *d, float baseline_flow_lpm)
{
    if (d == 0) {
        return;
    }
    d->baseline_flow_lpm = baseline_flow_lpm;
    d->low_flow_ms = 0u;
}

resp_event_t apnea_detector_update(apnea_detector_t *d,
                                   float flow_lpm,
                                   uint32_t dt_ms)
{
    if (d == 0) {
        return RESP_EVENT_NONE;
    }

    float apnea_threshold = d->baseline_flow_lpm * APNEA_FLOW_FRACTION;

    if (flow_lpm < apnea_threshold) {
        d->low_flow_ms += dt_ms;
        if (d->low_flow_ms >= EVENT_MIN_DURATION_MS) {
            d->low_flow_ms = 0u;
            return RESP_EVENT_APNEA;
        }
    } else {
        /* Flow recovered; reset the dwell counter. */
        d->low_flow_ms = 0u;
    }

    return RESP_EVENT_NONE;
}
