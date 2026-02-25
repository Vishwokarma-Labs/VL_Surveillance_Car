#pragma once
// Ensures this header file is only included once in the project
// Prevents duplicate definition errors

// ===================== EMOTION ENUM =====================
enum Emotion {
  EMO_NEUTRAL,    // Neutral expression (default face)
  EMO_HAPPY,      // Happy expression
  EMO_SAD,        // Sad expression
  EMO_ANGRY,      // Angry expression
  EMO_SURPRISED,  // Surprised expression
  EMO_BORED,      // Bored expression
  EMO_FLIRTY      // Flirty expression
};
// "enum" is short for enumeration. It gives names to numbers.
// For example, EMO_HAPPY might internally be 1, EMO_SAD = 2, etc.
// This makes code easier to read instead of using random numbers.

// ===================== FUNCTION DECLARATIONS =====================

// Sets the current emotion to 'e' (e.g., EMO_HAPPY)
void emotionSet(Emotion e);

// Gets the current emotion
Emotion emotionGet();

// Updates emotion when idle (e.g., small automatic animations while robot is doing nothing)
void emotionIdleUpdate();

// Marks the emotion as active (e.g., used to trigger certain behaviors or animations)
void emotionMarkActive();