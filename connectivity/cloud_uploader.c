/* cloud_uploader.c - nightly session upload
 *
 * SRS-8: Upload session data over TLS 1.2 or newer.
 *
 * The device uploads each night's compliance/therapy session to the cloud
 * over Bluetooth-tethered or cellular transport. Before any patient data
 * leaves the device we require a TLS 1.2+ channel; anything older is
 * refused outright. Certificate pinning (SRS-9) is checked immediately
 * after the version gate so a downgraded or spoofed endpoint never sees
 * the payload.
 */
#include "cloud_uploader.h"
#include "cert_pinning.h"

/* Reject any negotiated version below TLS 1.2 per SRS-8. */
static bool tls_version_ok(uint16_t negotiated)
{
    return negotiated >= TLS_VERSION_1_2;
}

upload_result_t cloud_upload_session(const upload_ctx_t *ctx)
{
    if (ctx == 0) {
        return UPLOAD_ERR_NETWORK;
    }

    /* 1. TLS version floor (SRS-8). */
    if (!tls_version_ok(ctx->tls_version)) {
        return UPLOAD_ERR_TLS_TOO_OLD;
    }

    /* 2. Certificate pinning (SRS-9). */
    if (!cert_pin_verify(ctx->server_spki_sha256)) {
        return UPLOAD_ERR_CERT_PIN;
    }

    /* 3. Stream the session payload. Transport details omitted in demo. */
    /* transport_send(ctx->session_bytes ...); */

    return UPLOAD_OK;
}
