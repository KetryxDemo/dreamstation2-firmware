Feature: Pressure sensor sampling rate
  As the DreamStation 2 firmware
  I want to sample the airway pressure sensor at 100 Hz
  So that therapy is safe and effective.

  @tests:pressure-sensor @tests:RESP-1 @id:test-pressure-sensor
  Scenario: Pressure sensor sampling rate
    Given the therapy loop is running
    When I measure the pressure sampling rate
    Then it should be at least 100 Hz
