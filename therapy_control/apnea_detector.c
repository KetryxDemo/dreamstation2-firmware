/* apnea_detector.c - respiratory-event detection
 *
 * SRS-2: Detect apnea    (airflow < 10% of baseline for >= 10 s).
 * SRS-3: Detect hypopnea (airflow 30-90% of baseline for >= 10 s).
 *
 * Airflow is derived from the blower and pressure signals upstream and
 * handed to this detector one sample at a time. We track how long flow has
 * stayed below the apnea threshold (apnea) and how long it has sat in the
 * partial-obstruction band (hypopnea); once either dwell reaches 10 s we
 * report the event and reset that timer. Apnea takes priority: a full
 * cessation is not also counted as a hypopnea.
 */
#include "apnea_detector.h"

/* Airflow at or below this fraction of baseline counts as "no flow". */
#define APNEA_FLOW_FRACTION      0.10f

/* Hypopnea band: a sustained partial reduction of baseline airflow. */
#define HYPOPNEA_FLOW_LOW_FRAC   0.30f
#define HYPOPNEA_FLOW_HIGH_FRAC  0.90f

/* Minimum dwell time before an event is declared. */
#define EVENT_MIN_DURATION_MS 10000u

void apnea_detector_init(apnea_detector_t *d, float baseline_flow_lpm)
{
    if (d == 0) {
        return;
    }
    d->baseline_flow_lpm = baseline_flow_lpm;
    d->low_flow_ms = 0u;
    d->partial_flow_ms = 0u;
}

resp_event_t apnea_detector_update(apnea_detector_t *d,
                                   float flow_lpm,
                                   uint32_t dt_ms)
{
    if (d == 0) {
        return RESP_EVENT_NONE;
    }

    float apnea_threshold  = d->baseline_flow_lpm * APNEA_FLOW_FRACTION;
    float hypo_low         = d->baseline_flow_lpm * HYPOPNEA_FLOW_LOW_FRAC;
    float hypo_high        = d->baseline_flow_lpm * HYPOPNEA_FLOW_HIGH_FRAC;

    if (flow_lpm < apnea_threshold) {
        /* Full cessation: count toward apnea, not hypopnea. */
        d->partial_flow_ms = 0u;
        d->low_flow_ms += dt_ms;
        if (d->low_flow_ms >= EVENT_MIN_DURATION_MS) {
            d->low_flow_ms = 0u;
            return RESP_EVENT_APNEA;
        }
    } else if (flow_lpm >= hypo_low && flow_lpm < hypo_high) {
        /* Sustained partial reduction: count toward hypopnea. */
        d->low_flow_ms = 0u;
        d->partial_flow_ms += dt_ms;
        if (d->partial_flow_ms >= EVENT_MIN_DURATION_MS) {
            d->partial_flow_ms = 0u;
            return RESP_EVENT_HYPOPNEA;
        }
    } else {
        /* Flow recovered (or is at/above baseline); reset both dwells. */
        d->low_flow_ms = 0u;
        d->partial_flow_ms = 0u;
    }

    return RESP_EVENT_NONE;
}
