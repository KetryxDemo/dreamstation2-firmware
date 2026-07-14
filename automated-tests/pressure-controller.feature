Feature: Auto pressure adjustment
  As the DreamStation 2 firmware
  I want to raise pressure on detected events
  So that therapy is safe and effective.

  @tests:pressure-controller @tests:RESP-4 @id:test-pressure-controller
  Scenario: Auto pressure adjustment
    Given a respiratory event is detected
    When the controller adjusts therapy
    Then pressure increases by 1 cmH2O up to the configured maximum
