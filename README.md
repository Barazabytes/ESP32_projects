# Milestone Projects

A personal collection of milestone embedded systems projects built with **ESP-IDF** and **FreeRTOS**.

---

## Projects

### Sample — Button-Driven LED Controller

An event-driven embedded application for the ESP32 that uses a modular architecture to detect button presses and control an LED accordingly.

**What it does:**
- A short button press toggles the LED on/off
- Holding the button triggers a rapid 5-blink sequence
- Button and LED logic are fully decoupled via an event bus

**Architecture:**

```
main.c
├── clickey (button module)   → detects press / hold events
├── blinkey (LED module)      → reacts to events, drives the LED
└── event_bus (core)          → relays events between modules
```

**Component breakdown:**

| Component | Role |
|---|---|
| `app_hal` | GPIO hardware abstraction layer (wraps ESP-IDF `driver/gpio`) |
| `BUTTON` | Raw button state reader, built on `app_hal` |
| `LED` | LED driver (on / off / toggle), built on `app_hal` |
| `clickey` | Detects PRESSED / HELD events from a button; posts to event bus |
| `blinkey` | Consumes events from bus; drives LED behavior |
| `core/event_bus` | FreeRTOS queue-based pub/sub relay between modules |

**Hardware:**
- Button on `GPIO_NUM_0`
- LED on `GPIO_NUM_2`

---

## Stack

- **Platform:** ESP32
- **Framework:** ESP-IDF
- **RTOS:** FreeRTOS
- **Language:** C

---

## Building

```bash
idf.py build
idf.py -p <PORT> flash monitor
```
