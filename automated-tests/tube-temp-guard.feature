Feature: Heated tube over-temperature cutoff
  As the DreamStation 2 firmware
  I want to disable the tube above 43 C
  So that therapy is safe and effective.

  @tests:tube-temp-guard @tests:RESP-7 @id:test-tube-temp-guard
  Scenario: Heated tube over-temperature cutoff
    Given the tube temperature reaches 44 C
    When the guard evaluates temperature
    Then the heated tube is disabled
