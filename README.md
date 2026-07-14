# DreamStation 2 Auto CPAP - Device Firmware

Embedded C firmware for the DreamStation 2 Auto CPAP sleep-therapy device.

This is a realistic-but-fictional demo codebase used to show Ketryx
code-to-requirement traceability. It is NOT production software.

## Device overview

- Auto-CPAP pressure therapy, 4-20 cmH2O, with breath-sensing auto-titration
- Integrated humidifier with a heated tube
- Bluetooth and cellular cloud upload of nightly session data
- Color touchscreen UI

Safety-critical behavior: commanded therapy pressure must never exceed
20 cmH2O, and the heated tube must shut off above 43 C.

## Module map

Each source file below implements one software requirement (SRS). Commit
messages are prefixed with the SRS key so the trace tooling can link code
to requirements.

| File                                    | Requirement |
|-----------------------------------------|-------------|
| therapy_control/pressure_sensor.c       | SRS-1  Sample pressure sensor at >= 100 Hz |
| therapy_control/apnea_detector.c        | SRS-2  Detect apnea (airflow < 10% baseline for >= 10 s) |
| therapy_control/apnea_detector.c        | SRS-3  Detect hypopnea (30-90% baseline for >= 10 s) |
| therapy_control/pressure_controller.c   | SRS-4  Raise pressure in 1 cmH2O steps up to configured max |
| therapy_control/pressure_clamp.c        | SRS-5  Clamp commanded pressure to [min,max], never exceed 20 cmH2O |
| humidifier/humidity_control.c           | SRS-6  Control heater plate to humidity setpoint |
| humidifier/tube_temp_guard.c            | SRS-7  Disable heated tube if tube temp > 43 C |
| connectivity/cloud_uploader.c           | SRS-8  Upload session data over TLS 1.2+ |
| connectivity/cert_pinning.c             | SRS-9  Certificate-pin the cloud endpoint |
| ui/touchscreen_app.c                    | SRS-10 Display therapy hours, AHI, mask fit |

## Test map

| File                             | Test case |
|----------------------------------|-----------|
| tests/test_apnea_detector.c      | TC-2  Bench-sim apnea detection (verifies SRS-2) |
| tests/test_pressure_clamp.c      | TC-5  Clamp never commands > 20 cmH2O (verifies SRS-5) |
| tests/test_tube_temp_guard.c     | TC-7  Tube cutoff at 43 C (verifies SRS-7) |
| tests/test_cloud_uploader.c      | TC-8  TLS + cert-pin negative test (verifies SRS-8/9) |

## Building the tests

```
make test
```

This compiles each unit test against its module and runs it. All tests
use a tiny assert-based harness (no external framework) so the build has
no dependencies beyond a C99 compiler.
