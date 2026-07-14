# DreamStation 2 firmware - test build
#
# Builds and runs the unit tests. Host-side build with a C99 compiler;
# no external test framework required.

CC      ?= cc
CFLAGS  ?= -std=c99 -Wall -Wextra -Itherapy_control -Ihumidifier -Iconnectivity -Iui
BIN     := build

# test name -> module sources it links against
TESTS := test_apnea_detector test_pressure_clamp test_tube_temp_guard test_cloud_uploader

test_apnea_detector_SRC  := therapy_control/apnea_detector.c
test_pressure_clamp_SRC  := therapy_control/pressure_clamp.c
test_tube_temp_guard_SRC := humidifier/tube_temp_guard.c
test_cloud_uploader_SRC  := connectivity/cloud_uploader.c connectivity/cert_pinning.c

.PHONY: test clean
test: $(TESTS)

$(TESTS):
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) tests/$@.c $($@_SRC) -o $(BIN)/$@
	@echo "== running $@ =="
	@$(BIN)/$@

clean:
	rm -rf $(BIN)
