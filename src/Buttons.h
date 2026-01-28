#pragma once
#include <AceButton.h>
using namespace ace_button;

// Initialize button hardware and attach AceButton handlers.
void buttonsInit();

// Poll/update button state; should be called regularly from the main loop.
void buttonsUpdate();

// Event handler used by AceButton. Declared here for clarity; definition
// lives in Buttons.cpp.
void handleButton(AceButton* btn, uint8_t eventType, uint8_t buttonState);
