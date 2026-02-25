#pragma once
#include <Arduino.h>

// ===================== HAND POSES =====================
// These are the different hand positions we can set based on the robot's emotion
enum HandPose {
  HAND_NEUTRAL,    // Hands relaxed in the middle
  HAND_HAPPY,      // Small up/down waving hands
  HAND_SAD,        // Slow, subtle hand movement
  HAND_ANGRY,      // Fast alternating hands
  HAND_SURPRISED,  // Hands up in a surprised pose
  HAND_BORED,      // Slightly apart, relaxed hands
  HAND_FLIRTY      // Playful, alternating hands
};

// ===================== FUNCTION DECLARATIONS =====================

// Call this once in setup() to initialize servos
void handSetup();

// Call this repeatedly in loop() to update hand movements
void handUpdate();

// Directly set a specific hand pose (if needed)
void handSetPose(HandPose pose);