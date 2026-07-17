---
itemId: humidity-control
itemType: Software Item Spec
itemTitle: Humidifier Control
itemFulfills: RESP-6
---

# Humidifier Control

Drives the heater plate to hold the patient-set humidity setpoint.

Implemented in `humidifier/humidity_control.c`. See the module's unit tests under `tests/`.

## Architecture

```mermaid
flowchart LR
    SET[Patient Humidity Setpoint] --> HC[Humidifier Control]
    RH[Humidity / Ambient Sensor] --> HC
    HC --> PWM[Heater Plate PWM]
    PWM --> PLATE[Heater Plate]
    PLATE --> AIR[Humidified Airflow]
```

## Control loop

```mermaid
flowchart TD
    S[Read setpoint + measured humidity] --> E[Compute error]
    E --> C{Below setpoint?}
    C -->|yes| UP[Increase heater duty cycle]
    C -->|no| DN[Decrease heater duty cycle]
    UP --> APPLY[Apply PWM to heater plate]
    DN --> APPLY
    APPLY --> S
```
