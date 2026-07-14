/* touchscreen_app.h - therapy summary UI
 *
 * SRS-10: Display therapy hours, AHI, and mask fit.
 */
#ifndef TOUCHSCREEN_APP_H
#define TOUCHSCREEN_APP_H

#include <stdint.h>

typedef struct {
    float    therapy_hours;   /* hours of therapy last session */
    float    ahi;             /* apnea-hypopnea index, events/hour */
    uint8_t  mask_fit_pct;    /* mask seal quality, 0..100 */
} therapy_summary_t;

/* Render the "My SleepProgress" home card from a session summary. Returns
 * the number of rows written to the display buffer. */
int touchscreen_render_summary(const therapy_summary_t *s);

#endif /* TOUCHSCREEN_APP_H */
