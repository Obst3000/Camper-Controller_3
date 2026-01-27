#include "Buttons.h"
#include "Config.h"
#include "Lighting.h"

AceButton *buttons[BUTTON_COUNT];

void handleButton(AceButton *, uint8_t, uint8_t);

void buttonsInit()
{
    for (int i = 0; i < BUTTON_COUNT; i++)
    {
        pinMode(BUTTON_PINS[i], INPUT_PULLUP);
        buttons[i] = new AceButton(BUTTON_PINS[i]);
    }

    ButtonConfig *cfg = ButtonConfig::getSystemButtonConfig();
    cfg->setEventHandler(handleButton);
    cfg->setFeature(ButtonConfig::kFeatureClick);
    cfg->setFeature(ButtonConfig::kFeatureDoubleClick);
    cfg->setFeature(ButtonConfig::kFeatureLongPress);
    // cfg->setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);
}

// Test

void buttonsUpdate()
{
    for (int i = 0; i < BUTTON_COUNT; i++)
        buttons[i]->check();
}

void handleButton(AceButton *btn, uint8_t eventType, uint8_t state)
{
    int pin = btn->getPin();
    int zone = -1;

    // Serial.println("Button Event Detected!");
    for (int i = 0; i < BUTTON_COUNT; i++)
        if (BUTTON_PINS[i] == pin)
        {
            zone = i;
        }
    if (zone < 0)
        return;

    switch (eventType)
    {
    case AceButton::kEventClicked:
        switch (pin)
        {
        case RearBedPin:
            LeftRearLed.toggle();
            //RightRearLed.setTarget(LeftRearLed.getTarget());
            //LeftRearLed.setTarget(0);
            Serial.println("Toggle Rear");

            break;
        case FrontBedPin:
            LeftFrontLed.toggle();
            RightFrontLed.setTarget(LeftFrontLed.getTarget());

            Serial.println("Toggle Front");
            break;
        case LeftKitchenPin:
            RightRearLed.setTarget(LeftRearLed.getTarget());
            RightFrontLed.setTarget(LeftRearLed.getTarget());
            LeftFrontLed.setTarget(LeftRearLed.getTarget());

            LeftRearLed.toggle();
            LeftFrontLed.toggle();
            RightRearLed.toggle();
            RightFrontLed.toggle();

            Serial.println("Toggle Front+Rear");
            break;
        case RightKitchenPin:
            KitchenLed.toggle();

            Serial.println("Toggle Kitchen");
            break;
        case LuggagePin:
            LuggageLed.toggle();

            Serial.println("Toggle Luggage");
            break;
        case SideDoorPin:

            /* code */
            break;

        default:
            break;
        }

        Serial.println("Button " + String(zone) + " Clicked!");
        // lightingSetTarget(zone, lightingGetCurrent(zone) == 0 ? 200 : 0);
        // LeftRearLed.setTarget(255); // Example of direct access

        // lightingSetAllOn();
        break;

    case AceButton::kEventDoubleClicked:
        Serial.println("Button " + String(zone) + " DoubleClicked!");
        lightingSetAllOff();
        // LeftRearLed.setTarget(0);
        Serial.println("Set All to 0");
        break;

    case AceButton::kEventLongPressed:
        Serial.println("Button " + String(zone) + " Long Pressed!");
        switch (pin)
        {
        case RearBedPin:
            RightRearLed.setTarget(LeftRearLed.getTarget()); // Equalize both sides

            LeftRearLed.startDimming();
            RightRearLed.startDimming();

            break;
        case FrontBedPin:
            RightFrontLed.setTarget(LeftFrontLed.getTarget()); // Equalize both sides

            LeftFrontLed.startDimming();
            RightFrontLed.startDimming();
            break;
        case LeftKitchenPin:
            RightRearLed.setTarget(LeftRearLed.getTarget());   // Equalize both sides
            RightFrontLed.setTarget(LeftFrontLed.getTarget()); // Equalize both sides

            LeftRearLed.startDimming();
            RightRearLed.startDimming();
            LeftFrontLed.startDimming();
            RightFrontLed.startDimming();
            break;
        case RightKitchenPin:
            KitchenLed.startDimming();
            break;
        case LuggagePin:
            /* code */
            LuggageLed.startDimming();
            break;
        case SideDoorPin:

            /* code */
            break;

        default:
            break;
        }
        break;
    case AceButton::kEventReleased:
        Serial.println("Button " + String(zone) + " Long Released!");
        switch (pin)
        {
        case RearBedPin:
            RightRearLed.setTarget(LeftRearLed.getTarget()); // Equalize both sides

            LeftRearLed.endDimming();
            RightRearLed.endDimming();

            break;
        case FrontBedPin:
            RightFrontLed.setTarget(LeftFrontLed.getTarget()); // Equalize both sides

            LeftFrontLed.endDimming();
            RightFrontLed.endDimming();
            break;
        case LeftKitchenPin:
            RightRearLed.setTarget(LeftRearLed.getTarget());   // Equalize both sides
            RightFrontLed.setTarget(LeftFrontLed.getTarget()); // Equalize both sides

            LeftRearLed.endDimming();
            RightRearLed.endDimming();
            LeftFrontLed.endDimming();
            RightFrontLed.endDimming();
            break;
        case RightKitchenPin:
            KitchenLed.endDimming();
            break;
        case LuggagePin:
            /* code */
            LuggageLed.endDimming();
            break;
        case SideDoorPin:

            /* code */
            break;

        default:
            break;
        }
        break;
        break;
    }
}
