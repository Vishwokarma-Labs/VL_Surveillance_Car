#include "face.h"       // Include face function declarations
#include "animation.h"  // Include animation variables (eye, brow, mouth positions)

// ===================== DRAW ANGRY FACE =====================
void drawAngry() {
  
  // Calculate the positions of the left and right eyes
  int lx = 42 + eyeX;  // left eye X position
  int rx = 86 + eyeX;  // right eye X position
  int y  = 30 + eyeY;  // both eyes Y position

  // ===================== DRAW EYES =====================
  // If the robot is blinking, draw straight lines instead of circles
  if (isBlinking) {
    u8g2.drawLine(lx-10, y, lx+10, y);  // left eye closed
    u8g2.drawLine(rx-10, y, rx+10, y);  // right eye closed
  } else {
    // Eyes OPEN
    u8g2.drawCircle(lx, y, 10);                // left eye outer circle
    u8g2.drawCircle(rx, y, 10);                // right eye outer circle
    u8g2.drawDisc(lx + pupilX, y + pupilY, 3); // left pupil
    u8g2.drawDisc(rx + pupilX, y + pupilY, 3); // right pupil
  }

  // ===================== DRAW ANGRY BROWS =====================
  // Angry eyebrows slanted downward toward the center
  int b = browOffset;  // use dynamic brow offset from animation

  u8g2.drawLine(lx - 10, 14 + b, lx + 6, 20 + b);  // left brow line
  u8g2.drawLine(rx + 10, 14 + b, rx - 6, 20 + b);  // right brow line

  // ===================== DRAW ANGRY MOUTH =====================
  // Tight mouth curved slightly down
  int m = mouthOffset;  // use dynamic mouth offset from animation
  u8g2.drawLine(52, 56 + m, 60, 54 + m);  // left part of mouth
  u8g2.drawLine(60, 54 + m, 68, 54 + m);  // middle part of mouth
  u8g2.drawLine(68, 54 + m, 76, 56 + m);  // right part of mouth
}