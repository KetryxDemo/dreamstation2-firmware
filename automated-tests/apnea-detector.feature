@tests:apnea-detector @tests:RESP-2 @tests:RESP-3 @id:test-apnea-detector
Feature: Apnea and hypopnea detection
  As the DreamStation 2 firmware
  I want to classify respiratory events from airflow
  So that therapy is safe and effective.

  Scenario: Apnea and hypopnea detection
    Given airflow drops below 10% of baseline for 10 seconds
    When the detector evaluates the signal
    Then an apnea event should be flagged
