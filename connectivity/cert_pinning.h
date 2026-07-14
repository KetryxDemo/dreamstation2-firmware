/* cert_pinning.h - cloud endpoint certificate pinning
 *
 * SRS-9: Certificate-pin the cloud endpoint.
 */
#ifndef CERT_PINNING_H
#define CERT_PINNING_H

#include <stdint.h>
#include <stdbool.h>

/* SHA-256 of the pinned server SubjectPublicKeyInfo. */
#define SPKI_SHA256_LEN 32u

/* Compare a presented server key hash against the built-in pin(s).
 * Returns true only on an exact match against a pinned key. A NULL hash
 * (no certificate presented) always fails. */
bool cert_pin_verify(const uint8_t *presented_spki_sha256);

#endif /* CERT_PINNING_H */
