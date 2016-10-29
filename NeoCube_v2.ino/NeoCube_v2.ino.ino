#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6             //Signal pin on arduino
#define NUM_OF_PIXELS 12  //Amount of neopixels

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_OF_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

//Simple way of representing a color
struct Color
{
  byte Red;
  byte Green;
  byte Blue;
};

//A simple pixel transform table for my particular "display" so they always start from the buttom!
//Note: really use at the moment.
int DisplayLookup[12] = {0, 1, 2, 5, 4, 3, 6, 7, 8, 11, 10, 9};

//The color (RGB)
Color CandleLightColor = {245, 121, 0}; //yellow'ish


//Flickers the lights in a candle like fashion
//wait: amount of time to wait before next change.
//percentOn: 1 - 100, anything above 100 will be seen as full, anything below 0 will be seen a zero.
void CandleLightFlicker(int wait, int percentOn)
{

  //Adjust the intensity of the color; 0 = off, 100 = full on.
  uint32_t color = strip.Color(IntensityConverter((float)CandleLightColor.Red, percentOn),
                               IntensityConverter((float)CandleLightColor.Green, percentOn),
                               IntensityConverter((float)CandleLightColor.Blue, percentOn));

  //set every pixel with the new intensicy
  for (int i = 0; i < strip.numPixels(); i++)
  {
     strip.setPixelColor(DisplayLookup[i], color);
  }

  //Send signal to refresh/"redraw" pixels
  strip.show();

  delay(wait);
}

int IntensityConverter(float value, int percent)
{
  float fixedPercent = 0.0;
  fixedPercent = percent;
  
  if (fixedPercent < 0)
  {
    fixedPercent = 0.0;
  }

  if (fixedPercent > 100)
  {
    fixedPercent = 100.0;
  }
    
  return ((value / 100.0) * fixedPercent);
}

void setup() 
{
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  randomSeed(analogRead(0));
}

void loop() 
{
  CandleLightFlicker(random(80, 200), random(30, 90));
}
