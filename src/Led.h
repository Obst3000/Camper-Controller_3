#pragma once
#include <Arduino.h>

// Lightweight LED object representing a single controllable LED strip.
// Provides: target/actual value tracking, smooth fading, simple dimming
// helpers and an optional auto-off timer.
class Led {
public:


    // Construct a Led bound to `pin` and PWM `channel` with optional
    // initial brightness (0-255).
    Led(int pin, int channel, uint8_t initial=0);

    // Accessors
    int getPin() const;
    int getChannel() const;

    uint8_t getTarget() const;
    uint8_t getActual() const;

    // Simple controls
    void toggle();            // toggle between off and `_defaultValue`
    void startDimming();      // begin continuous dimming (direction toggles)
    void endDimming();        // stop dimming

    void setTarget(uint8_t v);
    void setActual(uint8_t v);

    // Hardware initialization (attach PWM channel to pin)
    void initLed();

    // Start a timer (seconds) after which `_target` is forced to 0.
    // Call with `0` to cancel any running timer.
    void startTimer(uint32_t seconds);

    // Progress `_actual` toward `_target` by up to `step` and write
    // the resulting value to hardware.
    void update(uint8_t step);

    // Write the current `_actual` value to the PWM hardware.
    void apply();


private:
    int _pin;
    int _channel;
    uint8_t _target;
    uint8_t _actual;

    // If non-zero, the absolute millis() timestamp when the target will be
    // forced to 0. `_targetOffAt == 0` means no timer is armed.
    uint32_t _targetOffAt = 0;
    // default auto-off delay in milliseconds (used when apply() arms timer)
    uint32_t _defaultOffAt = 18UL * 60UL * 60UL * 1000UL; // 18 hours by default

    // Dimming state
    bool dimming = false;
    bool dimmingdirection = false;

    // Display/value limits and helpers
    int _defaultValue = 50;
    int _lastValue = 50;
    int _maxValue = 100;
};
