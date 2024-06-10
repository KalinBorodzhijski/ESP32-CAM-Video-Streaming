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

### Steps

1. **Clone the repository:**

   ```bash
   git clone https://github.com/KalinBorodzhijski/ESP32-CAM-Video-Streaming.git
   cd ESP32-CAM-Video-Streaming
   ```

2. **Install required libraries:**

   On Debian-based systems (e.g., Ubuntu):
   
   ```bash
   sudo apt-get update
   sudo apt-get install libmicrohttpd-dev libssl-dev
   ```

   On Red Hat-based systems (e.g., Fedora):
   
   ```bash
   sudo dnf install libmicrohttpd-devel openssl-devel
   ```

3. **Compile the project:**

   ```bash
   gcc -o webserver main.c -lmicrohttpd -lssl -lcrypto
   ```

## Usage

1. **Run the web server:**

   ```bash
   ./webserver
   ```

2. **Access the website:**

   Open a web browser and navigate to `https://localhost:8000`.

3. **Log in:**

   Use the provided login credentials to access the camera stream.

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

If you encounter any issues, please create a new issue in the [Issues](https://github.com/yourusername/yourrepository/issues) section.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

**Author:** Kalin Borodzhijski
**Contact:** kalin15342@gmail.com