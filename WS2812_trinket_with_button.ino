 
//CSK 3/17/2013 Libraries new location
//https://github.com/FastLED/FastLED
//https://github.com/FastLED/FastLED/wiki/Overview

#include <EEPROM.h>
#include "FastLED.h"

#define DATA_PIN 1
#define BUTTON_PIN 2
#define MAX_SEQUENCE_INDEX 7
#define EEPROM_INDEX 0

#define MIN_BRIGHTNESS 100
#define MAX_BRIGHTNESS 255

#define CHASE_SPEED 50
#define RAINBOW_SPEED 7

#define NUM_LEDS 25 


CRGB leds[NUM_LEDS];

int currentSequence = 0;

void setup()
{
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  pinMode(BUTTON_PIN, INPUT);

  int fromEEPROM = EEPROM.read(EEPROM_INDEX);
  if (fromEEPROM > 0){
    currentSequence = fromEEPROM;  
  }
}

void loop()
{
  switch(currentSequence)
  {
    case 0:
      color_chase(CRGB::White, CHASE_SPEED, MIN_BRIGHTNESS);
      break;
    case 1:
      color_chase(CRGB::White, CHASE_SPEED, MAX_BRIGHTNESS);
      break;
    case 2:
      color_chase(CRGB::BlueViolet, CHASE_SPEED, MIN_BRIGHTNESS);
      break;
    case 3:
      color_chase(CRGB::BlueViolet, CHASE_SPEED, MAX_BRIGHTNESS);
      break;
    case 4:
      color_chase(CRGB::Cyan, CHASE_SPEED, MIN_BRIGHTNESS);
      break;
    case 5:
      color_chase(CRGB::Cyan, CHASE_SPEED, MAX_BRIGHTNESS);
      break;
    case 6:
      rainbow(CHASE_SPEED, MIN_BRIGHTNESS);
      break;
    case 7:
    default:
      rainbow(CHASE_SPEED, MAX_BRIGHTNESS);
      break;
  }
}

boolean buttonPressed()
{
  // read the state of the pushbutton value:
  int buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == HIGH) {
    return true;
  }
  return false;
}

void incrementAndPause()
{
  if (currentSequence < MAX_SEQUENCE_INDEX){
    currentSequence++;
  } else{
    currentSequence = 0;
  }
  EEPROM.update(EEPROM_INDEX, currentSequence);
  delay(1000);
}


void color_chase(uint32_t color, uint8_t wait, uint8_t brightness)
{
  //clear() turns all LEDs off
  FastLED.clear();
  //The brightness ranges from 0-255
  FastLED.setBrightness(brightness);
  // Move a single led
  for(int led_number = 0; led_number < NUM_LEDS; led_number++)
  {
    leds[led_number] = color;
    FastLED.show();
    delay(wait);

    // Turn our current led back to black for the next loop around
    leds[led_number] = CRGB::Black;

    if(buttonPressed()){
      incrementAndPause();
      return;   
    }
  }
  return;
}

void rainbow(uint8_t wait, uint8_t brightness) 
{
  uint16_t hue;
  FastLED.clear();
  FastLED.setBrightness(brightness);

  for(hue=10; hue<255*3; hue++) 
  {
    fill_rainbow( &(leds[0]), NUM_LEDS /*led count*/, hue /*starting hue*/);    
    FastLED.show();
    delay(wait);

    if(buttonPressed()){
      incrementAndPause();
      return;   
    }
  }
  return;
}

