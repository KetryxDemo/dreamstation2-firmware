@tests:cloud-uploader @tests:RESP-8 @id:test-cloud-uploader
Feature: Encrypted session upload
  As the DreamStation 2 firmware
  I want to upload session data over TLS
  So that therapy is safe and effective.

  Scenario: Encrypted session upload
    Given a nightly session is ready
    When the uploader connects to the cloud
    Then the connection uses TLS 1.2 or higher
