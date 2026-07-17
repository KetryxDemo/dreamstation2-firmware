---
itemId: cert-pinning
itemType: Software Item Spec
itemTitle: Endpoint Certificate Pinning
itemFulfills: RESP-9
---

# Endpoint Certificate Pinning

Verifies the cloud endpoint via certificate pinning to block MITM.

Implemented in `connectivity/cert_pinning.c`. See the module's unit tests under `tests/`.

## Architecture

```mermaid
flowchart LR
    UP[Cloud Session Uploader] --> PIN[Endpoint Certificate Pinning]
    STORE[Pinned Public Key / Cert Hash] --> PIN
    PIN --> TLS[TLS Handshake]
    TLS --> CLOUD[(Cloud Endpoint)]
```

## Verification flow

```mermaid
flowchart TD
    H[TLS handshake: server cert received] --> X[Compute cert / key hash]
    X --> C{Matches pinned value?}
    C -->|yes| OK[Continue handshake]
    C -->|no| ABORT[Abort connection - possible MITM]
    OK --> SEND[Allow data upload]
    ABORT --> LOG[Log security event]
```
