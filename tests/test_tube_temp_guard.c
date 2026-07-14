/* test_tube_temp_guard.c
 *
 * TC-7: Tube cutoff at 43 C (verifies SRS-7).
 *
 * Checks the heated-tube guard boundary: 43.0 C is allowed, anything above
 * cuts power, and the trip latches until an explicit reset.
 */
#include <stdio.h>
#include "tube_temp_guard.h"

static int g_fails = 0;
#define CHECK(cond, msg) do { \
    if (!(cond)) { printf("FAIL: %s\n", (msg)); g_fails++; } \
    else { printf("PASS: %s\n", (msg)); } \
} while (0)

int main(void)
{
    tube_temp_guard_t g;

    /* Comfortable temperature: tube stays enabled. */
    tube_temp_guard_init(&g);
    CHECK(tube_temp_guard_update(&g, 30.0f) == 1, "tube on at 30 C");

    /* Exactly at the limit is still allowed. */
    tube_temp_guard_init(&g);
    CHECK(tube_temp_guard_update(&g, 43.0f) == 1, "tube on at exactly 43 C");

    /* Just above the limit cuts power. */
    tube_temp_guard_init(&g);
    CHECK(tube_temp_guard_update(&g, 43.1f) == 0, "tube off above 43 C");

    /* The trip latches: staying off even after the tube cools. */
    CHECK(tube_temp_guard_update(&g, 25.0f) == 0, "trip latches after cooldown");

    /* Explicit reset re-enables the tube. */
    tube_temp_guard_reset(&g);
    CHECK(tube_temp_guard_update(&g, 25.0f) == 1, "reset re-enables tube");

    printf("%s\n", g_fails == 0 ? "TC-7 OK" : "TC-7 FAILED");
    return g_fails == 0 ? 0 : 1;
}
