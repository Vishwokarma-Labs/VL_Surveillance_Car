// face.h                // This is a header file for face-related functions

#pragma once            // Prevents this file from being included multiple times (avoids errors)

#include <U8g2lib.h>    // Includes the U8g2 graphics library (used to draw on OLED display)


// This tells other files that the display object exists somewhere else
// "extern" means: the real object is defined in another file, not here
extern U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;


// ===================== FACE DRAWING FUNCTION DECLARATIONS =====================

// These are function declarations (only names, not full code).
// They tell the compiler that these functions exist in another .cpp file.

// Main function that decides which face to draw
void faceDraw();

// Function to draw happy face
void drawHappy();

// Function to draw sad face
void drawSad();

// Function to draw angry face
void drawAngry();

// Function to draw surprised face
void drawSurprised();

// Function to draw bored face
void drawBored();

// Function to draw flirty face
void drawFlirty();

// Function to draw neutral face
void drawNeutral();