# NanoBot Voice & Button Controlled Emotional Robot

This project is a small **emotional robot** that can be controlled in two ways:

1. **Voice Control** – Speak commands like `forward`, `stop`, `happy`, `angry`, etc.
2. **Web/Button Control** – Connect to the ESP32 Wi-Fi network (`192.168.4.1`) and use on-screen buttons.

It uses:

- ESP32 as the main controller
- 4 brushed DC BO motors with H-bridge driver
- 3 ultrasonic sensors (front, left, right)
- 1 PIR sensor (front)
- 1 IR sensor (rear)
- SH1106 128x64 OLED display (animated face)
- 2 servo motors (emotional hand gestures)
- Offline Whisper speech recognition
- Python for voice command handling


## Features

### Dual Control Modes
- Voice control using offline Whisper
- Web interface control via ESP32 Wi-Fi

### Movement Commands
- Forward
- Backward
- Left
- Right
- Stop
- U-Turn

### Emotional Expressions
- Happy
- Sad
- Angry
- Surprised
- Flirty
- Bored
- Neutral

Includes animated eyes, eyebrows, mouth, and matching hand gestures.

### Modes
- Obstacle Mode ON/OFF (uses ultrasonic sensors)
- Follow Mode ON/OFF (uses PIR and IR sensors)

### Optimized Voice System
- 2-second max recording
- Early stop when speech ends
- Asynchronous command sending
- Fully offline recognition


## Button Control (Web Interface)

1. Connect to the ESP32 Wi-Fi.
2. Open your browser.
3. Go to:

http://192.168.4.1

You will see buttons for:

Movement:
- Forward, Backward, Left, Right, Stop

Emotions:
- Happy, Sad, Angry, Surprised, Flirty, Bored, Neutral

Modes:
- Obstacle ON/OFF
- Follow ON/OFF

Clicking a button sends the command directly to the NanoBot.

-------------------------------------------------------------------------------------------------

## Upload Code to ESP32 (Required First Step)

Before using Voice Control or Button Control, you must upload the firmware to the ESP32.

### 1. Install Arduino IDE
Download and install Arduino IDE from:
https://www.arduino.cc/en/software

### 2. Add ESP32 Board Support
1. Open Arduino IDE
2. Go to **File > Preferences**
3. In "Additional Boards Manager URLs" paste:

https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

4. Go to **Tools > Board > Boards Manager**
5. Search for **ESP32**
6. Install **ESP32 by Espressif Systems**

### 3. Select Your Board
Go to:

Tools > Board > ESP32 Arduino > ESP32 Dev Module

(Or select your specific ESP32 model)

### 4. Select Port
Connect ESP32 via USB.
Go to:

Tools > Port

Select the correct COM / USB port.

### 5. Upload the NanoBot Code
1. Open the NanoBot ESP32 `.ino` file.
2. Click the Upload button.
3. Wait until upload completes successfully.

After successful upload, the ESP32 will start its Wi-Fi network automatically when powered.


## Python Voice Control Setup

### 1. (Optional) Set Whisper Model Directory

```bash
export WHISPER_MODEL_DIR=/home/username/.cache/whisper
```

### 2. Create a Virtual Environment

```bash
python3 -m venv <your_env_name>
```


### 3. Activate the Environment

Linux / macOS:

```bash
source <your_env_name>/bin/activate
```

Windows:

```bash
<your_env_name>\Scripts\activate
```

### 4. Install Dependencies

```bash
pip install -r requirements.txt
```

If PyAudio fails on Linux:

```bash
sudo apt install portaudio19-dev
pip install -r requirements.txt
```

### 5. Run Voice Control

```bash
python nanobot_voice_control.py
```

Speak clearly. Commands will be sent to the ESP32 over Wi-Fi.

-------------------------------------------------------------------------------------------------
## How to Start Button / Web Control

1. Insert batteries into the NanoBot and power it ON.
2. The ESP32 will start its Wi-Fi Access Point.
3. On your phone or laptop, open Wi-Fi settings.
4. Connect to the ESP32 network (example: ESP_CAR).
5. Open your browser.
6. Go to:

http://192.168.4.1

7. The control interface will load.
8. Press the movement, emotion, or mode buttons to control NanoBot.

No Python or computer setup is required for button control.