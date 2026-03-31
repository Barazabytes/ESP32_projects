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

# WebPages — ESP32 Wi-Fi Web Server

An ESP-IDF project that turns an ESP32 into a standalone web server, serving a multi-page authentication UI (Register + Sign In) directly from on-chip flash storage over Wi-Fi.

---

## Features

- **Wi-Fi Access Point mode** — the ESP32 creates its own network; no router required
- **Wi-Fi Station mode** — alternative mode to join an existing network
- **HTTP web server** — built on `esp_http_server`, serves HTML and CSS from SPIFFS
- **SPIFFS file system** — web pages are flashed into a dedicated storage partition and served at runtime
- **Stylised web UI** — dark-themed Register and Sign In pages with password strength meter, inline validation, and animated transitions

---

## Project Structure

```
WebPages/
├── CMakeLists.txt                          # Top-level CMake (project: webExt)
├── partitions.csv                          # Custom partition table
├── main/
│   ├── CMakeLists.txt                      # Registers main component; creates SPIFFS image
│   └── main.c                              # Entry point — starts Wi-Fi AP + web server
└── components/
    └── services/
        ├── CMakeLists.txt                  # Aggregated services component
        ├── WIFI/
        │   ├── CMakeLists.txt
        │   ├── include/
        │   │   └── wifi_service.h          # Wi-Fi service API
        │   ├── wifi_service_access_point.c # AP mode implementation
        │   └── wifi_service_station.c      # Station mode implementation
        └── HTTP/
            ├── CMakeLists.txt              # HTTP component; creates SPIFFS image
            ├── include/
            │   └── http_server.h           # Web server API
            ├── http_server.c               # URI handlers + server init
            └── pages/                      # Files flashed into SPIFFS
                ├── index.html              # Register page
                ├── signin.html             # Sign In page
                └── style.css              # Shared stylesheet
```

---

## Partition Table

| Name      | Type | SubType | Offset     | Size    |
|-----------|------|---------|------------|---------|
| nvs       | data | nvs     | 0x9000     | 24 KB   |
| phy_init  | data | phy     | 0xF000     | 4 KB    |
| factory   | app  | factory | 0x10000    | 2 MB    |
| storage   | data | spiffs  | 0x210000   | 1 MB    |

The `storage` partition holds the SPIFFS image containing the web pages.

---

## Getting Started

### Prerequisites

- [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/) v5.x
- An ESP32 development board

### Build & Flash

```bash
# Set up the IDF environment
. $IDF_PATH/export.sh

# Configure (optional — set Wi-Fi credentials, flash size, etc.)
idf.py menuconfig

# Build, flash, and monitor
idf.py build flash monitor
```

The build system automatically packages the files under `components/services/HTTP/pages/` into a SPIFFS image and flashes it to the `storage` partition.

### Wi-Fi Credentials

Edit `main/main.c` to set the access point SSID and password before flashing:

```c
char *ssid     = "ByteLink_6G";
char *password = "miminawewe";
```

---

## How It Works

1. `app_main` calls `wifi_service_init_ap()`, which brings up a soft AP on channel 6 with WPA2-PSK and a maximum of 5 connected stations.
2. `start_web_server()` starts the HTTP server and registers four URI handlers:

| URI           | Handler               | Description              |
|---------------|-----------------------|--------------------------|
| `/`           | `root_uri_function`   | Serves `index.html`      |
| `/index.html` | `root_uri_function`   | Serves `index.html`      |
| `/signin.html`| `signin_uri_function` | Serves `signin.html`     |
| `/style.css`  | `styles_handler`      | Serves `style.css`       |

3. Each handler mounts the SPIFFS partition at `/storage`, opens the requested file, and streams it to the client in 1 KB chunks.

To switch to Station mode (joining an existing network instead of creating one), replace the `wifi_service_init_ap` call in `main.c` with:

```c
wifi_service_init_sta(ssid, password);
```

---

## Web UI

Both pages share a single `style.css` and feature:

- Dark aesthetic with animated ambient background blobs
- Google Fonts (`Playfair Display` + `DM Sans`)
- SVG icons embedded inline
- Password visibility toggle
- **Register page** — live password strength meter (Weak / Fair / Good / Strong)
- **Sign In page** — inline field validation and loading spinner on submit

Connect a device to the ESP32's Wi-Fi network and open `http://192.168.4.1` in a browser.

---

## Authors

- **Said Baraza** — Wi-Fi service, HTTP server, web UI






