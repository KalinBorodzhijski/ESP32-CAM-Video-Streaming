# ESP32-CAM-Video-Streaming
 Personal Web Server That Hosts a Site and Allows Connection to a Connected Camera Using C with Authentication


This project involves developing a personal web server using C that hosts a simple website and allows connection to a connected camera with user authentication. The web server supports HTTPS for secure data transmission.

## Features

- Lightweight web server written in C.
- Live video streaming from a connected camera to the hosted website.
- User authentication system to access the camera stream.
- HTTPS support for secure data transmission.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Development](#development)
- [Contributing](#contributing)
- [License](#license)

## Installation

### Prerequisites

- GCC (GNU Compiler Collection)
- libmicrohttpd (for HTTP server)
- OpenSSL (for HTTPS support)
- A connected camera (compatible with your development environment)
- PlatformIO
- ESP-IDF

### Steps

1. **Clone the repository:**

   ```bash
   git clone https://github.com/KalinBorodzhijski/ESP32-CAM-Video-Streaming.git
   cd ESP32-CAM-Video-Streaming
   ```

2. **Install PlatformIO:**

    Follow the instructions on the [PlatformIO installation guide](https://platformio.org/install).


3. **Configure Wi-Fi Credentials:**

    Update the `wifi_config.c` file with your Wi-Fi SSID and Password. This file is located in `src/wifi_config.c`:

    ```c
    static const char *DEFAULT_SSID = "";
    static const char *DEFAULT_PASSWORD = "";
    ```

    Only the first time that you are running the application.

4. **Flash the Main Application:**

    Build and upload the main application environment:

    ```sh
    pio run -e esp32cam -t upload
    ```


## Usage

## Development

### Tasks

The project is divided into the following tasks:

1. **Setup and Initial Configuration**
2. **Develop Web Server**
3. **Implement Live Video Streaming**
4. **User Authentication System**
5. **Implement HTTPS for Secure Data Transmission**
6. **Testing and Debugging**
7. **Deployment and Documentation**

### Issues

If you encounter any issues, please create a new issue in the [Issues](https://github.com/KalinBorodzhijski/ESP32-CAM-Video-Streaming/issues) section.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

**Author:** Kalin Borodzhijski
**Contact:** kalin15342@gmail.com