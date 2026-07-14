/* tube_temp_guard.h - heated-tube over-temperature guard
 *
 * SRS-7: Disable the heated tube if tube temperature > 43 C.
 */
#ifndef TUBE_TEMP_GUARD_H
#define TUBE_TEMP_GUARD_H

#include <stdbool.h>

/* Patient-contact safety limit for the heated tube. */
#define TUBE_TEMP_LIMIT_C 43.0f

typedef struct {
    bool tube_enabled;   /* false once the guard has tripped */
    bool tripped;        /* latched fault flag */
} tube_temp_guard_t;

void tube_temp_guard_init(tube_temp_guard_t *g);

/* Evaluate the latest tube temperature. Returns true if the heated tube is
 * allowed to run, false if the guard has cut it off. The trip latches until
 * an explicit reset so the tube does not chatter around the limit. */
bool tube_temp_guard_update(tube_temp_guard_t *g, float tube_temp_c);

/* Clear a latched trip (e.g. after the tube has cooled and on user ack). */
void tube_temp_guard_reset(tube_temp_guard_t *g);

#endif /* TUBE_TEMP_GUARD_H */
