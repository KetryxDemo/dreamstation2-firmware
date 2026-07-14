/* pressure_clamp.h - safety clamp on commanded therapy pressure
 *
 * SRS-5: Clamp commanded pressure to [min,max] and never exceed 20 cmH2O.
 */
#ifndef PRESSURE_CLAMP_H
#define PRESSURE_CLAMP_H

/* Absolute hardware safety ceiling. No commanded pressure may exceed this
 * regardless of configuration. */
#define ABSOLUTE_MAX_PRESSURE_CMH2O 20.0f

/* Absolute therapy floor. */
#define ABSOLUTE_MIN_PRESSURE_CMH2O 4.0f

/* Clamp a raw commanded pressure into [min,max], then hard-limit to the
 * absolute [4,20] cmH2O safety envelope. Returns the safe value. */
float pressure_clamp(float commanded, float configured_min, float configured_max);

#endif /* PRESSURE_CLAMP_H */
