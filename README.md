# KP_TelU_Telkomsat_Bogor

## Karya: Rio Novlensra & Gery Arland Tangibali - Telkom University & Abdurasyid Nur Syarif - Universitas Gunadarma Published in 2023

## Agenda Pembelajaran Kerja Praktik Telkomsat Bogor (supervisi remote/online from Surabaya)

### Objective/Goals/Projects : Perancangan IoT menggunakan ESP32+DHT22+LED dengan bermacam konsep teknologi dibaliknya.

### Daftar Isi & Pemaparan singkat:
- Topic 1: Getting Started Microcontroller ESP32
  - Skenario dirancang dengan menggunakan 2 module: Module 1: firmware default, Module 2: firmware custom (micropython boot)
    - Task-1: Testing board:
      [Module 1. Program WiFi Scan (.ino)](https://github.com/muhammadandykmaulana/KP_TelU_Telkomsat_Bogor/blob/main/GettingStartedESP32/Test-HW-Firmware-Default/WiFiScan/WiFiScan.ino)
      [Module 2. Program Hello World (Micropython).](https://github.com/muhammadandykmaulana/KP_TelU_Telkomsat_Bogor/blob/main/GettingStartedESP32/Test-HW-Firmware-Custom/hello_world.py)
- Topic 2: Perancangan Elektronika Dasar: Sensor DHT22+ESP32.
  - Skenario menggunakan sensor DHT22 dengan kontroller ESP32.
    - Task-1: Output sensor DHT Serial Monitor dengan format `Temperature: 27.50°C | Humidity: 55.20% | Heat Index: 28.32°C`
    - Task-2: Eksplorasi library DHT dan Adafruit Unified.
- Topic-3: MQTT Two Way Communication :
    - Basic & preparation:
      - Assign Static IP on ESP32. Use tools: IP Scanner, ping & Mac Vendors.
      - Install Mosquitto broker, config mosquitto.conf add newline `listener 1883` & `allow_anonymous true`.
      - Install library, Setup/Authentication ke Mosquitto (broker).
    - Prepare DHT22 JSON, example: `{"sensor":"DHT11","temperature":"31.30","humidity":"37.00","heatIndex":"30.87"}`.
    - Task-1: MQTT-Node-Red.
    - Task-2: MQTT-Web Server 2-Way Communication with Ngrok as port forwarding.

### Resume keseluruhan:
Pembelajaran yang didapat:
- Belajar IoT from stratch dengan framework IoT model reference Cisco 2014
- Review Software Architecture (JWT, API, HTTP, MQTT)
- Konsep port forwarding
- IoT untuk support Standarisasi Datacenter ISO 27001 dan appliances lainnya di perusahaan Telekomunikasi berbasis satelit.
- Belajar PubSub MQTT dengan mengirim data dalam bentuk JSON dan bisa Two Way Communication.

### Insight: Memahami macam-macam beserta cara implementasi IoT berdasarkan referensi Cisco White Paper 2014 pada industri Telekomunikasi berbasis satelit.

### Core Value/pondasi:
- Inisiasi: explore project from stracth.
- Ideasi: explore project improvement concept.

### Kendala Umum:
- Com Port tidak muncul di Device Manager. Solusi: Tidak support untuk USB 2 wired (karena cuman buat power/charging), ganti  ke USB 3 wire (kabel data). [Driver](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
- Output serial monitor sesuai dengan baudrate.
- Pada task 3 alat yang digunakan berbeda dengan referensi yang kita pakai karena adanya keterbatasan alat yang kita punya.
     
### Referensi:
- Topic 1: Getting Started Microcontroller ESP32. [Module 1. Default firmware](https://randomnerdtutorials.com/getting-started-with-esp32/) [Module 2. Flash firmware Micropython](https://randomnerdtutorials.com/getting-started-thonny-micropython-python-ide-esp32-esp8266/)
- Topic 2: Perancangan Elektronika Dasar: Sensor DHT22+ESP32. [Module 1.Default firmware](https://randomnerdtutorials.com/esp32-dht11-dht22-temperature-humidity-sensor-arduino-ide/) [Module 2. Micropython Firmware-GUI](https://randomnerdtutorials.com/flash-upload-micropython-firmware-esp32-esp8266/)[ & Micropython Firmware-CLI](https://randomnerdtutorials.com/flashing-micropython-firmware-esptool-py-esp32-esp8266/)
- [Contoh penulisan README.md](https://raw.githubusercontent.com/gungunfebrianza/Belajar-Dengan-Jenius-Python/main/README.md)
- [Laporan PPT](https://docs.google.com/presentation/d/1NmUllhlAY6Tukser3Csq9SfjNbiNwvG42WOzhxu5ojU/edit?usp=sharing)
- [Dokumentasi](https://github.com/geryarland/dokumentasi_KerjaPraktik.git)
- Topic 3: (https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/)
