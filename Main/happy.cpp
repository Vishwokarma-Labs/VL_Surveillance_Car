#include "face.h"       // Include face drawing functions
#include "animation.h"  // Include animation variables

// ===================== DRAW HAPPY FACE =====================
void drawHappy() {

  // ===================== EYE POSITIONS =====================
  int lx = 42 + eyeX;   // left eye X
  int rx = 86 + eyeX;   // right eye X
  int y  = 30 + eyeY;   // both eyes Y

  // ===================== DRAW EYES =====================
  // Eyes are big and sparkly unless blinking
  if (isBlinking) {
    // If blinking, draw as a horizontal line
    u8g2.drawLine(lx - 10, y, lx + 10, y);  // left eye
    u8g2.drawLine(rx - 10, y, rx + 10, y);  // right eye
  } else {
    // Open eyes with pupils
    u8g2.drawCircle(lx, y, 10);                 // left eye outer circle
    u8g2.drawCircle(rx, y, 10);                 // right eye outer circle
    u8g2.drawDisc(lx + pupilX, y + pupilY, 3); // left pupil
    u8g2.drawDisc(rx + pupilX, y + pupilY, 3); // right pupil
  }

  // ===================== BROWS =====================
  // Raised and curved upwards (∩ shape) to look happy
  int b = browOffset - 2;  // move slightly up

  // Left brow
  u8g2.drawLine(lx - 10, 20 + b, lx - 3, 16 + b);
  u8g2.drawLine(lx - 3, 16 + b, lx + 3, 16 + b);
  u8g2.drawLine(lx + 3, 16 + b, lx + 10, 20 + b);

  // Right brow
  u8g2.drawLine(rx - 10, 20 + b, rx - 3, 16 + b);
  u8g2.drawLine(rx - 3, 16 + b, rx + 3, 16 + b);
  u8g2.drawLine(rx + 3, 16 + b, rx + 10, 20 + b);

  // ===================== MOUTH =====================
  // Big happy smile, curved upward
  int m = mouthOffset;  // small vertical motion for animation

  u8g2.drawLine(48, 54 + m, 54, 58 + m);  // left curve
  u8g2.drawLine(54, 58 + m, 64, 60 + m);  // middle-left
  u8g2.drawLine(64, 60 + m, 74, 58 + m);  // middle-right
  u8g2.drawLine(74, 58 + m, 80, 54 + m);  // right curve
}