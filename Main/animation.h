#pragma once
// Ensures this header file is only included once in the project
// Prevents duplicate definition errors

#include <Arduino.h>  // Standard Arduino library (needed for basic types and functions)

// ===================== FUNCTION DECLARATION =====================

// Updates the animation values for the robot's face
// e.g., moves eyes, brows, mouth slightly each frame
void animUpdate();

// ===================== EXTERNAL VARIABLES =====================

// These variables are defined in another file, but we want to use them here:

extern int eyeX, eyeY;        // How much the eyes move horizontally (X) and vertically (Y)
extern int pupilX, pupilY;    // How much the pupils move inside the eyes
extern int browOffset;        // Vertical movement of the eyebrows
extern int mouthOffset;       // Vertical movement of the mouth
extern int headOffset;        // Vertical movement of the head (for tilting, bobbing, etc.)

extern bool isBlinking;       // True when robot is blinking
extern bool isWinking;        // True when robot is winking (one eye only)