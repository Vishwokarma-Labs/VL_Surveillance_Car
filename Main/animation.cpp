#include "animation.h"
#include <math.h>

int eyeX=0, eyeY=0;// Horizontal and vertical movement of the eyes
int pupilX=0, pupilY=0; // Horizontal and vertical movement of the pupils inside eyes
int browOffset=0; // Vertical movement of eyebrows
int mouthOffset=0; // Vertical movement of mouth
int headOffset=0;// Vertical movement of head (bobbing)

// ===================== BLINK / WINK STATES =====================
bool isBlinking=false;
bool isWinking=false;
// ===================== TIMERS =====================
// tEye: timer for eye movement
// tBlink: timer for next blink
// tWink: timer for next wink
static unsigned long tEye=0, tBlink=0, tWink=0;

// ===================== ANIMATION UPDATE FUNCTION =====================
void animUpdate(){
  unsigned long now = millis();
  // Get current time in milliseconds since program started

  // HEAD BOB (very visible)
  // Use sine wave to move head up and down smoothly
  // Dividing by 300 makes movement slower
  // Multiply by 3 to increase amplitude (how far it moves)
  headOffset = sin(now / 300.0) * 3;
  

  // EYE SOCKET MOVE (big enough to see)
   // Every ~0.8 seconds, randomly move eyes
  if(now - tEye > 800){
    eyeX = random(-4,5);// Horizontal movement (-4 to +4)
    eyeY = random(-3,4);// Vertical movement (-3 to +3)

    // pupils exaggerate socket motion
    pupilX = eyeX * 2;
    pupilY = eyeY * 2;

    tEye = now;// Reset eye movement timer
  }

  // EYEBROW FLOAT
  // Eyebrows move up/down slowly using sine wave for natural effect
  browOffset = sin(now / 350.0) * 3;

  // MOUTH FLOAT
  // Mouth moves up/down slowly using sine wave
  // Creates subtle talking or idle expression effect
  mouthOffset = sin(millis() * 0.004) * 2;


  // BLINK
  
  if(!isBlinking && now - tBlink > random(2500,4500)){
    // If not blinking, and enough time passed (2.5s to 4.5s), start blink
    isBlinking = true;
    tBlink = now;// Record blink start time
  }
  if(isBlinking && now - tBlink > 140){
    // Blink lasts 140 milliseconds, then stop
    isBlinking = false;
  }

  // WINK (for flirty)
  if(!isWinking && now - tWink > random(7000,12000)){
    // If not winking, and enough time passed (7s to 12s), start wink
    isWinking = true;
    tWink = now;// Record wink start time
  }
  if(isWinking && now - tWink > 350){
    // Wink lasts 350 milliseconds, then stop
    isWinking = false;
  }
}
