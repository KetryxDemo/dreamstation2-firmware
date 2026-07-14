/* test_cloud_uploader.c
 *
 * TC-8: TLS + cert-pin negative test (verifies SRS-8 / SRS-9).
 *
 * Confirms the uploader refuses to send when TLS is below 1.2 and when the
 * server certificate fails pinning, and only succeeds when both the TLS
 * floor and the pin are satisfied.
 */
#include <stdio.h>
#include <string.h>
#include "cloud_uploader.h"
#include "cert_pinning.h"

static int g_fails = 0;
#define CHECK(cond, msg) do { \
    if (!(cond)) { printf("FAIL: %s\n", (msg)); g_fails++; } \
    else { printf("PASS: %s\n", (msg)); } \
} while (0)

/* The primary pinned hash, mirrored from cert_pinning.c for the happy path. */
static const uint8_t k_good_pin[SPKI_SHA256_LEN] = {
    0x9a,0x3c,0x1f,0x77,0x42,0x0b,0xe5,0x88,
    0x21,0xd4,0x6c,0x90,0xaf,0x13,0x5e,0x74,
    0x8b,0x0d,0xc2,0x36,0x59,0xee,0x41,0x7a,
    0x02,0xb8,0x9f,0x11,0x63,0xda,0x4c,0x28
};

int main(void)
{
    upload_ctx_t ctx;

    /* Negative: TLS 1.0 (0x0301) must be refused before anything else. */
    ctx.tls_version = 0x0301u;
    ctx.server_spki_sha256 = k_good_pin;
    ctx.session_bytes = 4096u;
    CHECK(cloud_upload_session(&ctx) == UPLOAD_ERR_TLS_TOO_OLD,
          "refuses TLS below 1.2");

    /* Negative: good TLS but an unpinned (attacker) certificate. */
    uint8_t bad_pin[SPKI_SHA256_LEN];
    memset(bad_pin, 0xAB, sizeof(bad_pin));
    ctx.tls_version = TLS_VERSION_1_2;
    ctx.server_spki_sha256 = bad_pin;
    CHECK(cloud_upload_session(&ctx) == UPLOAD_ERR_CERT_PIN,
          "refuses an unpinned certificate");

    /* Negative: no certificate presented at all. */
    ctx.server_spki_sha256 = 0;
    CHECK(cloud_upload_session(&ctx) == UPLOAD_ERR_CERT_PIN,
          "refuses when no cert is presented");

    /* Positive: TLS 1.2 and the correct pinned cert succeed. */
    ctx.tls_version = TLS_VERSION_1_2;
    ctx.server_spki_sha256 = k_good_pin;
    CHECK(cloud_upload_session(&ctx) == UPLOAD_OK,
          "uploads over TLS 1.2 with a pinned cert");

    /* Positive: TLS 1.3 is also accepted. */
    ctx.tls_version = TLS_VERSION_1_3;
    CHECK(cloud_upload_session(&ctx) == UPLOAD_OK,
          "uploads over TLS 1.3 with a pinned cert");

    printf("%s\n", g_fails == 0 ? "TC-8 OK" : "TC-8 FAILED");
    return g_fails == 0 ? 0 : 1;
}
