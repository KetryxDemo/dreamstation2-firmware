/* pressure_sensor.h - DreamStation 2 mask-pressure sensor interface
 *
 * SRS-1: Sample the mask pressure sensor at >= 100 Hz.
 */
#ifndef PRESSURE_SENSOR_H
#define PRESSURE_SENSOR_H

#include <stdint.h>
#include <stdbool.h>

/* Minimum sampling rate mandated by SRS-1. */
#define PRESSURE_SAMPLE_HZ_MIN 100u

/* One pressure reading: cmH2O scaled by 100 (e.g. 850 == 8.50 cmH2O). */
typedef struct {
    uint32_t timestamp_ms;   /* monotonic sample time */
    int32_t  pressure_cmh2o_x100;
    bool     valid;          /* false if the ADC flagged a fault */
} pressure_sample_t;

/* Configure the sensor timer for the requested rate. Returns the actual
 * rate that will be used, which is never below PRESSURE_SAMPLE_HZ_MIN. */
uint32_t pressure_sensor_init(uint32_t requested_hz);

/* Pop the most recent sample. Returns false if none is ready yet. */
bool pressure_sensor_read(pressure_sample_t *out);

#endif /* PRESSURE_SENSOR_H */
