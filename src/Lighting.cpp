#include "Lighting.h"
#include "Led.h"

// --- Led class implementation ---
Led::Led(int pin, int channel, uint8_t initial)
    : _pin(pin), _channel(channel), _target(initial), _actual(initial)
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
void Led::toggle()
{
    if (_target > 0)
    {
        //  setTarget(0);
        //  setActual(0);
        _target = 0;
    }
    else
    {
        // setTarget(255);
        //  setActual(255);
        _target = 255;
    }
    //_target=255;
    //  _actual=0;
    //_target=200;
    //  _actual = _target; // prevent underflow

     //apply();
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
    if (false)
    {

       

    }

     if (dimming)
        {
            int d = 2;
            if (dimmingdirection)
            {
                if (_target + d > 255)
                {
                    _target = 255;
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
    // if (_actual == _target)
    // {

    //     return;
    // }
    // else
    // {
    //     _actual = _target;
    //     apply();
    // }

    //_actual = _target; // prevent underflow
}

void Led::apply() const // gets jammed if called to often
{
    if (_channel >= 0)
    {
        ledcWrite(_channel, _actual);
    }
}

// Define one Led object per strip and expose the array (matches extern in Config.h)
Led LeftRearLed(LeftRearLedPin, 0);
Led RightRearLed(RightRearLedPin, 1, 100);
Led LeftFrontLed(LeftFrontLedPin, 2, 100);
Led RightFrontLed(RightFrontLedPin, 3, 100);
Led KitchenLed(KitchenLedPin, 4, 100);
Led LuggageLed(LuggageLedPin, 5, 100);
Led SideDoorLed(SideDoorLedPin, 6, 100);

// Led leds[ZONE_COUNT] = {
//     LeftRearLed,
//     RightRearLed,
//     LeftFrontLed,
//     RightFrontLed,
//     KitchenLed,
//     LuggageLed,
//     SideDoorLed};

const uint16_t FADE_STEP_MS = 15;
const uint8_t FADE_STEP = 5;
uint32_t lastFadeUpdate = 0;

void lightingSetAllOff()
{
    for (int i = 0; i < ZONE_COUNT; ++i)
    {
        // eds[i].setTarget(0);
    }
}
void lightingSetAllOn()
{
    for (int i = 0; i < ZONE_COUNT; ++i)
    {
        // leds[i].setTarget(50);
    }
}

void lightingUpdate()
{
    uint32_t now = millis();
    if (now - lastFadeUpdate < FADE_STEP_MS)
        return;
    lastFadeUpdate = now;

    LeftRearLed.update(FADE_STEP);
    for (int i = 0; i < ZONE_COUNT; i++)
    {
       // leds[i].update(FADE_STEP);
    }
}
