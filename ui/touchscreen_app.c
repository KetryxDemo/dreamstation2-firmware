/* touchscreen_app.c - therapy summary UI
 *
 * SRS-10: Display therapy hours, AHI, and mask fit.
 *
 * The color touchscreen home screen shows the patient a plain-language
 * summary of last night's therapy: hours used, the apnea-hypopnea index,
 * and a mask-fit seal indicator. This module formats the three metrics
 * into display rows; the graphics layer paints them. Formatting is kept
 * separate from painting so the values are easy to verify.
 */
#include <stdio.h>
#include "touchscreen_app.h"

/* Display buffer rows the graphics layer will paint. */
#define UI_MAX_ROWS 3
static char g_rows[UI_MAX_ROWS][40];

/* Map a mask-fit percentage to a friendly label. */
static const char *mask_fit_label(uint8_t pct)
{
    if (pct >= 90u) {
        return "Good seal";
    } else if (pct >= 70u) {
        return "Fair seal";
    }
    return "Check mask";
}

int touchscreen_render_summary(const therapy_summary_t *s)
{
    if (s == 0) {
        return 0;
    }

    snprintf(g_rows[0], sizeof(g_rows[0]), "Therapy: %.1f hours", s->therapy_hours);
    snprintf(g_rows[1], sizeof(g_rows[1]), "AHI: %.1f events/hr", s->ahi);
    snprintf(g_rows[2], sizeof(g_rows[2]), "Mask fit: %u%% (%s)",
             (unsigned)s->mask_fit_pct, mask_fit_label(s->mask_fit_pct));

    /* display_paint_rows(g_rows, UI_MAX_ROWS);  <- graphics layer */
    return UI_MAX_ROWS;
}
