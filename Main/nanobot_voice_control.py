"""
NanoBot Voice Control - OPTIMIZED LATENCY VERSION (with 2-second recording)
Keeps base model but adds early stop, async sending, and faster processing
"""

import os  # Allows us to work with operating system settings

# Force offline mode (so Whisper does NOT try to download anything from internet)
os.environ["TRANSFORMERS_OFFLINE"] = "1"
os.environ["HF_HUB_OFFLINE"] = "1"
os.environ["WHISPER_MODEL_DIR"] = "/home/kalikshya/.cache/whisper"  # Folder where Whisper model is stored locally

import time        # Used for delays and time tracking
import requests    # Used to send HTTP requests to ESP32
import whisper     # OpenAI Whisper speech-to-text model
import numpy as np # Used for processing audio data as numbers
import pyaudio     # Used to record audio from microphone
import threading   # Used to run background tasks
from queue import Queue  # Safe way to pass commands between threads

print("=" * 60)  # Print a separator line
print("🤖 NANOBOT VOICE CONTROL - OPTIMIZED LATENCY MODE")
print("=" * 60)

# ===================== ESP32 CONFIG =====================
ESP_IP = "http://192.168.4.1"  # IP address of your ESP32 robot

# ===================== AUDIO CONFIG =====================
SAMPLE_RATE = 16000      # Audio quality (16,000 samples per second)
CHUNK_SIZE = 1024        # How much audio we read at one time
RECORD_SECONDS = 2       # Maximum recording time (2 seconds)

# ===================== COMMAND QUEUE FOR ASYNC SENDING =====================
command_queue = Queue()  # Queue to store commands before sending them

# ===================== LOAD WHISPER =====================
print("\n🔄 Loading Whisper model...")
try:
    model = whisper.load_model("base", download_root="/home/kalikshya/.cache/whisper")  # Load Whisper base model
    print("✅ Whisper base ready!")
except Exception as e:
    print(f"❌ Failed: {e}")
    exit()  # Stop program if model fails to load

# ===================== AUDIO SETUP =====================
print("🔄 Setting up audio...")
try:
    audio = pyaudio.PyAudio()  # Start audio system
    
    stream = audio.open(       # Open microphone stream
        format=pyaudio.paInt16,  # Audio format
        channels=1,              # Mono audio (1 channel)
        rate=SAMPLE_RATE,        # 16k sample rate
        input=True,              # Input mode (microphone)
        frames_per_buffer=CHUNK_SIZE  # Buffer size
    )
    print("✅ Microphone ready!")
except Exception as e:
    print(f"❌ Audio error: {e}")
    exit()

# ===================== BACKGROUND SENDER THREAD =====================
def send_worker():
    """Background thread for sending commands to ESP32"""
    while True:
        command = command_queue.get()  # Take next command from queue
        
        if command is None:  # If None is received → stop this thread
            break
        
        # URL mapping (which command sends which URL to ESP32)
        url_map = {
            # MOVEMENT
            "forward": f"{ESP_IP}/voice?command=forward",
            "backward": f"{ESP_IP}/voice?command=backward",
            "left": f"{ESP_IP}/voice?command=left",
            "right": f"{ESP_IP}/voice?command=right",
            "stop": f"{ESP_IP}/voice?command=stop",
            "u_turn": f"{ESP_IP}/voice?command=uturn",
            
            # EMOTIONS
            "happy": f"{ESP_IP}/emotion?type=happy",
            "sad": f"{ESP_IP}/emotion?type=sad",
            "angry": f"{ESP_IP}/emotion?type=angry",
            "surprised": f"{ESP_IP}/emotion?type=surprised",
            "bored": f"{ESP_IP}/emotion?type=bored",
            "flirty": f"{ESP_IP}/emotion?type=flirty",
            "neutral": f"{ESP_IP}/emotion?type=neutral",
            
            # MODES
            "obstacle_on": f"{ESP_IP}/mode?auto=1",
            "obstacle_off": f"{ESP_IP}/mode?auto=0",
            "follow_on": f"{ESP_IP}/follow?f=1",
            "follow_off": f"{ESP_IP}/follow?f=0",
        }
        
        # Special handling for u_turn
        if command == "u_turn":
            url = f"{ESP_IP}/voice?command=uturn"
        elif command in url_map:
            url = url_map[command]
        else:
            print(f"❌ Unknown command: {command}")
            continue
        
        try:
            # Send request in separate thread so main program does not wait
            threading.Thread(target=requests.get, 
                            args=(url,), 
                            kwargs={'timeout': 1}).start()
            print(f"✅ Sent: {command}")
        except Exception as e:
            print(f"⚠️  Failed to send {command}: {e}")

# Start the background sender thread
sender_thread = threading.Thread(target=send_worker, daemon=True)
sender_thread.start()

# ===================== FAST RECORD WITH EARLY STOP =====================
def record_audio():
    """Record audio with early stop when speech ends - keeps 2 second max"""
    print("🎤 Recording...", end="", flush=True)
    
    frames = []  # Store small pieces of audio
    voice_detected = False
    silence_chunks = 0
    max_silence = 8  # Stop after small silence period
    total_chunks = int(SAMPLE_RATE / CHUNK_SIZE * RECORD_SECONDS)  # Total chunks in 2 seconds
    
    for i in range(total_chunks):
        try:
            data = stream.read(CHUNK_SIZE, exception_on_overflow=False)  # Read audio chunk
            frames.append(data)
            
            # Convert raw audio into numbers
            audio_chunk = np.frombuffer(data, dtype=np.int16)
            volume = np.abs(audio_chunk).mean()  # Check average loudness
            
            if volume > 300:  # If loud enough → speech detected
                voice_detected = True
                silence_chunks = 0
                print("🗣️", end="", flush=True)
            else:
                silence_chunks += 1
                print(".", end="", flush=True)
            
            # If we already heard speech and now silence continues → stop early
            if voice_detected and silence_chunks >= max_silence:
                print(" (done early)", end="", flush=True)
                break
                
        except Exception as e:
            print(f" Error: {e}")
            break
    
    print()
    
    if not frames:
        return None
    
    # Convert recorded data into float format for Whisper
    audio_array = np.frombuffer(b"".join(frames), dtype=np.int16)
    audio_float = audio_array.astype(np.float32) / 32768.0
    return audio_float

# ===================== FAST COMMAND MATCHING =====================
def match_command(text):
    """Simple fast command matching"""
    text = text.lower().strip()  # Convert to lowercase
    
    # Remove punctuation marks
    for char in ".,!?;:'\"":
        text = text.replace(char, "")
    
    words = text.split()  # Split sentence into words
    
    if len(words) > 4:  # Ignore long sentences
        print(f"⚠️  Too many words ({len(words)}), ignoring")
        return None
    
    print(f"🔍 Processing: '{text}'")
    
    # Map spoken words to robot commands
    command_map = {
        "forward": "forward", "front": "forward", "go": "forward", "move": "forward",
        "backward": "backward", "back": "backward",
        "left": "left", "right": "right",
        "stop": "stop", "halt": "stop", "freeze": "stop",
        "u turn": "u_turn", "u-turn": "u_turn", "turn around": "u_turn", "reverse": "u_turn", "flip": "u_turn",
        
        "happy": "happy", "sad": "sad", "angry": "angry",
        "surprise": "surprised", "surprised": "surprised",
        "bored": "bored", "board": "bored",
        "flirty": "flirty", "flirt": "flirty","30": "flirty","floor t": "flirty",
        "neutral": "neutral",
        
        "obstacle": "obstacle_on", "avoid": "obstacle_on", "obstacle on": "obstacle_on", "obstacle mode on": "obstacle_on",
        "manual": "obstacle_off", "obstacle off": "obstacle_off", "obstacle mode off": "obstacle_off",
        "follow": "follow_on", "follow me": "follow_on",
        "stop follow": "follow_off", "follow off": "follow_off", "don't follow": "follow_off"
    }
    
    if text in command_map:
        print(f"🎯 Exact match: '{text}' → '{command_map[text]}'")
        return command_map[text]
    
    if words and words[0] in command_map:
        print(f"🎯 First word match: '{words[0]}' → '{command_map[words[0]]}'")
        return command_map[words[0]]
    
    for word in words:
        if word in command_map:
            print(f"🎯 Word match: '{word}' → '{command_map[word]}'")
            return command_map[word]
    
    # Handle common speech recognition mistakes
    mishearings = {
        "ford": "forward", "fourth": "forward",
        "stock": "stop", "stopp": "stop",
        "flood": "flirty", "thirty": "flirty", "dirty": "flirty",
        "bord": "bored"
    }
    
    for wrong, correct in mishearings.items():
        if wrong in text:
            print(f"🎯 Corrected '{wrong}' → '{correct}'")
            return correct
    
    return None

# ===================== TRANSCRIBE =====================
def transcribe_audio(audio_data):
    """Convert speech audio to text using Whisper"""
    try:
        result = model.transcribe(audio_data, fp16=False, language="en")
        text = result["text"].strip()
        return text
    except Exception as e:
        print(f"❌ Transcription error: {e}")
        return ""

# ===================== SEND TO ESP32 =====================
def send_to_esp(command):
    """Put command into queue so background thread can send it"""
    command_queue.put(command)
    return True

# ===================== MAIN PROGRAM =====================
def main():
    print("\n" + "="*60)
    print("🎯 OPTIMIZED VOICE CONTROL - 2 SECOND RECORDING")
    print("="*60)

    print("\n📢 SAY COMMANDS CLEARLY:")
    print("\n  MOVEMENT: 'forward', 'front', 'go', 'move', 'stop', 'backward', 'back', 'left', 'right'")
    print("  COMBINED: 'go forward', 'move forward', 'go back', 'turn left', 'turn right'")
    print("  U-TURN: 'U-turn', 'turn around'")
    print("  EMOTIONS: 'happy', 'sad', 'angry', 'surprised', 'bored', 'board', 'flirty', 'neutral'")
    print("  MODES: 'obstacle on', 'obstacle off', 'manual', 'follow', 'stop follow'")
    print("\n⚡ OPTIMIZATIONS ENABLED:")
    print("  • Early stop when speech ends")
    print("  • Async command sending (no waiting)")
    print("  • Fast command matching")
    print("  • 2-second max recording (your preference)")
    print("-"*60)
    
    print("\n🔌 Testing ESP32 connection...")
    connected = False
    try:
        response = requests.get(f"{ESP_IP}/", timeout=2)
        if response.status_code == 200:
            connected = True
            print(f"✅ Connected! (Status: {response.status_code})")
    except Exception as e:
        print(f"⚠️  ESP32 not found: {e}")
        print("   Running in demo mode - commands will be queued but not sent")
    
    print("\n🎤 READY! Press Ctrl+C to quit")
    print("-"*60)
    
    last_command_time = 0
    min_command_interval = 0.8  # Prevent double triggers
    
    try:
        while True: #start a loop
            audio_data = record_audio() # record  voice from microphone
            if audio_data is None or len(audio_data) < SAMPLE_RATE * 0.3:#if nothing recorded or audio is shorter than 0.3 seconds
                print("❌ Too short, try again")
                time.sleep(0.3) #wait 0.3 seconds
                continue #back to loop and listen again
            
            text = transcribe_audio(audio_data)# send recorded audio to whisper, convert speech to text
            if not text:
                print("❌ No speech detected")
                time.sleep(0.3)
                continue
            
            print(f"🗣️  Heard: '{text}'") #show what whisper understood
            
            if text in ["quit", "exit", "goodbye"]:
                print("👋 Exiting...")
                break
            
            current_time = time.time()
            # if new command is coming to fast after the last one
            if current_time - last_command_time < min_command_interval:
                print(f"⏱️  Too fast ({(current_time - last_command_time):.1f}s), ignoring...") #ignore it/prevents double triggering
                print("-"*40)
                continue
            
            command = match_command(text)
            if command: #if command is valid send it ESP32
                send_to_esp(command)
                last_command_time = current_time
            else:
                print("🤔 Not a recognized command")
            
            print("-"*40)
            time.sleep(0.3)#small delay before next listening
            
    except KeyboardInterrupt: #catches the manual stop from keyboard
        print("\n👋 Stopped by user")
    finally:
        command_queue.put(None)  # Stop background thread
        #properly close microphone and free audio resources
        stream.stop_stream()
        stream.close()
        audio.terminate()
        print("Voice control stopped")

# ===================== RUN =====================
if __name__ == "__main__":
    main()