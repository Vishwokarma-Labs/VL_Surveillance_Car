#include <WiFi.h>
#include <WebServer.h>
#include <U8g2lib.h>
#include <Wire.h>

// ================= OLED Display =================
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Include emotion files
#include "emotion.h"
#include "animation.h"
#include "face.h"

// Emotion update timing
unsigned long lastEmotionUpdate = 0;
const unsigned long EMOTION_UPDATE_INTERVAL = 50;

// ================= WiFi =================
const char* apSsid = "ESP32_Car";
const char* apPassword = "12345678";
WebServer server(80);

// ================= L298N =================
#define IN1 14      // LEFT motor forward
#define IN2 27      // LEFT motor backward
#define IN3 26      // RIGHT motor forward
#define IN4 25      // RIGHT motor backward
#define ENA_PIN 33  // LEFT motor PWM
#define ENB_PIN 32  // RIGHT motor PWM

// ================= PWM =================
const int PWM_FREQ = 20000;
const int PWM_RES  = 8;
const int CH_A = 0;    // LEFT motor channel
const int CH_B = 1;    // RIGHT motor channel

// ================= Speed =================
uint8_t manualSpeed = 230;
uint8_t autoSpeed   = 230;
uint8_t turnSpeed   = 215;     
uint8_t backupSpeed = 215;
uint8_t uTurnSpeed = 205;     // Slightly slower for U-turn

// ================= Ultrasonic Sensors (3 total) =================
#define TRIG_CENTER 4   // Front-Center (90° - straight ahead)
#define ECHO_CENTER 5

#define TRIG_LEFT 12    // Front-Left (45° - angled left)
#define ECHO_LEFT 13

#define TRIG_RIGHT 15   // Front-Right (135° - angled right)
#define ECHO_RIGHT 2

// ================= Flags =================
// Manual mode flags (for single-click actions)
bool manualForward = false;
bool manualBack = false;
bool manualLeft = false;
bool manualRight = false;
unsigned long manualActionStart = 0;
bool manualActionActive = false;
uint8_t currentManualAction = 0; // 0=none, 1=forward, 2=back, 3=left, 4=right

// Voice/Continuous mode flags (for continuous movement)
bool voiceForward = false;
bool voiceBack = false;
bool voiceLeft = false;
bool voiceRight = false;
bool voiceUTurn = false;
unsigned long uTurnStartTime = 0;
bool uTurnActive = false;

bool obstacleMode = false;
bool followMode   = false;

// ================= Auto Obstacle =================
enum AutoState { AUTO_IDLE, AUTO_FORWARD, AUTO_BACKWARD, AUTO_TURN_LEFT, AUTO_TURN_RIGHT };
AutoState autoState = AUTO_IDLE;

unsigned long autoStateStart = 0;
bool lastTurnWasLeft = true;

// Optimized timing
const unsigned long BACK_TIME = 300;       // Short backup
const unsigned long TURN_TIME = 375;       // Smooth turn

// Manual action timing
const unsigned long MANUAL_FORWARD_TIME = 300;  // 300ms forward
const unsigned long MANUAL_BACKWARD_TIME = 300; // 300ms backward
const unsigned long MANUAL_TURN_TIME = 250;     // 250ms turn

// U-turn timing (180 degree turn)
const unsigned long U_TURN_TIME = 1000;     // 800ms for 180 degree turn

// ================= REDUCED DETECTION RANGES (200cm MAX) =================
const float DANGER_ZONE = 20.0;           // Stop immediately!
const float ACTION_ZONE = 25.0;           // Take action here
const float WARNING_ZONE = 70.0;          // Slow down
const float MAX_DETECTION = 100.0;        // IGNORE anything beyond 200cm

// For angled sensors
const float SIDE_DANGER_ZONE = 25.0;      // Side danger
const float SIDE_ACTION_ZONE = 35.0;      // Side action
const float SIDE_WARNING_ZONE = 90.0;     // Side warning

// Turn clearance
const float TURN_CLEARANCE_DIST = 60.0;   // Good clearance

// Emotion timing
unsigned long lastObstacleTime = 0;
const unsigned long OBSTACLE_EMOTION_TIME = 3000; // Show angry for 3s after obstacle
unsigned long lastActivityTime = 0;
const unsigned long IDLE_BORED_TIME = 15000; // Show bored after 15s idle

// ================= HTML with Emotion Controls =================
const char index_html[] PROGMEM = R"=====(<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body{background:#111;color:#fff;font-family:Arial;text-align:center}
.btn{padding:16px 26px;margin:6px;border:none;border-radius:10px;background:#333;color:#fff;font-size:16px}
.btn:active{background:#555}
.status{background:#222;padding:10px;border-radius:8px;margin:15px auto;max-width:300px}
.emotion-btn{background:#444;color:#ffcc00;font-weight:bold}
.mode-btn{background:#0066cc;color:white}
.stop-btn{background:#cc3300;color:white}
</style>
</head>
<body>

<h2>🤖 ESP32 Robot Car with Emotions</h2>

<div class="status">
  <p>Detection Ranges: <br>
  Danger: &lt;20cm<br>
  Action: &lt;35cm<br>
  Warning: &lt;70cm<br>
  Max: &lt;150cm</p>
</div>

<h3>Manual Control (Single Click = One Movement)</h3>
<button class="btn" 
        onmousedown="cmd('f',1)" 
        onmouseup="cmd('f',0)" 
        onmouseleave="cmd('f',0)"
        ontouchstart="cmd('f',1)" 
        ontouchend="cmd('f',0)" 
        ontouchcancel="cmd('f',0)">Forward</button><br>

<button class="btn" 
        onmousedown="cmd('l',1)" 
        onmouseup="cmd('l',0)" 
        onmouseleave="cmd('l',0)"
        ontouchstart="cmd('l',1)" 
        ontouchend="cmd('l',0)" 
        ontouchcancel="cmd('l',0)">Left</button>

<button class="btn stop-btn" onclick="cmd('s',1)">Stop</button>

<button class="btn" 
        onmousedown="cmd('r',1)" 
        onmouseup="cmd('r',0)" 
        onmouseleave="cmd('r',0)"
        ontouchstart="cmd('r',1)" 
        ontouchend="cmd('r',0)" 
        ontouchcancel="cmd('r',0)">Right</button><br>

<button class="btn" 
        onmousedown="cmd('b',1)" 
        onmouseup="cmd('b',0)" 
        onmouseleave="cmd('b',0)"
        ontouchstart="cmd('b',1)" 
        ontouchend="cmd('b',0)" 
        ontouchcancel="cmd('b',0)">Backward</button>

<h3>🤖 Emotion Control</h3>
<button class="btn emotion-btn" onclick="emotion('neutral')">😐 Neutral</button>
<button class="btn emotion-btn" onclick="emotion('happy')">😊 Happy</button>
<button class="btn emotion-btn" onclick="emotion('sad')">😢 Sad</button>
<button class="btn emotion-btn" onclick="emotion('angry')">😠 Angry</button>
<button class="btn emotion-btn" onclick="emotion('surprised')">😲 Surprised</button>
<button class="btn emotion-btn" onclick="emotion('bored')">😑 Bored</button>
<button class="btn emotion-btn" onclick="emotion('flirty')">😉 Flirty</button>

<h3>⚙️ Mode Control</h3>
<button class="btn mode-btn" onclick="mode(1)">OBSTACLE AVOID ON</button>
<button class="btn" onclick="mode(0)">OFF</button>

<h3>👣 Follow Mode</h3>
<button class="btn mode-btn" onclick="follow(1)">FOLLOW ON</button>
<button class="btn" onclick="follow(0)">FOLLOW OFF</button>

<script>
function cmd(d,s){ 
  fetch(`/cmd?dir=${d}&state=${s}`);
  if(s==1) fetch('/activity'); // Mark activity
}
function mode(v){ fetch(`/mode?auto=${v}`); }
function follow(v){ fetch(`/follow?f=${v}`); }
function emotion(e){ fetch(`/emotion?type=${e}`); }
</script>

</body>
</html>)=====";

// ================= MOTOR CONTROL =================
void setMotorSpeed(uint8_t leftSpeed, uint8_t rightSpeed){
  ledcWrite(CH_A, leftSpeed);
  ledcWrite(CH_B, rightSpeed);
}

void driveStop(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  setMotorSpeed(0, 0);
}

void driveForward(uint8_t speed){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  setMotorSpeed(speed, speed);
  lastActivityTime = millis();
}

void driveBackward(uint8_t speed){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  setMotorSpeed(speed, speed);
  lastActivityTime = millis();
}

void driveLeft(uint8_t speed){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  setMotorSpeed(speed, speed);
  lastActivityTime = millis();
}

void driveRight(uint8_t speed){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  setMotorSpeed(speed, speed);
  lastActivityTime = millis();
}

// NEW: U-Turn function (180 degree turn)
void driveUTurn(uint8_t speed){
  digitalWrite(IN1, HIGH);   // Left motor forward
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);    // Right motor backward
  digitalWrite(IN4, HIGH);
  setMotorSpeed(speed, speed);
  lastActivityTime = millis();
}

// ================= ULTRASONIC FUNCTIONS =================
float getDistanceCm(uint8_t trigPin, uint8_t echoPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);  // Normal pulse
  digitalWrite(trigPin, LOW);
  
  unsigned long duration = pulseIn(echoPin, HIGH, 26000); // ~450cm max
  
  if(duration == 0) return 999.0;
  
  float distance = duration * 0.0343 / 2;
  
  // IGNORE anything beyond 100cm (your requirement)
  if(distance > MAX_DETECTION) return 999.0;
  
  return distance;
}

void getAllDistances(float &center, float &left, float &right){
  center = getDistanceCm(TRIG_CENTER, ECHO_CENTER);
  delayMicroseconds(50);
  left = getDistanceCm(TRIG_LEFT, ECHO_LEFT);
  delayMicroseconds(50);
  right = getDistanceCm(TRIG_RIGHT, ECHO_RIGHT);
}

// ================= Manual Control (Single Click) =================
void handleManual(){
  unsigned long now = millis();
  
  // Handle U-turn first (if active)
  if(uTurnActive) {
    if(now - uTurnStartTime >= U_TURN_TIME) {
      // U-turn completed
      driveStop();
      uTurnActive = false;
      voiceUTurn = false;
      Serial.println("Voice: U-turn completed (180 degree turn)");
    } else {
      // Continue U-turn
      driveUTurn(uTurnSpeed);
      return;
    }
  }
  
  // Handle manual single-click movements
  if(manualActionActive) {
    // Check if manual action time is complete
    unsigned long actionTime = 0;
    
    if(currentManualAction == 1) actionTime = MANUAL_FORWARD_TIME;
    else if(currentManualAction == 2) actionTime = MANUAL_BACKWARD_TIME;
    else if(currentManualAction == 3 || currentManualAction == 4) actionTime = MANUAL_TURN_TIME;
    
    if(now - manualActionStart >= actionTime) {
      // Stop the manual action
      driveStop();
      manualActionActive = false;
      currentManualAction = 0;
      Serial.println("Manual: Action completed");
    }
  }
  
  // Start a new manual action if requested
  if(!manualActionActive && !uTurnActive) {
    if(manualForward){
      driveForward(manualSpeed);
      manualActionStart = now;
      manualActionActive = true;
      currentManualAction = 1;
      manualForward = false; // Reset flag immediately
      Serial.println("Manual: Forward (single step)");
    }
    else if(manualBack){
      driveBackward(manualSpeed);
      manualActionStart = now;
      manualActionActive = true;
      currentManualAction = 2;
      manualBack = false; // Reset flag immediately
      Serial.println("Manual: Backward (single step)");
    }
    else if(manualLeft){
      driveLeft(manualSpeed);
      manualActionStart = now;
      manualActionActive = true;
      currentManualAction = 3;
      manualLeft = false; // Reset flag immediately
      Serial.println("Manual: Left (single step)");
    }
    else if(manualRight){
      driveRight(manualSpeed);
      manualActionStart = now;
      manualActionActive = true;
      currentManualAction = 4;
      manualRight = false; // Reset flag immediately
      Serial.println("Manual: Right (single step)");
    }
  }
  
  // Handle voice continuous movements (separate from manual)
  if(voiceForward){
    driveForward(manualSpeed);
    Serial.println("Voice: Forward (continuous)");
  }
  else if(voiceBack){
    driveBackward(manualSpeed);
    Serial.println("Voice: Backward (continuous)");
  }
  else if(voiceLeft){
    driveLeft(manualSpeed);
    Serial.println("Voice: Left (continuous)");
  }
  else if(voiceRight){
    driveRight(manualSpeed);
    Serial.println("Voice: Right (continuous)");
  }
  
  // Stop if no actions are active
  if(!manualActionActive && !uTurnActive && !voiceForward && !voiceBack && !voiceLeft && !voiceRight) {
    driveStop();
  }
}

// ================= LESS SENSITIVE OBSTACLE AVOIDANCE =================
void handleObstacle(){
  unsigned long now = millis();
  static float lastCenterDist = 999.0;
  static float lastLeftDist = 999.0;
  static float lastRightDist = 999.0;
  static unsigned long lastTurnTime = 0;
  
  // Read sensors
  getAllDistances(lastCenterDist, lastLeftDist, lastRightDist);
  
  // Check for obstacles to trigger emotions
  if(lastCenterDist < DANGER_ZONE || lastLeftDist < SIDE_DANGER_ZONE || lastRightDist < SIDE_DANGER_ZONE){
    lastObstacleTime = now;
    emotionSet(EMO_ANGRY);
  } else if(lastCenterDist < ACTION_ZONE || lastLeftDist < SIDE_ACTION_ZONE || lastRightDist < SIDE_ACTION_ZONE){
    lastObstacleTime = now;
    emotionSet(EMO_SURPRISED);
  }
  
  switch(autoState){
    case AUTO_IDLE: 
      autoState = AUTO_FORWARD;
      break;
      
    case AUTO_FORWARD:{
      bool dangerCenter = (lastCenterDist < DANGER_ZONE);
      bool actionCenter = (lastCenterDist < ACTION_ZONE);
      bool warningCenter = (lastCenterDist < WARNING_ZONE);
      
      bool dangerLeft = (lastLeftDist < SIDE_DANGER_ZONE);
      bool actionLeft = (lastLeftDist < SIDE_ACTION_ZONE);
      
      bool dangerRight = (lastRightDist < SIDE_DANGER_ZONE);
      bool actionRight = (lastRightDist < SIDE_ACTION_ZONE);
      
      if(dangerCenter || dangerLeft || dangerRight){
        driveStop();
        delay(100);
        autoState = AUTO_BACKWARD;
        autoStateStart = now;
        driveBackward(backupSpeed);
      }
      else if(actionCenter && (now - lastTurnTime > 1000)){
        autoState = AUTO_BACKWARD;
        autoStateStart = now;
        driveBackward(backupSpeed);
        lastTurnTime = now;
      }
      else if(actionLeft && !actionRight && (now - lastTurnTime > 1500)){
        autoState = AUTO_TURN_RIGHT;
        autoStateStart = now;
        driveRight(turnSpeed);
        lastTurnTime = now;
      }
      else if(!actionLeft && actionRight && (now - lastTurnTime > 1500)){
        autoState = AUTO_TURN_LEFT;
        autoStateStart = now;
        driveLeft(turnSpeed);
        lastTurnTime = now;
      }
      else if(warningCenter){
        driveForward(autoSpeed * 0.85);
      }
      else {
        driveForward(autoSpeed);
      }
    }
    break;
    
    case AUTO_BACKWARD:{
      driveBackward(backupSpeed);
      
      if(now - autoStateStart >= BACK_TIME){
        getAllDistances(lastCenterDist, lastLeftDist, lastRightDist);
        
        if(lastLeftDist > lastRightDist){
          autoState = AUTO_TURN_LEFT;
          autoStateStart = now;
          driveLeft(turnSpeed);
        } else {
          autoState = AUTO_TURN_RIGHT;
          autoStateStart = now;
          driveRight(turnSpeed);
        }
      }
    }
    break;
    
    case AUTO_TURN_LEFT:
      driveLeft(turnSpeed);
      if(now - autoStateStart >= TURN_TIME){
        getAllDistances(lastCenterDist, lastLeftDist, lastRightDist);
        
        if(lastCenterDist > TURN_CLEARANCE_DIST){
          autoState = AUTO_FORWARD;
          driveForward(autoSpeed);
        } else {
          autoState = AUTO_TURN_RIGHT;
          autoStateStart = now;
          driveRight(turnSpeed);
        }
      }
      break;
      
    case AUTO_TURN_RIGHT:
      driveRight(turnSpeed);
      if(now - autoStateStart >= TURN_TIME){
        getAllDistances(lastCenterDist, lastLeftDist, lastRightDist);
        
        if(lastCenterDist > TURN_CLEARANCE_DIST){
          autoState = AUTO_FORWARD;
          driveForward(autoSpeed);
        } else {
          autoState = AUTO_TURN_LEFT;
          autoStateStart = now;
          driveLeft(turnSpeed);
        }
      }
      break;
  }
}

// ================= Follow Mode =================
void handleFollow(){
  float centerDist = getDistanceCm(TRIG_CENTER, ECHO_CENTER);
  
  if(centerDist < 0 || centerDist > MAX_DETECTION){
    driveStop(); 
    return; 
  }
  
  if(centerDist < 20){
    driveBackward(autoSpeed * 0.8);
  }
  else if(centerDist <= 35){
    driveStop();
  }
  else if(centerDist <= 80){
    driveForward(autoSpeed * 0.9);
  }
  else {
    driveStop();
  }
}

// ================= Emotion Management =================
void updateEmotionBasedOnState() {
  unsigned long now = millis();
  
  // If obstacle detected recently, show angry/surprised
  if(now - lastObstacleTime < OBSTACLE_EMOTION_TIME) {
    // Emotion already set in handleObstacle()
    return;
  }
  
  // Check current mode
  if(followMode) {
    emotionSet(EMO_FLIRTY); // Flirty when following
  }
  else if(obstacleMode) {
    // In obstacle mode but no recent obstacles - show neutral/happy
    if(emotionGet() == EMO_ANGRY || emotionGet() == EMO_SURPRISED) {
      emotionSet(EMO_NEUTRAL);
    }
  }
  
  // Check for idle/bored state
  if(now - lastActivityTime > IDLE_BORED_TIME) {
    emotionSet(EMO_BORED);
  } else if(emotionGet() == EMO_BORED && now - lastActivityTime < 1000) {
    // If was bored and now active, go back to neutral
    emotionSet(EMO_NEUTRAL);
  }
  
  // Update emotion idle state
  emotionIdleUpdate();
}

// ================= HTTP Handlers =================
void handleRoot(){ 
  server.send_P(200,"text/html",index_html); 
}

// Manual control handler (single click) - SIMPLE VERSION
void handleCmd(){
  String d = server.arg("dir");
  bool p = server.arg("state").toInt();
  
  // STOP command should work in ANY mode
  if(d == "s"){
    // Stop everything
    manualForward = manualBack = manualLeft = manualRight = false;
    manualActionActive = false;
    voiceForward = voiceBack = voiceLeft = voiceRight = false;
    voiceUTurn = false;
    uTurnActive = false;
    obstacleMode = false;
    followMode = false;
    autoState = AUTO_IDLE;
    driveStop();
    
    Serial.println("EMERGENCY STOP: All modes stopped");
    lastActivityTime = millis();
    emotionMarkActive();
    server.send(200,"text/plain","STOP_OK");
    return;
  }
  
  // For other movement commands, check auto modes
  if(obstacleMode || followMode){
    server.send(200,"text/plain","IGNORED");
    return;
  }

  // MANUAL BUTTON HANDLING - SUPER SIMPLE
  if(p == 1){  // Button pressed
    // Clear voice commands when manual is used
    voiceForward = voiceBack = voiceLeft = voiceRight = false;
    voiceUTurn = false;
    uTurnActive = false;
    
    // Set the manual action flag
    if(d == "f") manualForward = true;
    else if(d == "b") manualBack = true;
    else if(d == "l") manualLeft = true;
    else if(d == "r") manualRight = true;
    
    Serial.print("Manual button pressed: ");
    Serial.println(d);
  }
  // We ignore p==0 (button release) for manual mode
  
  lastActivityTime = millis();
  emotionMarkActive();
  server.send(200,"text/plain","MANUAL_OK");
}

// Voice command handler for Python voice control - SEPARATE ENDPOINT
void handleVoiceCommand() {
  String command = server.arg("command");
  
  Serial.print("Voice command received: ");
  Serial.println(command);
  
  // Clear manual actions first
  manualForward = manualBack = manualLeft = manualRight = false;
  manualActionActive = false;
  
  // Handle U-turn command
  if(command == "uturn" || command == "u_turn") {
    voiceUTurn = true;
    uTurnActive = true;
    uTurnStartTime = millis();
    
    // Clear all other voice commands
    voiceForward = voiceBack = voiceLeft = voiceRight = false;
    
    Serial.println("Voice: U-turn (180 degree rotation)");
    
    // Also disable obstacle and follow modes when using voice
    obstacleMode = false;
    followMode = false;
    autoState = AUTO_IDLE;
    
    lastActivityTime = millis();
    emotionMarkActive();
    server.send(200,"text/plain","U_TURN_OK");
    return;
  }
  
  // Handle other voice commands
  if(command == "forward") {
    voiceForward = true;
    voiceBack = voiceLeft = voiceRight = false;
    voiceUTurn = false;
    uTurnActive = false;
    Serial.println("Voice: Forward (continuous)");
  }
  else if(command == "backward") {
    voiceBack = true;
    voiceForward = voiceLeft = voiceRight = false;
    voiceUTurn = false;
    uTurnActive = false;
    Serial.println("Voice: Backward (continuous)");
  }
  else if(command == "left") {
    voiceLeft = true;
    voiceForward = voiceBack = voiceRight = false;
    voiceUTurn = false;
    uTurnActive = false;
    Serial.println("Voice: Left (continuous)");
  }
  else if(command == "right") {
    voiceRight = true;
    voiceForward = voiceBack = voiceLeft = false;
    voiceUTurn = false;
    uTurnActive = false;
    Serial.println("Voice: Right (continuous)");
  }
  else if(command == "stop") {
    voiceForward = voiceBack = voiceLeft = voiceRight = false;
    voiceUTurn = false;
    uTurnActive = false;
    Serial.println("Voice: Stop movement");
  }
  else {
    server.send(400,"text/plain","UNKNOWN_VOICE_COMMAND");
    return;
  }
  
  // Also disable obstacle and follow modes when using voice
  obstacleMode = false;
  followMode = false;
  autoState = AUTO_IDLE;
  
  lastActivityTime = millis();
  emotionMarkActive();
  server.send(200,"text/plain","VOICE_OK");
}

void handleMode(){
  obstacleMode = server.arg("auto").toInt();
  followMode = false;
  autoState = AUTO_FORWARD;
  
  // Clear all movement commands when switching modes
  manualForward = manualBack = manualLeft = manualRight = false;
  manualActionActive = false;
  voiceForward = voiceBack = voiceLeft = voiceRight = false;
  voiceUTurn = false;
  uTurnActive = false;
  
  driveStop();
  
  if(obstacleMode) {
    emotionSet(EMO_NEUTRAL);
    Serial.println("Voice: Obstacle Avoidance Mode ON");
  } else {
    Serial.println("Voice: Obstacle Avoidance Mode OFF");
  }
  
  server.send(200,"text/plain","OK");
}

void handleFollowHttp(){
  followMode = server.arg("f").toInt();
  obstacleMode = false;
  
  // Clear all movement commands when switching modes
  manualForward = manualBack = manualLeft = manualRight = false;
  manualActionActive = false;
  voiceForward = voiceBack = voiceLeft = voiceRight = false;
  voiceUTurn = false;
  uTurnActive = false;
  
  driveStop();
  
  if(followMode) {
    emotionSet(EMO_FLIRTY);
    Serial.println("Voice: Follow Mode ON");
  } else {
    Serial.println("Voice: Follow Mode OFF");
  }
  
  server.send(200,"text/plain","OK");
}

// Combined emotion handler that works with BOTH web and voice
void handleEmotion(){
  String emotionType = server.arg("type");
  String emotionValue = server.arg("e");
  
  // Support both parameter names: "type" (voice) and "e" (web)
  if(emotionType.length() == 0 && emotionValue.length() > 0) {
    emotionType = emotionValue;
  }
  
  Serial.print("Voice/Web Emotion: ");
  Serial.println(emotionType);
  
  if(emotionType == "neutral") {
    emotionSet(EMO_NEUTRAL);
    Serial.println("Emotion: Neutral");
  }
  else if(emotionType == "happy") {
    emotionSet(EMO_HAPPY);
    Serial.println("Emotion: Happy");
  }
  else if(emotionType == "sad") {
    emotionSet(EMO_SAD);
    Serial.println("Emotion: Sad");
  }
  else if(emotionType == "angry") {
    emotionSet(EMO_ANGRY);
    Serial.println("Emotion: Angry");
  }
  else if(emotionType == "surprised") {
    emotionSet(EMO_SURPRISED);
    Serial.println("Emotion: Surprised");
  }
  else if(emotionType == "bored") {
    emotionSet(EMO_BORED);
    Serial.println("Emotion: Bored");
  }
  else if(emotionType == "flirty") {
    emotionSet(EMO_FLIRTY);
    Serial.println("Emotion: Flirty");
  }
  
  lastActivityTime = millis();
  server.send(200,"text/plain","OK");
}

void handleActivity(){
  lastActivityTime = millis();
  emotionMarkActive();
  server.send(200,"text/plain","OK");
}

// ================= SETUP =================
void setup(){
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=== ESP32 Robot Car with Emotions ===");
  Serial.println("Manual mode: Single click = one movement");
  Serial.println("Voice mode: Continuous movement until stop");
  Serial.println("U-turn: 180 degree rotation");
  Serial.println("Connect to WiFi: ESP32_Car");
  Serial.println("Password: 12345678");
  Serial.println("IP: 192.168.4.1");

  // Initialize I2C with specific pins for OLED
  Wire.begin(21, 22);  // SDA=21, SCL=22
  
  // Initialize OLED
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(10, 30, "System Ready!");
  u8g2.sendBuffer();
  
  // Initialize emotion system
  emotionSet(EMO_NEUTRAL);
  lastActivityTime = millis();

  // Initialize all movement flags
  manualForward = manualBack = manualLeft = manualRight = false;
  manualActionActive = false;
  currentManualAction = 0;
  voiceForward = voiceBack = voiceLeft = voiceRight = false;
  voiceUTurn = false;
  uTurnActive = false;
  obstacleMode = false;
  followMode = false;

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Initialize motors to stop
  driveStop();

  // PWM setup
  ledcSetup(CH_A, PWM_FREQ, PWM_RES);
  ledcAttachPin(ENA_PIN, CH_A);
  ledcSetup(CH_B, PWM_FREQ, PWM_RES);
  ledcAttachPin(ENB_PIN, CH_B);

  // Ultrasonic sensor pins
  pinMode(TRIG_CENTER, OUTPUT);
  pinMode(ECHO_CENTER, INPUT);
  pinMode(TRIG_LEFT, OUTPUT);
  pinMode(ECHO_LEFT, INPUT);
  pinMode(TRIG_RIGHT, OUTPUT);
  pinMode(ECHO_RIGHT, INPUT);

  // Initialize sensors
  digitalWrite(TRIG_CENTER, LOW);
  digitalWrite(TRIG_LEFT, LOW);
  digitalWrite(TRIG_RIGHT, LOW);
  delay(100);

  // WiFi AP
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSsid, apPassword);

  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  // Web server routes
  server.on("/", handleRoot);
  server.on("/cmd", handleCmd);              // Manual control ONLY (single click)
  server.on("/voice", handleVoiceCommand);   // Voice control ONLY (continuous + U-turn)
  server.on("/mode", handleMode);            // For obstacle mode: /mode?auto=1
  server.on("/follow", handleFollowHttp);    // For follow mode: /follow?f=1
  server.on("/emotion", handleEmotion);      // For emotions: /emotion?type=happy OR /emotion?e=happy
  server.on("/activity", handleActivity);
  
  server.begin();
  
  Serial.println("Web server started");
  Serial.println("Manual buttons: Single click via /cmd");
  Serial.println("Voice commands: Continuous via /voice?command=forward");
  Serial.println("U-turn: /voice?command=uturn");
  
  // Show happy face on startup
  emotionSet(EMO_HAPPY);
  delay(1000);
  emotionSet(EMO_NEUTRAL);
}

// ================= LOOP =================
void loop(){
  server.handleClient();
  
  // Update animation
  animUpdate();
  
  // Update emotion based on robot state
  updateEmotionBasedOnState();
  
  // Draw face using your existing faceDraw() function
  u8g2.clearBuffer();
  faceDraw();  // This will draw the current emotion
  u8g2.sendBuffer();
  
  // Handle robot modes
  if(followMode){
    handleFollow();
  }
  else if(obstacleMode){
    handleObstacle();
  } 
  else {
    handleManual();
  }
  
  delay(10);
}