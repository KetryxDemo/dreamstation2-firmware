---
itemId: pressure-clamp
itemType: Software Item Spec
itemTitle: Pressure Clamp (Safety)
itemFulfills: RESP-5
---

# Pressure Clamp (Safety)

Hard-limits commanded pressure to the configured range and never exceeds 20 cmH2O.

Implemented in `therapy_control/pressure_clamp.c`. See the module's unit tests under `tests/`.

## Architecture

```mermaid
flowchart LR
    CTL[Auto Pressure Controller] --> REQ[Requested Pressure]
    REQ --> CLMP[Pressure Clamp Safety]
    CFG[Configured Min/Max] --> CLMP
    LIM[Absolute Limit 20 cmH2O] --> CLMP
    CLMP --> CMD[Commanded Pressure]
    CMD --> BLO[Blower / Motor]
```

## Clamp flow

```mermaid
flowchart TD
    R[Requested pressure] --> C1{Below configured min?}
    C1 -->|yes| MIN[Set to min]
    C1 -->|no| C2{Above configured max?}
    C2 -->|yes| MAX[Set to max]
    C2 -->|no| PASS[Keep requested]
    MIN --> HARD{Exceeds 20 cmH2O?}
    MAX --> HARD
    PASS --> HARD
    HARD -->|yes| CAP[Cap at 20 cmH2O]
    HARD -->|no| OK[Accept value]
    CAP --> OUT[Command to blower]
    OK --> OUT
```
