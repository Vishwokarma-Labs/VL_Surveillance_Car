#include "face.h"       // Include face drawing functions
#include "animation.h"  // Include animation variables

// ----------------- SURPRISED FACE -----------------
void drawSurprised() {
  // ===================== EYE POSITIONS =====================
  int lx = 42 + eyeX;  // left eye X position
  int rx = 86 + eyeX;  // right eye X position
  int y  = 28 + eyeY;  // Y position for both eyes

  // ===================== EYES =====================
  // Surprised eyes are wide open, no blinking
  u8g2.drawCircle(lx, y, 11);                 // left eye outer circle
  u8g2.drawCircle(rx, y, 11);                 // right eye outer circle
  u8g2.drawDisc(lx + pupilX, y + pupilY, 3); // left pupil inside eye
  u8g2.drawDisc(rx + pupilX, y + pupilY, 3); // right pupil inside eye

  // ===================== EYEBROWS =====================
  int b = browOffset - 4; // raise eyebrows higher for surprise

  // Left eyebrow (high ∩ shape)
  u8g2.drawLine(lx - 10, 18 + b, lx - 3, 12 + b); // left slope
  u8g2.drawLine(lx - 3, 12 + b, lx + 3, 12 + b);  // top flat
  u8g2.drawLine(lx + 3, 12 + b, lx + 10, 18 + b); // right slope

  // Right eyebrow (high ∩ shape)
  u8g2.drawLine(rx - 10, 18 + b, rx - 3, 12 + b); // left slope
  u8g2.drawLine(rx - 3, 12 + b, rx + 3, 12 + b);  // top flat
  u8g2.drawLine(rx + 3, 12 + b, rx + 10, 18 + b); // right slope

  // ===================== MOUTH =====================
  // Surprised mouth is open circle "O"
  u8g2.drawCircle(64, 54, 6); // center X=64, Y=54, radius=6
}