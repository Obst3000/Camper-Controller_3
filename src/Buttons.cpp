#include "Buttons.h"
#include "Config.h"
#include "Lighting.h"

// Button subsystem
// - Initializes AceButton instances for configured pins
// - Uses a single event handler `handleButton` to translate button events
//   into lighting actions (toggle, dim, auto-off)

// Array of allocated AceButton objects for each configured button pin
AceButton *button_instances[BUTTON_COUNT];

// Initialize button pins and AceButton objects, and configure features.
// Call once at startup.
void buttonsInit()
{
    for (int i = 0; i < BUTTON_COUNT; i++)
    {
        pinMode(BUTTON_PINS[i], INPUT_PULLUP);
        button_instances[i] = new AceButton(BUTTON_PINS[i]);
    }

    ButtonConfig *cfg = ButtonConfig::getSystemButtonConfig();
    cfg->setEventHandler(handleButton);
    cfg->setFeature(ButtonConfig::kFeatureClick);
    cfg->setFeature(ButtonConfig::kFeatureDoubleClick);
    cfg->setFeature(ButtonConfig::kFeatureLongPress);
    cfg->setLongPressDelay(200); // milliseconds
}

// Poll buttons; should be called frequently from the main loop.
void buttonsUpdate()
{
    for (int i = 0; i < BUTTON_COUNT; i++)
        button_instances[i]->check();
}

// Central event handler for all button events.
// eventType: AceButton::kEventClicked, kEventDoubleClicked, kEventLongPressed, kEventReleased
// state: not used here but provided by AceButton API
void handleButton(AceButton *btn, uint8_t eventType, uint8_t state)
{
    int pin = btn->getPin();
    int zone = -1;

    // Map pin to zone index
    for (int i = 0; i < BUTTON_COUNT; i++)
        if (BUTTON_PINS[i] == pin)
        {
            zone = i;
        }
    if (zone < 0)
        return; // unknown pin

    switch (eventType)
    {
    case AceButton::kEventClicked:
        // Single click: toggle or mirror depending on the button
        switch (pin)
        {
        case RearBedPin:
            // Toggle both rear LEDs and ensure right matches left
            led_right_rear.setTarget(led_left_rear.getTarget());
            led_left_rear.toggle();
            led_right_rear.toggle();
            Serial.println("Toggle Rear:" + String(led_left_rear.getTarget()));
            break;

        case FrontBedPin:
            // Toggle left front and mirror to right front
            led_left_front.toggle();
            led_right_front.setTarget(led_left_front.getTarget());
            Serial.println("Toggle Front:" + String(led_left_front.getTarget()));
            break;

        case LeftKitchenPin:
            // Toggle a group: front + rear on the left side, mirror to right
            led_right_rear.setTarget(led_left_rear.getTarget());
            led_right_front.setTarget(led_left_rear.getTarget());
            led_left_front.setTarget(led_left_rear.getTarget());

            led_left_rear.toggle();
            led_left_front.toggle();
            led_right_rear.toggle();
            led_right_front.toggle();
            Serial.println("Toggle Front+Rear:" + String(led_left_rear.getTarget()));
            break;

        case RightKitchenPin:
            // Toggle kitchen light and test auto-off by toggling luggage light
            led_kitchen.toggle();
            Serial.println("Toggle Kitchen:" + String(led_kitchen.getTarget()));

            // Test auto-off: toggle luggage and set 10s timer
            led_left_rear.setTarget(100);
            led_left_rear.startTimer(10); // turn off after 10 seconds
            break;

        case LuggagePin:
            // Toggle luggage light with 10s auto-off
            led_luggage.toggle();
            led_luggage.startTimer(10);
            Serial.println("Toggle Luggage:" + String(led_luggage.getTarget()));
            break;

        case SideDoorPin:
            led_side_door.setTarget(100);
            led_side_door.startTimer(30); // turn off after 10 seconds
            // Side door currently unused — reserved for future action
            break;

        default:
            break;
        }

        Serial.println("Button " + String(zone) + " Clicked!");
        break;

    case AceButton::kEventDoubleClicked:
        // Double click: switch all lights off
        Serial.println("Button " + String(zone) + " DoubleClicked!");
        lightingSetAllOff();
        Serial.println("Set All to 0");
        break;

    case AceButton::kEventLongPressed:
        // Long press: start dimming the respective zone(s)
        Serial.println("Button " + String(zone) + " Long Pressed!");
        switch (pin)
        {
        case RearBedPin:
            // Start dimming both rear sides together
            led_right_rear.setTarget(led_left_rear.getTarget()); // Equalize both sides
            led_left_rear.startDimming();
            led_right_rear.startDimming();
            Serial.println("Started Dimming Rear LEDs:" + String(led_left_rear.getTarget()));
            break;

        case FrontBedPin:
            // Start dimming front pair
            led_right_front.setTarget(led_left_front.getTarget());
            led_left_front.startDimming();
            led_right_front.startDimming();
            Serial.println("Started Dimming Front LEDs:" + String(led_left_front.getTarget()));
            break;

        case LeftKitchenPin:
            // Start dimming main group (front+rear)
            led_right_rear.setTarget(led_left_rear.getTarget());
            led_right_front.setTarget(led_left_front.getTarget());
            led_left_rear.startDimming();
            led_right_rear.startDimming();
            led_left_front.startDimming();
            led_right_front.startDimming();
            Serial.println("Started Dimming Main LEDs:" + String(led_left_front.getTarget()));
            break;

        case RightKitchenPin:
            // Start dimming kitchen light
            led_kitchen.startDimming();
            Serial.println("Started Dimming Kitchen LEDs:" + String(led_kitchen.getTarget()));
            break;

        case LuggagePin:
            // Start dimming luggage light
            led_luggage.startDimming();
            Serial.println("Started Dimming Luggage LEDs:" + String(led_luggage.getTarget()));
            break;

        case SideDoorPin:
            // reserved
            break;

        default:
            break;
        }
        break;

    case AceButton::kEventReleased:
        // Release after long press -> stop dimming
        Serial.println("Button " + String(zone) + " Long Released!");
        switch (pin)
        {
        case RearBedPin:
            led_right_rear.setTarget(led_left_rear.getTarget()); // Equalize both sides
            led_left_rear.endDimming();
            led_right_rear.endDimming();
            Serial.println("Ended Dimming Rear LEDs:" + String(led_left_rear.getTarget()));
            break;

        case FrontBedPin:
            led_right_front.setTarget(led_left_front.getTarget());
            led_left_front.endDimming();
            led_right_front.endDimming();
            Serial.println("Ended Dimming Front LEDs:" + String(led_left_front.getTarget()));
            break;

        case LeftKitchenPin:
            led_right_rear.setTarget(led_left_rear.getTarget());
            led_right_front.setTarget(led_left_front.getTarget());
            led_left_rear.endDimming();
            led_right_rear.endDimming();
            led_left_front.endDimming();
            led_right_front.endDimming();
            Serial.println("Ended Dimming Main LEDs:" + String(led_left_rear.getTarget()));
            break;

        case RightKitchenPin:
            led_kitchen.endDimming();
            Serial.println("Ended Dimming Kitchen LEDs:" + String(led_kitchen.getTarget()));
            break;

        case LuggagePin:
            led_luggage.endDimming();
            Serial.println("Ended Dimming Luggage LEDs:" + String(led_luggage.getTarget()));
            break;

        case SideDoorPin:
            // reserved
            break;

        default:
            break;
        }
        break;

    default:
        // Unhandled event types are ignored
        break;
    }
}
