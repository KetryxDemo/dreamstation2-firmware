Feature: Humidity setpoint control
  As the DreamStation 2 firmware
  I want to hold the humidity setpoint
  So that therapy is safe and effective.

  @tests:humidity-control @tests:RESP-6 @id:test-humidity-control
  Scenario: Humidity setpoint control
    Given the target humidity is set
    When the heater plate control runs
    Then measured humidity converges to the setpoint
