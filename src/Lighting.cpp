#include "Lighting.h"
#include "Led.h"

// Lighting implementation
// - Implements `Led` methods (constructor, update/apply, dimming, timer)
// - Declares global `led_*` instances and `leds[]` pointer array
// - Provides simple helpers: `lightingUpdate`, `lightingSetAllOn/Off`

// --- Led class implementation ---
Led::Led(int pin, int channel, uint8_t initial)
    : _pin(pin), _channel(channel), _target(initial), _actual(initial), _targetOffAt(0)
{
    initLed();
}

int Led::getPin() const { return _pin; }
int Led::getChannel() const { return _channel; }

uint8_t Led::getTarget() const { return _target; }
uint8_t Led::getActual() const { return _actual; }

void Led::setTarget(uint8_t v) { _target = v; }
void Led::setActual(uint8_t v) { _actual = v; }

void Led::initLed()
{
    ledcSetup(_channel, PWM_FREQ, PWM_RES);
    ledcAttachPin(_pin, _channel);
}

void Led::startTimer(uint32_t s)
{
    if (s == 0) {
        _targetOffAt = 0;
    } else {
        _targetOffAt = millis() + s*1000;
    }
}
void Led::toggle()
{
    if (_target > 0)
    {
        _lastValue = _target;
        _target = 0;

        _targetOffAt = 0; // cancel any running timer
    }
    else
    {
        _target = _defaultValue;
    }

    
}
void Led::startDimming()
{
    dimmingdirection = !dimmingdirection;
    dimming = true;
}
void Led::endDimming()
{
    dimming = false;
}

void Led::update(uint8_t step)
{
    // If a timer to switch off was set and expired, force target to 0
    if (_targetOffAt != 0 && millis() >= _targetOffAt) {
        _target = 0;
        _targetOffAt = 0;
    }

     if (dimming)
        {
            int d = 1; // dimming step size
            if (dimmingdirection)
            {
                if (_target + d > _maxValue)
                {
                    _target = _maxValue;
                }
                else
                {
                    _target += d;
                }
            }
            else
            {
                if (_target < d)
                {
                    _target = 0;
                }
                else
                {
                    _target -= d;
                }
            }

            _actual = _target;
                apply();
                return;
        }

        
        if (_actual == _target)
            return;
        if (_actual < _target)
        {

            if ((_actual + step) < _target)
            {
                _actual = _actual + step;
            }
            else
            {
                _actual = _target;
            }
            
        }
        else
        {
            // uint8_t next = _actual - step;
            if ((_actual - step) > _target)
            {

                _actual = _actual - step;
            }
            else
            {
                _actual = _target; // prevent underflow
            }
        }
        apply();

}

void Led::apply()  // gets jammed if called to often
{
    if (_channel < 0)
        return;

    // output range and compensation settings
     int outMin = 0;
     int outMax = 255;
     bool compensateLinearity = true; // set to false to use linear mapping
     float gamma = 2.8f;              // >1 darkens low end, <1 brightens low end

    float maxIn = (_maxValue > 0) ? float(_maxValue) : 100.0f;
    float inNorm = float(_actual) / maxIn;
    if (inNorm <= 0.0f)
    {
        ledcWrite(_channel, outMin);
        return;
    }
    if (inNorm >= 1.0f)
    {
        ledcWrite(_channel, outMax);
        return;
    }

    if (compensateLinearity)
        inNorm = powf(inNorm, gamma);

    int mapped = outMin + int(roundf(inNorm * float(outMax - outMin)));
    if (mapped < outMin) mapped = outMin;
    if (mapped > outMax) mapped = outMax;

    ledcWrite(_channel, mapped);

    if (_targetOffAt == 0)//Start default turn off timer if not already running
    {
       startTimer(_defaultOffAt);
    }
    
    

}

// Define one Led object per strip and expose the array (matches extern in Config.h)
// Global Led instances (renamed to snake_case with `led_` prefix)
Led led_left_rear(LeftRearLedPin, 0);
Led led_right_rear(RightRearLedPin, 1);
Led led_left_front(LeftFrontLedPin, 2);
Led led_right_front(RightFrontLedPin, 3);
Led led_kitchen(KitchenLedPin, 4);
Led led_luggage(LuggageLedPin, 5);
Led led_side_door(SideDoorLedPin, 6);

// Array of pointers to the existing Led instances - preserves identity
Led* leds[ZONE_COUNT] = {
    &led_left_rear,
    &led_right_rear,
    &led_left_front,
    &led_right_front,
    &led_kitchen,
    &led_luggage,
    &led_side_door
};

const uint16_t FADE_STEP_MS = 15;
const uint8_t FADE_STEP = 2;
uint32_t lastFadeUpdate = 0;

void lightingSetAllOff()
{
    for (int i = 0; i < ZONE_COUNT; ++i)
    {
        leds[i]->setTarget(0);
    }
}
void lightingSetAllOn()
{
    for (int i = 0; i < ZONE_COUNT; ++i)
    {
        leds[i]->setTarget(255);
    }
}

void lightingUpdate()
{
    uint32_t now = millis();
    if (now - lastFadeUpdate < FADE_STEP_MS)
        return;
    lastFadeUpdate = now;

    for (int i = 0; i < ZONE_COUNT; i++)
    {
        leds[i]->update(FADE_STEP);
    }
}
