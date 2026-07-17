---
itemId: pressure-controller
itemType: Software Item Spec
itemTitle: Auto Pressure Controller
itemFulfills: RESP-4
---

# Auto Pressure Controller

Raises delivered pressure in 1 cmH2O steps on detected events, seeking the lowest effective pressure.

Implemented in `therapy_control/pressure_controller.c`. See the module's unit tests under `tests/`.

## Architecture

```mermaid
flowchart LR
    DET[Apnea / Hypopnea Detector] --> CTL[Auto Pressure Controller]
    CFG[Configured Min/Max Pressure] --> CTL
    CTL --> CMD[Commanded Pressure]
    CMD --> CLMP[Pressure Clamp Safety]
    CLMP --> BLO[Blower / Motor]
```

## Titration sequence

```mermaid
sequenceDiagram
    participant DET as Event Detector
    participant CTL as Auto Pressure Controller
    participant CLMP as Pressure Clamp
    DET->>CTL: respiratory event detected
    CTL->>CTL: raise target by 1 cmH2O (<= configured max)
    CTL->>CLMP: command increased pressure
    Note over CTL: no events -> decay toward lowest effective pressure
    CTL->>CLMP: command decreased pressure
```
