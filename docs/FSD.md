# ESP32-C3 SOS Blinker + WiFi Manager + OTA

## Functional Specification Document (FSD)

---

## Document Information

| Field        | Value                |
| ------------ | -------------------- |
| Version      | 1.0                  |
| Status       | Draft                |
| Framework    | Arduino (PlatformIO) |
| Target MCU   | ESP32-C3             |
| Connectivity | WiFi                 |
| OTA          | ArduinoOTA           |

---

# 1. Overview

## 1.1 Purpose

Firmware shall implement:

1. SOS Morse blinking using built-in LED
2. Serial monitor output of "SOS" per cycle
3. WiFi auto-connect with retry mechanism
4. OTA firmware update over WiFi

System is designed modular but simple, suitable for coursework and demonstration purposes.

---

## 1.2 Design Philosophy

* Non-blocking LED timing using `millis()`
* WiFi handled asynchronously
* OTA handled independently
* LED logic must NOT depend on WiFi state

Engineering rule:

If WiFi fails, SOS must continue blinking.

---

# 2. System Context

## 2.1 High-Level Context Diagram

```
                ┌──────────────────────┐
                │        User          │
                │  (Serial Monitor)    │
                └──────────┬───────────┘
                           │ USB UART
                           ▼
                ┌──────────────────────┐
                │     ESP32-C3         │
                │  Arduino Framework   │
                │                      │
                │  ┌────────────────┐  │
                │  │  SOS Engine    │  │
                │  └────────────────┘  │
                │                      │
                │  ┌────────────────┐  │
                │  │ WiFi Manager   │  │
                │  └────────────────┘  │
                │                      │
                │  ┌────────────────┐  │
                │  │ ArduinoOTA     │  │
                │  └────────────────┘  │
                └──────────┬───────────┘
                           │
                           ▼
                     Built-in LED

                           │
                           ▼
                     WiFi Router
                           │
                           ▼
                       OTA Client (PC)
```

---

## 2.2 Context Explanation

### User

Interacts through:

* Serial Monitor for debugging and status output
* OTA upload from computer over WiFi network

---

### ESP32-C3 System Boundary

The firmware consists of three logical subsystems:

1. **SOS Engine**

   * Controls built-in LED
   * Generates Morse timing
   * Prints "SOS" at each cycle start

2. **WiFi Manager**

   * Handles connection lifecycle
   * Monitors connection state
   * Performs reconnection if needed

3. **OTA Module**

   * Waits for OTA request
   * Handles firmware flashing
   * Reboots system after successful update

All subsystems run concurrently inside the Arduino main loop.

---

## 2.3 Data Flow Summary

| Source         | Destination    | Purpose            |
| -------------- | -------------- | ------------------ |
| Serial Monitor | ESP32-C3       | Debug / Monitoring |
| ESP32-C3       | Serial Monitor | Status Output      |
| ESP32-C3       | WiFi Router    | Network connection |
| OTA Client     | ESP32-C3       | Firmware update    |
| ESP32-C3       | Built-in LED   | SOS visual output  |

---

# 3. Hardware Specification

## 3.1 Target Board

ESP32-C3 (any variant supported by PlatformIO)

---

## 3.2 GPIO Assignment

| Function     | GPIO                        |
| ------------ | --------------------------- |
| Built-in LED | LED_BUILTIN (board default) |

Note: GPIO must be verified from board definition in PlatformIO.

---

# 4. Functional Requirements

---

# 4.1 SOS LED Blinking

## 4.1.1 Morse Definition

SOS = `... --- ...`

Timing unit: 200 ms

| Symbol     | Duration    |
| ---------- | ----------- |
| Dot        | 1 unit ON   |
| Dash       | 3 units ON  |
| Symbol gap | 1 unit OFF  |
| Letter gap | 3 units OFF |
| Cycle gap  | 7 units OFF |

---

## 4.1.2 Behavior

LED-001
System SHALL blink continuously in SOS pattern.

LED-002
Blink timing SHALL use `millis()`, not `delay()`.

LED-003
Blink cycle SHALL repeat indefinitely.

LED-004
System SHALL print "SOS" to Serial at start of each full cycle.

---

## 4.1.3 Morse Timing Diagram

```
Dot:   ON 200ms  OFF 200ms
Dash:  ON 600ms  OFF 200ms

Pattern:
... (S)
--- (O)
... (S)
```

---

# 4.2 WiFi Manager

## 4.2.1 Behavior

WIFI-001
System SHALL attempt WiFi connection using predefined SSID and password.

WIFI-002
If connection fails within 15 seconds, system SHALL retry periodically.

WIFI-003
Connection status SHALL be printed to Serial.

---

## 4.2.2 WiFi States

| State           | Description           |
| --------------- | --------------------- |
| WIFI_INIT       | Begin WiFi            |
| WIFI_CONNECTING | Attempting connection |
| WIFI_CONNECTED  | Connected             |
| WIFI_RETRY      | Retry mode            |

---

# 4.3 OTA Update

## 4.3.1 OTA Framework

OTA SHALL use ArduinoOTA library.

---

## 4.3.2 OTA Requirements

OTA-001
OTA shall only operate when WiFi is connected.

OTA-002
OTA handler shall run inside `loop()`.

OTA-003
During OTA update, LED blinking may pause.

OTA-004
System SHALL reboot automatically after successful OTA.

---

## 4.3.3 OTA Flow

```
WiFi Connected
      │
Start ArduinoOTA
      │
Wait for update request
      │
Receive firmware
      │
Flash
      │
Reboot
```

---

# 5. Firmware Architecture

---

## 5.1 Project Structure

```
esp32c3-sos/
│
├── platformio.ini
├── src/
│   ├── main.cpp
│   ├── sos.cpp
│   ├── sos.h
│   ├── wifi_manager.cpp
│   ├── wifi_manager.h
│
└── docs/
    └── FSD.md
```

---

## 5.2 Module Responsibilities

### main.cpp

* Setup serial
* Initialize WiFi
* Initialize OTA
* Call SOS update function
* Call `ArduinoOTA.handle()`

---

### sos.cpp

* Implements Morse state machine
* Uses `millis()`
* Controls LED
* Triggers serial print per cycle

---

### wifi_manager.cpp

* Handles `WiFi.begin()`
* Manages connection state
* Provides `isConnected()` function

---

# 6. State Model

Global runtime behavior:

```
BOOT
  │
  ▼
INIT_WIFI
  │
  ▼
RUNNING
  │
  ├── SOS blinking (always active)
  ├── WiFi monitor
  └── OTA handler
```

No deep sleep required.
No interrupt-driven wake logic required.

---

# 7. Non-Functional Requirements

| Metric              | Target                 |
| ------------------- | ---------------------- |
| Boot time           | < 5 seconds            |
| WiFi connect        | < 15 seconds           |
| LED timing accuracy | ±10 ms                 |
| OTA reliability     | No firmware corruption |

---

# 8. Constraints

* `delay()` SHALL NOT be used for Morse timing.
* WiFi reconnection SHALL NOT block LED blinking.
* OTA handler SHALL be called in every loop iteration.
* Serial baud rate: 115200.

---

# 9. Example platformio.ini

```
[env:esp32c3]
platform = espressif32
board = esp32-c3-devkitm-1
framework = arduino
monitor_speed = 115200
```

Board name MUST match actual hardware.

---
