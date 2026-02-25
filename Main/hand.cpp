#include "hand.h"
#include "emotion.h"
#include <ESP32Servo.h> // Library for controlling servo motors on ESP32

// ===================== GLOBAL VARIABLES =====================
static Emotion lastEmotion = EMO_NEUTRAL; // Remember last emotion
static Servo leftHand;                    // Servo object for left hand
static Servo rightHand;                   // Servo object for right hand

#define LEFT_HAND_PIN  18   // GPIO pin for left hand servo
#define RIGHT_HAND_PIN 19   // GPIO pin for right hand servo

static unsigned long lastMove = 0; // Timestamp of last motion
static bool phase = false;         // Phase toggle for motion (used to alternate positions)

// ===================== MOTION FUNCTIONS =====================

// Neutral position (hands relaxed, middle)
static void neutral(){
  leftHand.write(90);  // Middle position (0-180 degrees)
  rightHand.write(90); 
}

// Angry motion (fast, alternating hand positions)
static void angryMotion(unsigned long now){
  if(now - lastMove < 1000) return; // Move only every 1 second
  lastMove = now;                    // Update last move time
  phase = !phase;                    // Toggle phase for alternate motion
  leftHand.write(phase ? 70 : 110);  // Left hand moves left-right
  rightHand.write(phase ? 110 : 70); // Right hand moves opposite
}

// Happy motion (small up/down wave)
static void happyMotion(unsigned long now){
  if(now - lastMove < 800) return; // Move every 0.8 seconds
  lastMove = now;
  phase = !phase;
  leftHand.write(phase ? 85 : 95);  
  rightHand.write(phase ? 95 : 85); 
}

// Sad motion (slow small movements)
static void sadMotion(unsigned long now){
  if(now - lastMove < 1500) return; // Move every 1.5 seconds
  lastMove = now;
  phase = !phase;
  leftHand.write(phase ? 92 : 88);  
  rightHand.write(phase ? 88 : 92); 
}

// Surprised motion (single pose)
static void surprisedMotion(unsigned long now){
  if(phase) return;             // Only set once until emotion changes
  leftHand.write(75);           // Left hand slightly up
  rightHand.write(105);         // Right hand slightly up
  phase = true;                 // Mark motion done
}

// Flirty motion (playful back-and-forth)
static void flirtyMotion(unsigned long now){
  if(now - lastMove < 1200) return; // Move every 1.2 seconds
  lastMove = now;
  phase = !phase;
  leftHand.write(phase ? 88 : 92);  
  rightHand.write(phase ? 98 : 82); 
}

// Bored motion (relaxed, slightly apart)
static void boredMotion(unsigned long now){
  if(phase) return;             // Only set once
  leftHand.write(89);           // Slightly off center
  rightHand.write(91);          // Slightly off center
  phase = true;                 // Mark motion done
}

// ===================== SETUP =====================
void handSetup(){
  leftHand.setPeriodHertz(50);  // Standard servo refresh rate
  rightHand.setPeriodHertz(50);

  leftHand.attach(LEFT_HAND_PIN, 500, 2400);  // Attach servo to pin with min/max pulse width
  rightHand.attach(RIGHT_HAND_PIN, 500, 2400);

  neutral(); // Start hands in neutral position
}

// ===================== UPDATE HANDS =====================
void handUpdate(){
  unsigned long now = millis();   // Current time
  Emotion e = emotionGet();       // Get current emotion

  // Reset motion when emotion changes
  if (e != lastEmotion) {
    phase = false;      // Reset phase
    lastMove = 0;       // Reset last move timer
    lastEmotion = e;    // Remember new emotion
  }

  // Call the appropriate motion function based on current emotion
  switch(e){
    case EMO_ANGRY:     angryMotion(now); break;
    case EMO_HAPPY:     happyMotion(now); break;
    case EMO_SAD:       sadMotion(now); break;
    case EMO_SURPRISED: surprisedMotion(now); break;  // Hold pose once
    case EMO_FLIRTY:    flirtyMotion(now); break;
    case EMO_BORED:     boredMotion(now); break;      // Hold pose once
    default:            neutral(); break;             // Default relaxed
  }
}