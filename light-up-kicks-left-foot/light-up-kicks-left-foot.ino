/*Author: Naomi Touchet
  August 2015
  Credit to: Adafruit Firewalker code by Phillip Burgess & Clayton Ritcher's Luminous Lowtops
*/

#include <Adafruit_NeoPixel.h>

const int numStrand = 37; //number of RGB LEDs in strand
const int heelInPin = A10; //Analog input pin for toe sensor
const int toeInPin = A9; //Analog input pin for heel sensor
const int backOfShoe = 8; //index of the rear-most LED on shoe
const int shoeLength = 18; //Number of LEDs down one side of the shoe
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numStrand, 6, NEO_GRB + NEO_KHZ800); //instance of the strip object
int toeSensorValue = 0; //value read from toe sensor
int heelSensorValue = 0; //value read from heel sensor
int decrementCounter = 8;
int secondDecrementCounter = 36;
int colorCount = 0;

void setup() {
  // initialize communication with serial
  Serial.begin(9600);
  //set pin mode
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
     
  //start up neopixel strip
  strip.begin();
  strip.show(); //Initialize all pixels to 'off'
} 

void loop() {
  // read the analog in value:
  toeSensorValue = analogRead(toeInPin)*1.5; 
  heelSensorValue = analogRead(heelInPin)*1.5;  
  // print the results to the serial monitor:
//  Serial.println("toe");  
//  Serial.println(toeSensorValue);
//  Serial.println("heel");
//  Serial.println(heelSensorValue);
//  delay(50);  

 
    //iterate throught the LEDs(only half way because the shoe is a mirror image
  for (int i=8; i<=27; i++) {
    if(decrementCounter >=0){
      decrementCounter--;
      //set the current LED and its mirrored pair to a combination of the front and back force sensor readings, proportional to their distance from each sensor
      strip.setPixelColor(i, getColorFromForce((toeSensorValue*colorCount+heelSensorValue*((36/2)-colorCount-1))/10));
      strip.setPixelColor(decrementCounter, getColorFromForce((toeSensorValue*colorCount+heelSensorValue*((36/2)-colorCount-1))/10));
    }else if(secondDecrementCounter >= 27){
      //set the current LED and its mirrored pair to a combination of the front and back force sensor readings, proportional to their distance from each sensor
      strip.setPixelColor(i, getColorFromForce((toeSensorValue*colorCount+heelSensorValue*((36/2)-colorCount-1))/10));
      strip.setPixelColor(secondDecrementCounter, getColorFromForce((toeSensorValue*colorCount+heelSensorValue*((36/2)-colorCount-1))/10));
      secondDecrementCounter--;  
    }
    colorCount++;
  }
  decrementCounter = 10;
  secondDecrementCounter = 36;
  colorCount = 0;
    //send the new colors to the strip
  strip.show();
                    
}

//color flash function
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
  
//right & left feet = 37 pixels total, rearmost = 8,  pixels long = 18,  top = 26
  
  

}

//helper function that shifts and scales the force reading to a range of 0-384(for the color wheel function)
uint32_t getColorFromForce(int force){
  //the shift and scaling factors were found experimentally
  int color=force*2;
//  Serial.println("color");
//  Serial.println(color);
//  delay(500);
  if (color>384){
    color=384;
  }
  else if (color<0){
    color=0;
  }
  //return the color that matches the force
  return Wheel(color);
}

//Input a value 0 to 384 to get a color value.
//The colours are a transition b - g -r - back to b
uint32_t Wheel(uint16_t WheelPos)
{
  byte r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      b = 127 - WheelPos % 128;   //blue down
      g = WheelPos % 128;      // Green up
      r = 0;                  //red off
      break; 
    case 1:
      g = 127 - WheelPos % 128;  //green down
      r = WheelPos % 128;      //red up
      b = 0;                  //blue off
      break; 
    case 2:   
      r = 127 - WheelPos % 128;  //red down 
      b = WheelPos % 128;      //blue up
      g = 0;                  //green off
      break; 
  }
  return(strip.Color(r,g,b));
}
