#include "command.h"    // Includes command function declaration
#include "emotion.h"    // Includes emotion functions and Emotion enum

// ===================== COMMAND UPDATE FUNCTION =====================

void commandUpdate(){
  
  // If there is NO data coming from Serial (like from PC or voice system),
  // then stop here and do nothing
  if(!Serial.available()) return;

  // Read incoming text from Serial until a newline character '\n'
  // Example input: "EMOTION:HAPPY"
  String cmd = Serial.readStringUntil('\n');
  
  // Remove any extra spaces or newline characters from beginning/end
  cmd.trim();

  // Mark the emotion system as active
  // This likely prevents idle animation or resets idle timer
  emotionMarkActive();

  // ===================== CHECK COMMAND =====================
  // If the received command matches one of these,
  // set the corresponding emotion

  if(cmd=="EMOTION:NEUTRAL") emotionSet(EMO_NEUTRAL);

  if(cmd=="EMOTION:HAPPY") emotionSet(EMO_HAPPY);

  if(cmd=="EMOTION:SAD") emotionSet(EMO_SAD);

  if(cmd=="EMOTION:ANGRY") emotionSet(EMO_ANGRY);

  if(cmd=="EMOTION:SURPRISED") emotionSet(EMO_SURPRISED);

  if(cmd=="EMOTION:BORED") emotionSet(EMO_BORED);

  if(cmd=="EMOTION:FLIRTY") emotionSet(EMO_FLIRTY);
}