/* pressure_sensor.c - mask-pressure sensor sampling
 *
 * SRS-1: Sample the mask pressure sensor at >= 100 Hz.
 *
 * The pressure transducer feeds a 12-bit ADC. A hardware timer drives
 * the conversion at a fixed rate; the ISR pushes each converted reading
 * into a single-slot mailbox that the therapy loop drains. We never let
 * the configured rate drop below 100 Hz so the auto-titration loop always
 * sees breath-resolution data.
 */
#include "pressure_sensor.h"

/* Hardware timer base clock (Hz). Divided down to the sample rate. */
#define TIMER_BASE_HZ 48000u

/* Single-slot mailbox written by the ISR, read by the therapy loop. */
static volatile pressure_sample_t g_latest;
static volatile bool g_has_sample;
static uint32_t g_sample_hz;

/* Convert a raw 12-bit ADC code to cmH2O x100. The transducer is linear
 * over 0..40 cmH2O across the full 0..4095 code range. */
static int32_t adc_code_to_cmh2o_x100(uint16_t code)
{
    /* 40.00 cmH2O == 4000 (x100) spread over 4095 codes. */
    return ((int32_t)code * 4000) / 4095;
}

uint32_t pressure_sensor_init(uint32_t requested_hz)
{
    uint32_t hz = requested_hz;
    if (hz < PRESSURE_SAMPLE_HZ_MIN) {
        hz = PRESSURE_SAMPLE_HZ_MIN;   /* enforce SRS-1 floor */
    }

    /* Pick a divider that yields at least the requested rate. */
    uint32_t divider = TIMER_BASE_HZ / hz;
    if (divider == 0u) {
        divider = 1u;
    }
    g_sample_hz = TIMER_BASE_HZ / divider;
    g_has_sample = false;

    /* hw_timer_start(divider);  <- board support, omitted in demo */
    return g_sample_hz;
}

/* Called from the sample-timer ISR with a fresh ADC code. */
void pressure_sensor_on_adc(uint32_t timestamp_ms, uint16_t code, bool fault)
{
    g_latest.timestamp_ms = timestamp_ms;
    g_latest.pressure_cmh2o_x100 = adc_code_to_cmh2o_x100(code);
    g_latest.valid = !fault;
    g_has_sample = true;
}

bool pressure_sensor_read(pressure_sample_t *out)
{
    if (!g_has_sample || out == 0) {
        return false;
    }
    *out = g_latest;
    g_has_sample = false;
    return true;
}
