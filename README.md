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


---
 
### webServer — ESP32 Wi-Fi Web Server
 
An embedded web server running on the ESP32 that serves an HTML page over Wi-Fi. Supports both Access Point (AP) and Station (STA) modes, with static file serving via SPIFFS.
 
**What it does:**
- Boots the ESP32 in AP or STA Wi-Fi mode
- Serves an HTML page stored on the SPIFFS partition over HTTP
- Reads the file from flash on each request and streams it to the client in chunks
 
**Architecture:**
 
```
main.c
├── wifi_service (WIFI)   → initializes Wi-Fi in AP or STA mode
└── http_server (HTTP)    → registers URI handlers and serves files from SPIFFS
```
 
**Component breakdown:**
 
| Component | Role |
|---|---|
| `wifi_service_access_point.c` | Configures and starts the ESP32 as a Wi-Fi access point |
| `wifi_service_station.c` | Configures and starts the ESP32 as a Wi-Fi station with retry logic |
| `http_server.c` | Starts the HTTP server, mounts SPIFFS, and serves `index.html` on `GET /` |
 
**Partition layout:**
 
| Name | Type | Size |
|---|---|---|
| `nvs` | data/nvs | 24 KB |
| `phy_init` | data/phy | 4 KB |
| `factory` | app/factory | 2 MB |
| `storage` | data/spiffs | 1 MB |
 
**Hardware / Config:**
- Target: ESP32
- Flash: 4 MB
- Default Wi-Fi mode: Access Point (`ByteLink_6G`)
- HTTP server on port 80 (default)
- SPIFFS mounted at `/storage`
 
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
