#include "face.h"       // Include face drawing functions
#include "animation.h"  // Include animation variables

// ===================== DRAW BORED FACE =====================
void drawBored() {

  // Calculate left and right eye positions
  int lx = 42 + eyeX;   // left eye X position
  int rx = 86 + eyeX;   // right eye X position
  int y  = 32 + eyeY;   // both eyes Y position (slightly lower than normal)

  // ===================== DRAW EYES =====================
  // Bored eyes are half-lidded, represented by straight horizontal lines
  u8g2.drawLine(lx - 10, y, lx + 10, y);  // left eye
  u8g2.drawLine(rx - 10, y, rx + 10, y);  // right eye

  // ===================== DRAW BROWS =====================
  // Bored brows are low and curved like a small ∩ (semi-circle)
  int b = browOffset + 4;  // offset for slight downward position

  // Left brow
  u8g2.drawLine(lx - 10, 22 + b, lx - 3, 20 + b);
  u8g2.drawLine(lx - 3, 20 + b, lx + 3, 20 + b);
  u8g2.drawLine(lx + 3, 20 + b, lx + 10, 22 + b);

  // Right brow
  u8g2.drawLine(rx - 10, 22 + b, rx - 3, 20 + b);
  u8g2.drawLine(rx - 3, 20 + b, rx + 3, 20 + b);
  u8g2.drawLine(rx + 3, 20 + b, rx + 10, 22 + b);

  // ===================== DRAW MOUTH =====================
  // Flat mouth, but moves slightly with animation (mouthOffset)
  int m = mouthOffset;
  u8g2.drawLine(56, 56 + m, 72, 56 + m);
}