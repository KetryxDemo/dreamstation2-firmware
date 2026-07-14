/* cert_pinning.c - cloud endpoint certificate pinning
 *
 * SRS-9: Certificate-pin the cloud endpoint.
 *
 * The firmware ships with the SHA-256 of the cloud endpoint's public key
 * (SubjectPublicKeyInfo). During the TLS handshake the presented server
 * key is hashed and compared against this pin. A primary and a backup pin
 * are held so the server key can be rotated without a firmware update.
 * Comparison is constant-time to avoid leaking match progress.
 */
#include "cert_pinning.h"

/* Pinned SPKI SHA-256 hashes (demo values). Primary + rotation backup. */
static const uint8_t k_pins[2][SPKI_SHA256_LEN] = {
    { /* primary */
      0x9a,0x3c,0x1f,0x77,0x42,0x0b,0xe5,0x88,
      0x21,0xd4,0x6c,0x90,0xaf,0x13,0x5e,0x74,
      0x8b,0x0d,0xc2,0x36,0x59,0xee,0x41,0x7a,
      0x02,0xb8,0x9f,0x11,0x63,0xda,0x4c,0x28 },
    { /* backup */
      0x1b,0x22,0x8e,0x04,0xf6,0x37,0x99,0xc1,
      0x50,0x6a,0xd3,0x2f,0x84,0x0e,0x7c,0xb5,
      0x39,0xa7,0x12,0x68,0xcd,0x40,0x93,0x5b,
      0x76,0x1e,0xef,0x82,0x0a,0x34,0xd9,0x67 },
};

/* Constant-time equality over SPKI_SHA256_LEN bytes. */
static bool ct_equal(const uint8_t *a, const uint8_t *b)
{
    uint8_t diff = 0u;
    for (uint32_t i = 0u; i < SPKI_SHA256_LEN; i++) {
        diff |= (uint8_t)(a[i] ^ b[i]);
    }
    return diff == 0u;
}

bool cert_pin_verify(const uint8_t *presented_spki_sha256)
{
    if (presented_spki_sha256 == 0) {
        return false;   /* no certificate presented -> reject */
    }

    bool match = false;
    for (uint32_t p = 0u; p < 2u; p++) {
        /* OR the result rather than early-return to keep timing flat. */
        match |= ct_equal(presented_spki_sha256, k_pins[p]);
    }
    return match;
}
