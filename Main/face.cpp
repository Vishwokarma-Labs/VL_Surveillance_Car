#include "face.h"      // Include face function declarations
#include "emotion.h"   // Include emotion system

// Forward declarations of specific face drawing functions
// These are defined elsewhere, but we declare them so we can use them here
void drawHappy();
void drawSad();
void drawAngry();
void drawSurprised();
void drawBored();
void drawFlirty();
void drawNeutral();

// ===================== DRAW FACE FUNCTION =====================
void faceDraw() {
  
  Emotion e = emotionGet();  
  // Get the current emotion from the emotion system

  // ===================== DEBUG PRINT =====================
  // This prints to Serial monitor which face is being drawn
  static Emotion lastEmotion = (Emotion)-1;  
  // Stores the last emotion drawn to avoid printing repeatedly

  if (e != lastEmotion) {
    Serial.print("📺 Drawing face: ");
    switch(e) {
      case EMO_NEUTRAL: Serial.println("NEUTRAL"); break;
      case EMO_HAPPY: Serial.println("HAPPY"); break;
      case EMO_SAD: Serial.println("SAD"); break;
      case EMO_ANGRY: Serial.println("ANGRY"); break;
      case EMO_SURPRISED: Serial.println("SURPRISED"); break;
      case EMO_BORED: Serial.println("BORED"); break;
      case EMO_FLIRTY: Serial.println("FLIRTY"); break;
      default: Serial.println("UNKNOWN!"); break;
    }
    lastEmotion = e;  // Update the last drawn emotion
  }

  // ===================== CALL THE CORRECT DRAW FUNCTION =====================
  switch (e) {
    case EMO_NEUTRAL:    
      drawNeutral();  // Draw neutral face
      break;
      
    case EMO_HAPPY:      
      drawHappy();    // Draw happy face
      break;
      
    case EMO_SAD:        
      drawSad();      // Draw sad face
      break;
      
    case EMO_ANGRY:      
      drawAngry();    // Draw angry face
      break;
      
    case EMO_SURPRISED:  
      drawSurprised();// Draw surprised face
      break;
      
    case EMO_BORED:      
      drawBored();    // Draw bored face
      break;
      
    case EMO_FLIRTY:     
      drawFlirty();   // Draw flirty face
      break;
      
    default:
      Serial.println("⚠️ Unknown emotion, drawing neutral");
      drawNeutral();  // If unknown emotion, default to neutral
      break;
  }
}