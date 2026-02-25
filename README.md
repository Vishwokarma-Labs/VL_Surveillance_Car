# NanoBot Voice & Button Controlled Emotional Robot

This project is a small **emotional robot** that can be controlled in **two ways**:

1. **Voice Control** – Speak commands like `forward`, `stop`, `happy`, `angry`, etc.
2. **Web/Buttons Control** – Connect to the ESP32 Wi-Fi network (`192.168.4.1`) and use on-screen buttons.

It uses:

- **ESP32** as the main controller
- **DC motors** (for movement)
- **4 brushed DC motors** with H-bridge driver
- **3 ultrasonic sensors** (front, left, right) for obstacle detection
- **1 PIR sensor** (front) for motion detection
- **1 IR sensor** (rear) for line/follow detection
- **SH1106 OLED display** for animated facial expressions
- **Whisper offline** for speech recognition
- **Python** for voice command handling


## Features

1. **Dual Control Modes**  
   - **Voice** – Speak commands directly into the microphone  
   - **Button/Web Interface** – Connect to the ESP32 Wi-Fi and control using buttons  

2. **Movement Commands**  
   - Forward, Backward, Left, Right, Stop, U-Turn  

3. **Emotional Expressions**  
   - Happy, Sad, Angry, Surprised, Flirty, Bored, Neutral  
   - Animated **eyes, eyebrows, mouth**  

4. **Modes**  
   - Obstacle mode on/off (uses ultrasonic sensors to avoid collisions)  
   - Follow mode on/off (uses IR and PIR sensors for tracking)  

5. **Optimized Voice Control**  
   - 2-second max recording  
   - Early stop when speech ends  
   - Async command sending for low latency  
   - Offline Whisper transcription  

6. **Sensors & Safety**  
   - **3 ultrasonic sensors** for distance measurement  
   - **1 PIR sensor** for motion detection  
   - **1 IR sensor** at the back for line/following  
   - Automatically stops or avoids obstacles in obstacle mode  


## Button Control (Web Interface)

Once connected to the ESP32 Wi-Fi (`192.168.4.1`), a simple web page appears with buttons for:

- **Movement:** Forward, Backward, Left, Right, Stop  
- **Emotions:** Happy, Sad, Angry, Surprised, Flirty, Bored, Neutral  
- **Modes:** Obstacle On/Off, Follow On/Off  

Clicking a button sends a command directly to the NanoBot over Wi-Fi.


##  Python Voice Control Usage

1. Make sure Whisper model is downloaded offline:

```bash
export WHISPER_MODEL_DIR=/home/username/.cache/whisper
