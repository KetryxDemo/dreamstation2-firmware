/* apnea_detector.h - respiratory-event detection
 *
 * SRS-2: Detect apnea  (airflow < 10% of baseline for >= 10 s).
 */
#ifndef APNEA_DETECTOR_H
#define APNEA_DETECTOR_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    RESP_EVENT_NONE = 0,
    RESP_EVENT_APNEA
} resp_event_t;

typedef struct {
    float    baseline_flow_lpm;  /* rolling baseline airflow, L/min */
    uint32_t low_flow_ms;        /* time spent below the apnea threshold */
} apnea_detector_t;

/* Reset the detector and seed the baseline airflow. */
void apnea_detector_init(apnea_detector_t *d, float baseline_flow_lpm);

/* Feed one airflow sample. dt_ms is the interval since the previous sample.
 * Returns the event detected on this update (NONE until a full event fires). */
resp_event_t apnea_detector_update(apnea_detector_t *d,
                                   float flow_lpm,
                                   uint32_t dt_ms);

#endif /* APNEA_DETECTOR_H */
