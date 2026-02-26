# Surveillance Car Components Specification

This document lists all hardware components used in the **SUrveillance Car**, including specifications, purpose, and purchase recommendations. 


## Microcontrollers

### `ESP32 Dev Board`

- **Specifications:**
  - Microcontroller: `ESP32-WROOM-32`
  - Operating Voltage: `3.3V`
  - Flash Memory: `4 MB`
  - Wi-Fi: `802.11 b/g/n`
  - Bluetooth: `BLE 4.2`
  - GPIO Pins: `34 digital I/O`
  - ADC Channels: `18`
  - PWM Channels: `16`

- **Usage:** Main controller for motors, sensors, and web/voice interface.  
- **Purchase Information:** Kindly visit the seller to know the updated price.

### `ESP32 Expansion Board`

- **Specifications:**
  - Provides easy connections for sensors, motors, and peripherals
  - Includes screw terminals, logic level pins, and breakout connectors

- **Usage:** Simplifies wiring for NanoBot setup  
- **Purchase Information:** Kindly visit the seller to know the updated price



## Sensors

### `PIR Sensor (×1)`

- **Specifications:**
  - Operating Voltage: `5V`
  - Detection Range: `~7 meters`
  - Detection Angle: `~120°`
  - Output: Digital `HIGH` when motion detected

- **Usage:** Detects motion in front of NanoBot for follow/obstacle mode  
- **Purchase Information:** Kindly visit the seller to know the updated price

### `IR Sensor (×1)`

- **Specifications:**
  - Operating Voltage: `3.3V – 5V`
  - Output: Digital `HIGH/LOW`
  - Detection Distance: `2–30 cm`

- **Usage:** Rear line/follow detection  
- **Purchase Information:** Kindly visit the seller to know the updated price

### `Ultrasonic Sensor (×3)`

- **Specifications:**
  - Operating Voltage: `5V`
  - Range: `2–400 cm`
  - Frequency: `40 kHz`
  - Output: Digital distance reading

- **Usage:** Front, left, and right obstacle detection  
- **Purchase Information:** Kindly visit the seller to know the updated price



## Motor Driver

### `L298N `

- **Specifications:**
  - Operating Voltage: `5–35V`
  - Output Current: `2A per channel`
  - Control: Direction + PWM speed control
  - Channels: 2 DC motors (can combine for 4 motors)

- **Usage:** Drives 4 DC motors for Surveillance Car movement  
- **Purchase Information:** Kindly visit the seller to know the updated price



## Power Supply

### `Battery Pack & Charger`

- **Specifications:**
  - Battery Type: 3 3.7V IMR18650 Li-ion Batteries(suitable voltage for motors + 
    ESP32)
  - Charger: Compatible with battery pack

- **Usage:** Powers ESP32, motors, and sensors  
- **Purchase Information:** Kindly visit the seller to know the updated price



## Chassis

### `4-Wheel Car Chassis`

- **Specifications:**
  - Material: Acrylic / Plastic
  - Includes 4 wheels and BO motor + motor mounts
  - Space for mounting ESP32, sensors, and battery pack

- **Usage:** Base frame for Surveillance Car  
- **Purchase Information:** Kindly visit the seller to know the updated price


## Display

### `SH1106 128x64 OLED`

- **Specifications:**
  - Interface: `I2C`
  - Resolution: `128x64`
  - Operating Voltage: `3.3–5V`

- **Usage:** Shows animated face and emotions  
- **Purchase Information:** Kindly visit the seller to know the updated price



## Wiring & Connectors

### `Jumper Wires`

- **Specifications:**
  - Type: Male-to-Male, Male-to-Female, Female-to-Female
  - Length: `10–20 cm`

- **Usage:** Connect sensors, motors, and ESP32  
- **Purchase Information:** Kindly visit the seller to know the updated price

### `Breadboard (Half-Size)`

- **Specifications:**
  - Type: Half-size

- **Usage:** Optional for prototyping  
- **Purchase Information:** Kindly visit the seller to know the updated price