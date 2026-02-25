#pragma once
// Ensures this header file is included only once in the project
// Prevents duplicate definition errors

// Includes the configuration file
#include "config.h"  

// ===================== FUNCTION DECLARATION =====================

// Updates the robot's commands
// This function is likely called repeatedly to process input (like voice commands)
void commandUpdate();