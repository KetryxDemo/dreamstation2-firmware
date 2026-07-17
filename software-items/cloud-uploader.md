---
itemId: cloud-uploader
itemType: Software Item Spec
itemTitle: Cloud Session Uploader
itemFulfills: RESP-8
---

# Cloud Session Uploader

Uploads nightly session data to the cloud over TLS 1.2+.

Implemented in `connectivity/cloud_uploader.c`. See the module's unit tests under `tests/`.

## Architecture

```mermaid
flowchart LR
    SESS[Nightly Session Data] --> UP[Cloud Session Uploader]
    UP --> TLS[TLS 1.2+ Channel]
    PIN[Endpoint Certificate Pinning] --> TLS
    TLS --> CLOUD[(Cloud Backend)]
```

## Upload sequence

```mermaid
sequenceDiagram
    participant S as Therapy Session Store
    participant U as Cloud Uploader
    participant P as Cert Pinning
    participant C as Cloud Backend
    S->>U: session complete
    U->>C: open TLS 1.2+ connection
    C-->>U: server certificate
    U->>P: verify pinned certificate
    alt certificate valid
        P-->>U: OK
        U->>C: upload session data
        C-->>U: 200 OK
    else mismatch or upload fails
        P-->>U: reject
        U->>U: back off and retry later
    end
```
