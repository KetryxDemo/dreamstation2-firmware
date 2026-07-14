Feature: Endpoint certificate pinning
  As the DreamStation 2 firmware
  I want to reject an unpinned certificate
  So that therapy is safe and effective.

  @tests:cert-pinning @tests:RESP-9 @id:test-cert-pinning
  Scenario: Endpoint certificate pinning
    Given the cloud endpoint presents an unexpected certificate
    When the pinning check runs
    Then the upload is rejected
