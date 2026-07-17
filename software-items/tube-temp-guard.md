---
itemId: tube-temp-guard
itemType: Software Item Spec
itemTitle: Heated-Tube Temperature Guard
itemFulfills: RESP-7
---

# Heated-Tube Temperature Guard

Disables the heated tube when measured tube temperature exceeds 43 C.

Implemented in `humidifier/tube_temp_guard.c`. See the module's unit tests under `tests/`.

## Architecture

```mermaid
flowchart LR
    TS[Tube Temperature Sensor] --> TG[Heated-Tube Temperature Guard]
    TG --> SW[Tube Heater Switch]
    SW --> TUBE[Heated Tube]
    TG -. safety override .-> HC[Humidifier Control]
```

## Over-temperature flow

```mermaid
flowchart TD
    S[Sample tube temperature] --> C{Temp > 43 C?}
    C -->|yes| OFF[Disable heated tube]
    C -->|no| ON[Allow normal heating]
    OFF --> A[Raise over-temperature alarm]
    ON --> S
    A --> S
```
