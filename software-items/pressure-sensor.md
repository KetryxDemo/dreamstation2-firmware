---
itemId: pressure-sensor
itemType: Software Item Spec
itemTitle: Pressure Sensor Driver
itemFulfills: RESP-1
---

# Pressure Sensor Driver

Samples the airway pressure transducer at 100 Hz and exposes calibrated readings to the therapy loop.

Implemented in `therapy_control/pressure_sensor.c`. See the module's unit tests under `tests/`.

## Architecture

```mermaid
flowchart LR
    XDCR[Airway Pressure Transducer] --> ADC[ADC Channel]
    ADC --> DRV[Pressure Sensor Driver<br/>100 Hz sample + calibration]
    DRV --> READ[Calibrated Pressure Reading cmH2O]
    READ --> APN[Apnea / Hypopnea Detector]
    READ --> CTL[Auto Pressure Controller]
```

## Sampling sequence

```mermaid
sequenceDiagram
    participant T as 100 Hz Timer
    participant D as Pressure Sensor Driver
    participant A as ADC
    participant L as Therapy Loop
    T->>D: tick (every 10 ms)
    D->>A: read raw counts
    A-->>D: raw ADC value
    D->>D: apply calibration (offset, gain)
    D->>L: publish calibrated pressure (cmH2O)
```
