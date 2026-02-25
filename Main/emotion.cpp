#include "emotion.h"   // Includes Emotion enum and function declarations
#include <Arduino.h>   // Standard Arduino library (for millis() function)

// ===================== VARIABLES =====================

// Keeps track of the current emotion of the robot
static Emotion currentEmotion = EMO_NEUTRAL;

// Stores the time of the last interaction with the robot
static unsigned long lastInteraction = 0;

// ===================== CONSTANTS =====================

// Time in milliseconds before robot goes idle (15 seconds)
#define IDLE_TIMEOUT 15000  

// ===================== SET EMOTION =====================
void emotionSet(Emotion e){
  currentEmotion = e;       // Change the current emotion to 'e'
  lastInteraction = millis();  // Reset idle timer to now
}

// ===================== GET CURRENT EMOTION =====================
Emotion emotionGet(){
  return currentEmotion;    // Return the current emotion
}

// ===================== IDLE UPDATE =====================
// Called repeatedly in the main loop
void emotionIdleUpdate(){
  // If robot is happy but no interaction for IDLE_TIMEOUT
  if(currentEmotion == EMO_HAPPY){
    if(millis() - lastInteraction > IDLE_TIMEOUT){
      currentEmotion = EMO_BORED;  // Switch to bored expression
    }
  }
}

// ===================== MARK ACTIVE =====================
// Called whenever user interacts with robot (voice, button, etc.)
void emotionMarkActive(){
  lastInteraction = millis();  // Reset idle timer
}