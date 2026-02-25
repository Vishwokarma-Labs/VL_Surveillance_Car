#include "face.h"       // Include face drawing functions
#include "animation.h"  // Include animation variables

// ===================== DRAW FLIRTY FACE =====================
void drawFlirty() {

  // ===================== WINK CONTROL =====================
  static bool wink = false;             // Track if left eye is winking
  static unsigned long lastWink = 0;   // Time when last wink started
  unsigned long now = millis();         // Current time in milliseconds

  // Trigger a wink occasionally (every 4-6.5 seconds)
  if (!wink && now - lastWink > random(4000, 6500)) {
    wink = true;        // start wink
    lastWink = now;     // reset timer
  }

  // End the wink after 250 ms
  if (wink && now - lastWink > 250) {
    wink = false;
    lastWink = now;     // reset timer
  }

  // ===================== EYE POSITIONS =====================
  int lx = 42 + eyeX;   // left eye X
  int rx = 86 + eyeX;   // right eye X
  int y  = 30 + eyeY;   // both eyes Y

  // ===================== LEFT EYE =====================
  // Left eye is winking sometimes
  if (wink || isBlinking) {
    u8g2.drawLine(lx-10, y, lx+10, y);  // draw as closed line
  } else {
    u8g2.drawCircle(lx, y, 10);                 // outer circle
    u8g2.drawDisc(lx + pupilX, y + pupilY, 3); // pupil
  }

  // ===================== RIGHT EYE =====================
  // Always open unless blinking
  if (isBlinking) {
    u8g2.drawLine(rx-10, y, rx+10, y);  // closed
  } else {
    u8g2.drawCircle(rx, y, 10);                 // outer circle
    u8g2.drawDisc(rx + pupilX, y + pupilY, 3); // pupil
  }

  // ===================== BROWS =====================
  // Playful, slightly lifted brows
  int b = browOffset - 2;  // adjust upward slightly

  // Left brow
  u8g2.drawLine(lx - 10, 20 + b, lx, 14 + b);
  u8g2.drawLine(lx, 14 + b, lx + 10, 20 + b);

  // Right brow
  u8g2.drawLine(rx - 10, 20 + b, rx - 3, 16 + b);
  u8g2.drawLine(rx - 3, 16 + b, rx + 3, 16 + b);
  u8g2.drawLine(rx + 3, 16 + b, rx + 10, 20 + b);

  // ===================== MOUTH =====================
  // Cute smirk for flirty expression
  int m = mouthOffset;
  u8g2.drawLine(56, 56 + m, 64, 58 + m);  // left part of smirk
  u8g2.drawLine(64, 58 + m, 72, 54 + m);  // right part of smirk
}