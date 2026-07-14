Feature: Therapy summary display
  As the DreamStation 2 firmware
  I want to show therapy hours, AHI and mask fit
  So that therapy is safe and effective.

  @tests:touchscreen-app @tests:RESP-10 @id:test-touchscreen-app
  Scenario: Therapy summary display
    Given a completed therapy session exists
    When the summary screen renders
    Then therapy hours, AHI and mask fit are displayed
