# ⚡ ESP32-C3 SOS Blinker + WiFi + OTA

![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP32-orange)
![Framework](https://img.shields.io/badge/Framework-Arduino-blue)
![OTA](https://img.shields.io/badge/OTA-ArduinoOTA-green)
![Status](https://img.shields.io/badge/Status-Active-success)

---

## 📌 Project Overview

Embedded system built on **ESP32-C3** that implements:

* 🔴 SOS Morse LED Blinking (non-blocking)
* 📟 Serial Monitor output ("SOS")
* 📶 WiFi Auto-Connect with Retry
* 🔄 OTA Firmware Update (ArduinoOTA)

This project demonstrates:

* Modular firmware design
* State-machine based LED control
* Asynchronous WiFi handling
* Concurrent OTA service

---

## 🏗️ System Architecture

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

## ✨ Features

| Feature         | Description                          |
| --------------- | ------------------------------------ |
| 🔴 SOS Engine   | Morse `... --- ...` blinking         |
| ⏱ Non-Blocking  | Uses `millis()` instead of `delay()` |
| 📶 WiFi Manager | Auto-connect + retry logic           |
| 🔄 OTA Update   | Wireless firmware flashing           |
| 📟 Serial Log   | Status monitoring at 115200 baud     |

---

## 📖 Documentation

Full system specification available here:

➡ **[📄 Functional Specification Document (FSD)](docs/FSD.md)**

---

## 🗂️ Project Structure

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

## 🔧 Hardware

### Supported Board

* ESP32-C3 DevKit (PlatformIO compatible)

### GPIO

| Function     | GPIO        |
| ------------ | ----------- |
| Built-in LED | LED_BUILTIN |

---

## 🚀 Getting Started

### 1️⃣ Install PlatformIO

Install VSCode + PlatformIO extension.

---

### 2️⃣ Build

```
pio run
```

---

### 3️⃣ Flash via USB

```
pio run --target upload
```

---

### 4️⃣ Open Serial Monitor

```
pio device monitor
```

Baud rate: **115200**

You should see:

```
System Booting...
WiFi Connecting...
SOS
```

---

## 🔄 OTA Update

After WiFi connects:

1. Make code changes
2. Select network port in PlatformIO
3. Upload

OTA progress will appear in Serial Monitor.

---

## 🧠 Engineering Concepts Demonstrated

* Finite state machine for Morse control
* Non-blocking embedded timing
* Parallel subsystem execution in `loop()`
* Network reconnection strategy
* Safe OTA handling

---

## 📊 Runtime Model

```
BOOT
  │
  ▼
INIT_WIFI
  │
  ▼
RUNNING
  ├── SOS Engine (always active)
  ├── WiFi Monitor
  └── OTA Handler
```

All subsystems execute cooperatively.


## 📜 License

This project is for educational and demonstration purposes.

