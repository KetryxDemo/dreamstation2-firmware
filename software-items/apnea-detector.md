---
itemId: apnea-detector
itemType: Software Item Spec
itemTitle: Apnea / Hypopnea Detector
itemFulfills: RESP-2, RESP-3
---

# Apnea / Hypopnea Detector

Classifies apnea and hypopnea events from the airflow signal against a running baseline.

Implemented in `therapy_control/apnea_detector.c`. See the module's unit tests under `tests/`.

## Architecture

```mermaid
flowchart LR
    PS[Pressure Sensor Driver] --> FLOW[Airflow Signal]
    FLOW --> DET[Apnea / Hypopnea Detector]
    BASE[Running Baseline Estimator] --> DET
    DET --> EVT[Event: Apnea / Hypopnea / None]
    EVT --> CTL[Auto Pressure Controller]
```

## Classification flow

```mermaid
flowchart TD
    S[New airflow sample] --> B[Update running baseline]
    B --> C{Airflow vs baseline}
    C -->|< 10% baseline for >= 10 s| A[Classify APNEA]
    C -->|30-90% baseline for >= 10 s| H[Classify HYPOPNEA]
    C -->|>= 90% baseline| N[No event]
    A --> R[Report event to controller]
    H --> R
    N --> R
```
