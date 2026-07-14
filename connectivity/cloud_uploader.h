/* cloud_uploader.h - nightly session upload
 *
 * SRS-8: Upload session data over TLS 1.2 or newer.
 */
#ifndef CLOUD_UPLOADER_H
#define CLOUD_UPLOADER_H

#include <stdint.h>
#include <stdbool.h>

/* Negotiated TLS versions, encoded as (major<<8 | minor) like the wire. */
#define TLS_VERSION_1_2 0x0303u
#define TLS_VERSION_1_3 0x0304u

typedef enum {
    UPLOAD_OK = 0,
    UPLOAD_ERR_TLS_TOO_OLD,   /* refused: negotiated below TLS 1.2 */
    UPLOAD_ERR_CERT_PIN,      /* refused: server cert failed pinning */
    UPLOAD_ERR_NETWORK
} upload_result_t;

typedef struct {
    uint16_t tls_version;         /* negotiated TLS version */
    const uint8_t *server_spki_sha256;  /* server key hash presented in handshake */
    uint32_t session_bytes;       /* payload size */
} upload_ctx_t;

/* Attempt to upload one therapy session. Enforces the TLS 1.2+ floor
 * (SRS-8) and certificate pinning (SRS-9) before any payload is sent. */
upload_result_t cloud_upload_session(const upload_ctx_t *ctx);

#endif /* CLOUD_UPLOADER_H */
