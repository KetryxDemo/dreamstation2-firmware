---
itemId: touchscreen-app
itemType: Software Item Spec
itemTitle: Therapy Summary UI
itemFulfills: RESP-10
---

# Therapy Summary UI

Renders therapy hours, AHI, and mask-fit status on the color touchscreen.

Implemented in `ui/touchscreen_app.c`. See the module's unit tests under `tests/`.

## Architecture

```mermaid
flowchart LR
    CTL[Therapy Loop] --> STATS[Session Statistics]
    DET[Apnea / Hypopnea Detector] --> STATS
    MASK[Mask-Fit / Leak Estimator] --> STATS
    STATS --> UI[Therapy Summary UI]
    UI --> SCREEN[Color Touchscreen]
```

## Render sequence

```mermaid
sequenceDiagram
    participant T as Therapy Loop
    participant M as Metrics Aggregator
    participant U as Therapy Summary UI
    participant S as Touchscreen
    T->>M: therapy hours, events, leak data
    M->>M: compute AHI + mask-fit status
    M->>U: summary metrics
    U->>S: render therapy hours, AHI, mask fit
    Note over U,S: refreshed on wake / each morning
```
