#!/usr/bin/env bash

# Usage: ./run-tests.sh
# Requirements:
# - install the GNU Guix package manager
# - add the Guix channel at: https://github.com/jakeforster/guix-channel

# Base directory of the script
BASE_DIR=$(pwd)

# Directory for log files
LOG_DIR="$BASE_DIR/logs"
mkdir -p "$LOG_DIR"

# Geant4 versions to test
# Expects -vis version exists for each
G4_VERSIONS=(geant4@11.1.1 geant4@11.1.2 geant4@11.1.3 geant4@11.2.0)

# Examples to test
# expects:
# - dir named <APP>
# - main file example<APP>.cc
# - batch mode macro file run1.mac
APPS=(A1 A2)

# Function to check and report test result
check_and_report() {
    local log_file="$1"
    local test_name="$2"
    if grep -q "G4Exception" "$log_file" || grep -q "Aborted" "$log_file"; then
        echo "Test failed for $test_name, see log: $log_file"
    else
        echo "Test passed for $test_name"
    fi
}

# Tests for packages without -vis
for G4V in "${G4_VERSIONS[@]}"; do
    for APP in "${APPS[@]}"; do
	echo "Starting test for ${APP} on ${G4V}..."
	EXECUTABLE_NAME="example$APP"
        LOG_FILE="$LOG_DIR/log_${G4V}_${APP}.txt"
        guix shell -C "$G4V" cmake make gcc-toolchain coreutils bash -- bash -c "
        cd \"$BASE_DIR/$APP\" &&
        { [ ! -d 'build' ] || rm -rf build; } &&
        mkdir build &&
        cd build &&
        cmake .. &&
        make &&
        ./$EXECUTABLE_NAME run1.mac > \"$LOG_FILE\" 2>&1 &&
        cd \"$BASE_DIR\" &&
        rm -rf \"$BASE_DIR/$APP/build\""
        check_and_report "$LOG_FILE" "$G4V - $APP"
    done
done

# Append -vis to each element in $G4_VERSIONS array
G4_VIS_VERSIONS=("${G4_VERSIONS[@]}")
for i in "${!G4_VIS_VERSIONS[@]}"; do
    G4_VIS_VERSIONS[$i]=${G4_VIS_VERSIONS[$i]/geant4/geant4-vis}
done

# Tests for -vis packages
# NB tests are ran in batch mode not interactive mode
# NB extra mesa package required
for G4V in "${G4_VIS_VERSIONS[@]}"; do
    for APP in "${APPS[@]}"; do
	echo "Starting test for ${APP} on ${G4V}..."
	EXECUTABLE_NAME="example$APP"
        LOG_FILE="$LOG_DIR/log_${G4V}_${APP}.txt"
        guix shell -C "$G4V" cmake make gcc-toolchain mesa coreutils bash -- bash -c "
        cd \"$BASE_DIR/$APP\" &&
        { [ ! -d 'build' ] || rm -rf build; } &&
        mkdir build &&
        cd build &&
        cmake .. &&
        make &&
        ./$EXECUTABLE_NAME run1.mac > \"$LOG_FILE\" 2>&1 &&
        cd \"$BASE_DIR\" &&
        rm -rf \"$BASE_DIR/$APP/build\""
        check_and_report "$LOG_FILE" "$G4V - $APP"
    done
done

