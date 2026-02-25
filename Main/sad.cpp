#include "face.h"       // Include face drawing functions
#include "animation.h"  // Include animation variables

// ----------------- Helper: Cute Curved Brow -----------------
// Draws a thick "∩"-shaped eyebrow at center x (cx) and y position
static void drawCuteBrow(int cx, int y){
  u8g2.drawLine(cx-12, y+3, cx-6,  y);  // left slope
  u8g2.drawLine(cx-6,  y,   cx,    y-2); // middle peak
  u8g2.drawLine(cx,    y-2, cx+6,  y);  // right slope down
  u8g2.drawLine(cx+6,  y,   cx+12, y+3); // far right end
}

// ----------------- SAD FACE -----------------
void drawSad(){
  // ===================== EYE POSITIONS =====================
  int lx = 42 + eyeX;  // left eye X position
  int rx = 86 + eyeX;  // right eye X position
  int y  = 30 + eyeY;  // Y position for both eyes

  // ===================== EYES =====================
  if(isBlinking){
    // If blinking, eyes are horizontal lines
    u8g2.drawLine(lx-11, y, lx+11, y);  // left eye
    u8g2.drawLine(rx-11, y, rx+11, y);  // right eye
  } else {
    // Eyes open as big circles
    u8g2.drawCircle(lx, y, 11);                 // left eye outer circle
    u8g2.drawCircle(rx, y, 11);                 // right eye outer circle

    // Pupils inside eyes, move slightly
    u8g2.drawDisc(lx + pupilX, y + pupilY, 4); // left pupil
    u8g2.drawDisc(rx + pupilX, y + pupilY, 4); // right pupil
  }

  // ===================== EYEBROWS =====================
  // Draw sad, curved eyebrows using helper function
  drawCuteBrow(lx, 16 + browOffset); // left brow
  drawCuteBrow(rx, 16 + browOffset); // right brow

  // ===================== MOUTH =====================
  // Big U-shaped sad mouth, animated by mouthOffset
  int m = mouthOffset;

  u8g2.drawLine(48, 56+m, 54, 54+m);
  u8g2.drawLine(54, 54+m, 64, 53+m);
  u8g2.drawLine(64, 53+m, 74, 54+m);
  u8g2.drawLine(74, 54+m, 80, 56+m);
}