/* test_apnea_detector.c
 *
 * TC-2: Bench-sim apnea detection (verifies SRS-2).
 *
 * Feeds a simulated airflow trace into the detector and checks that an
 * apnea fires only after airflow stays below 10% of baseline for >= 10 s,
 * and does not fire for shorter drops or partial reductions.
 */
#include <stdio.h>
#include "apnea_detector.h"

static int g_fails = 0;
#define CHECK(cond, msg) do { \
    if (!(cond)) { printf("FAIL: %s\n", (msg)); g_fails++; } \
    else { printf("PASS: %s\n", (msg)); } \
} while (0)

/* Drive the detector with a flat flow for a number of 100 ms ticks,
 * returning true if an apnea fired anywhere in the window. */
static int run_flow(apnea_detector_t *d, float flow_lpm, int ticks)
{
    int fired = 0;
    for (int i = 0; i < ticks; i++) {
        if (apnea_detector_update(d, flow_lpm, 100u) == RESP_EVENT_APNEA) {
            fired = 1;
        }
    }
    return fired;
}

int main(void)
{
    apnea_detector_t d;
    float baseline = 20.0f;   /* 20 L/min baseline airflow */

    /* Near-zero flow for 10 s (100 ticks * 100 ms) must trip an apnea. */
    apnea_detector_init(&d, baseline);
    CHECK(run_flow(&d, 0.5f, 100), "apnea fires after 10 s of no flow");

    /* Near-zero flow for only 5 s must NOT trip. */
    apnea_detector_init(&d, baseline);
    CHECK(!run_flow(&d, 0.5f, 50), "no apnea for a 5 s drop");

    /* Normal breathing (full baseline) never trips. */
    apnea_detector_init(&d, baseline);
    CHECK(!run_flow(&d, baseline, 200), "no apnea during normal breathing");

    /* A partial reduction (50% of baseline) is not an apnea. */
    apnea_detector_init(&d, baseline);
    CHECK(!run_flow(&d, baseline * 0.5f, 200), "50% flow is not an apnea");

    printf("%s\n", g_fails == 0 ? "TC-2 OK" : "TC-2 FAILED");
    return g_fails == 0 ? 0 : 1;
}
