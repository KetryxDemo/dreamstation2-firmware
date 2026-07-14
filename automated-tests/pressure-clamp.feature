@tests:pressure-clamp @tests:RESP-5 @id:test-pressure-clamp
Feature: Pressure clamp safety limit
  As the DreamStation 2 firmware
  I want to never command pressure above 20 cmH2O
  So that therapy is safe and effective.

  Scenario: Pressure clamp safety limit
    Given the controller requests 25 cmH2O
    When the clamp is applied
    Then the commanded pressure should be 20 cmH2O
