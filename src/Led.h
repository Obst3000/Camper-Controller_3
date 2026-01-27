#pragma once
#include <Arduino.h>

// Lightweight LED object to represent a single controllable LED strip
class Led {
public:

    Led(int pin, int channel, uint8_t initial=0);

    

    int getPin() const;
    int getChannel() const;

    uint8_t getTarget() const;
    uint8_t getActual() const;

    void toggle();
    void startDimming();
    void endDimming();

    void setTarget(uint8_t v);
    void setActual(uint8_t v);

    void initLed();
    // Move actual value toward target by up to step and apply to hardware
    void update(uint8_t step);
    // Write the current actual value to the hardware PWM channel
    void apply() const;


private:
    int _pin;
    int _channel;
    uint8_t _target;
    uint8_t _actual;

    bool dimming;
    bool dimmingdirection;
};
