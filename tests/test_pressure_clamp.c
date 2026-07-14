/* test_pressure_clamp.c
 *
 * TC-5: Clamp never commands > 20 cmH2O (verifies SRS-5).
 *
 * Exercises the safety clamp across normal, over-range, under-range, and
 * misconfigured windows. The absolute 20 cmH2O ceiling must hold in every
 * case, including when the configured max is set too high.
 */
#include <stdio.h>
#include "pressure_clamp.h"

static int g_fails = 0;
#define CHECK(cond, msg) do { \
    if (!(cond)) { printf("FAIL: %s\n", (msg)); g_fails++; } \
    else { printf("PASS: %s\n", (msg)); } \
} while (0)

int main(void)
{
    /* In-range command passes through untouched. */
    CHECK(pressure_clamp(10.0f, 4.0f, 15.0f) == 10.0f,
          "in-range command unchanged");

    /* Above the configured max is pulled down to the max. */
    CHECK(pressure_clamp(18.0f, 4.0f, 15.0f) == 15.0f,
          "clamped down to configured max");

    /* Below the configured min is raised to the min. */
    CHECK(pressure_clamp(2.0f, 6.0f, 15.0f) == 6.0f,
          "raised up to configured min");

    /* A too-high configured max cannot lift the command past 20 cmH2O. */
    CHECK(pressure_clamp(99.0f, 4.0f, 40.0f) == 20.0f,
          "absolute ceiling holds at 20 cmH2O");

    /* Absolute ceiling holds even with an insane command and no headroom. */
    CHECK(pressure_clamp(1000.0f, 4.0f, 25.0f) <= 20.0f,
          "never commands above 20 cmH2O");

    /* Inverted window (min > max) is normalized, ceiling still holds. */
    CHECK(pressure_clamp(30.0f, 25.0f, 10.0f) <= 20.0f,
          "inverted window still capped at 20");

    printf("%s\n", g_fails == 0 ? "TC-5 OK" : "TC-5 FAILED");
    return g_fails == 0 ? 0 : 1;
}
