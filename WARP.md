# WARP.md

This file provides guidance to WARP (warp.dev) when working with code in this repository.

## Project Overview

ChoppControl_Embarcado is an ESP32-based embedded system project for beer tap control, built using PlatformIO and the Arduino framework. The system implements a state machine pattern to manage different operational slots.

## Build and Development Commands

### Building and Uploading
```pwsh
# Build the project
pio run

# Build and upload to ESP32
pio run --target upload

# Upload and open serial monitor
pio run --target upload && pio device monitor

# Clean build files
pio run --target clean
```

### Serial Monitor
```pwsh
# Open serial monitor (115200 baud)
pio device monitor

# Open serial monitor with specific baud rate
pio device monitor --baud 115200

# Exit monitor with Ctrl+C
```

### Testing
```pwsh
# Run all tests
pio test

# Run specific test environment
pio test --environment esp32dev
```

### Dependency Management
```pwsh
# Install library dependencies
pio pkg install

# Update libraries
pio pkg update

# List installed libraries
pio pkg list
```

## Architecture

### Hardware Target
- **Platform**: ESP32 (Espressif32)
- **Board**: ESP32 Dev Module
- **Framework**: Arduino
- **Serial Baud Rate**: 115200

### Code Structure

The project follows PlatformIO's standard structure:

- **src/main.cpp**: Main application entry point containing the state machine implementation
  - Uses a slot-based state machine (4 states: 0-3)
  - Timer-based slot transitions with microsecond precision
  - Serial debugging output for state transitions

- **include/**: Header files for modular components (currently empty, ready for future expansion)

- **lib/**: Project-specific libraries (custom libraries should be placed here as separate directories)

- **test/**: Unit tests using PlatformIO Test Runner

### State Machine Pattern

The current implementation uses a simple slot-based state machine:
- 4 sequential states (slots 0-3) that cycle continuously
- Timer mechanism using `millis()` for non-blocking delays
- State transitions logged via Serial for debugging

### Key Functions
- `ResetaTimer(int tempo)`: Initializes timing for state transitions
- `AguardaTimer()`: Non-blocking wait implementation using millisecond timing

## Development Guidelines

### When adding new features:
1. Place reusable code in `lib/` as separate library modules
2. Add hardware-specific definitions to header files in `include/`
3. Keep state machine logic modular and well-documented
4. Use Serial output for debugging embedded behavior

### Working with ESP32:
- The project uses Arduino framework abstractions (`millis()`, `Serial`, etc.)
- All Arduino.h functions are available
- Be mindful of ESP32's dual-core architecture when adding advanced features
- WiFi, Bluetooth, and ESP32-specific peripherals can be added as needed

### Code Style:
- Function names use PascalCase (e.g., `ResetaTimer`, `AguardaTimer`)
- Global variables use lowercase
- State management through global `slot` variable
