# Door Access Control System 
This is my bachelor project 1

## Overview
Door Access Control System  is an embedded system designed for access control within a designated area. This project uses the STM32 microcontroller to process input parameters, transmits data to the ESP32 via UART, and utilizes Google Sheets for cloud data storage.

## Features
- **STM32 Microcontroller**: Processing input parameters and verifying their validity
- **ESP32 Microcontroller**: Receiving data and uploading it to Google Sheets
- **Google Sheets API**: Acts as an intermediary to automate data transmission and storage

## Technologies Used
- **Microcontroller**: STM32F103C8T6, ESP-Wroom-32 (C/C++)
- **API**: Google Sheets API
- 
## Usage
- Deploy the system in areas requiring access control.
- Monitor entry and exit information of authorized individuals.

## Future Enhancements
- Adding verification methods such as facial recognition and fingerprint scanning.
- Adding environmental sensors.
- Enhancing real-time processing.
