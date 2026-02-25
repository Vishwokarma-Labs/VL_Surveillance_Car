#include "face.h"       // Include face drawing functions
#include "animation.h"  // Include animation variables

/*
 * NEUTRAL / IDLE FACE
 * --------------------
 * - Eyes move slightly to look curious
 * - Natural blinking
 * - Brows relaxed and straight
 * - Mouth soft and neutral
 */

void drawNeutral() {

  // ===================== EYE POSITIONS =====================
  int lx = 42 + eyeX;  // left eye X position
  int rx = 86 + eyeX;  // right eye X position
  int y  = 30 + eyeY;  // Y position for both eyes

  // ===================== DRAW EYES =====================
  if (isBlinking) {
    // If blinking, eyes are just horizontal lines
    u8g2.drawLine(lx - 10, y, lx + 10, y);  // left eye
    u8g2.drawLine(rx - 10, y, rx + 10, y);  // right eye
  } else {
    // Eyes open as circles
    u8g2.drawCircle(lx, y, 10);                 // left eye outer circle
    u8g2.drawCircle(rx, y, 10);                 // right eye outer circle

    // Pupils inside eyes, move slightly for curiosity
    u8g2.drawDisc(lx + pupilX, y + pupilY, 3); // left pupil
    u8g2.drawDisc(rx + pupilX, y + pupilY, 3); // right pupil
  }

  // ===================== EYEBROWS =====================
  // Straight and relaxed (not frowning or raised)
  int b = browOffset / 2;  // subtle vertical offset for slight movement

  u8g2.drawLine(lx - 8, 18 + b, lx + 8, 18 + b); // left brow
  u8g2.drawLine(rx - 8, 18 + b, rx + 8, 18 + b); // right brow

  // ===================== MOUTH =====================
  // Soft, straight neutral mouth
  int m = mouthOffset / 2; // subtle vertical motion for natural look

  u8g2.drawLine(56, 56 + m, 72, 56 + m); // neutral mouth line
}