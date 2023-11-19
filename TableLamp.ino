/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2022 Gregg E. Berman
 *  
 *  https://github.com/HomeSpan/HomeSpan
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *  
 ********************************************************************************/

#include "HomeSpan.h"         // include the HomeSpan library
#include <Arduino.h>
#include <IRremote.h>

#define BRIGHTER      0x9
#define DARKER        0x1D

#define WHITE         0x15

#define OFF           0x1F
#define ON            0xD

#define RED           0x19
#define DARK_ORANGE   0x17
#define ORANGE        0x40
#define LIGHT_ORANGE  0xA
#define YELLOW        0x1C

#define GREEN         0x1B
#define LIGHT_GREEN   0x12
#define TORQUOIS      0x4C
#define GREEN_BLUE    0x1E
#define LIGHT_BLUE    0x14 

#define BLUE          0x11
#define DARK_BLUE     0x16
#define PURPLE        0x4
#define DARK_PINK     0xE
#define PINK          0xF

#define FLASH         0x4D
#define STROBE        0x0
#define FADE          0x1A
#define SMOOTH        0xC


#define IR_SEND_PIN   15

struct TableLamp : Service::LightBulb{

    SpanCharacteristic *power;             // store a reference to the On Characteristic
    SpanCharacteristic *H;                       // reference to the Hue Characteristic
    SpanCharacteristic *S;
    SpanCharacteristic *B;
  
  TableLamp(int lampPin) : Service::LightBulb(){       // constructor() method for TableLamp defined with one parameter.  Note we also call the constructor() method for the LightBulb Service.

    power=new Characteristic::On();      // instantiate the On Characteristic and save it as lampPower
    H=new Characteristic::Hue(15);
    S=new Characteristic::Saturation();
    B=new Characteristic::Brightness(100); 
    new Characteristic::Name("Stehlampe");
    B->setRange(20,100,20);
                     
    
  } // end constructor()
  
  boolean update(){   
    
    int hue = H->getNewVal();
    int saturation = S->getNewVal();                       // update() method
    int brightness = B->getNewVal();

    if(power->getNewVal()==0){
      IrSender.sendNEC(0x0, 0x1F, 0);
    } else if(power->getNewVal()==1){
      IrSender.sendNEC(0x0, 0xD, 0);
    }

    if(saturation<=25){
      IrSender.sendNEC(0x0, WHITE, 0);
    } else {
      int colorCode = colorLookup(hue);
      IrSender.sendNEC(0x0, colorCode, 0);
      //Serial.print("Hue:" + String(colorCode));
    }

    if(brightness==20) {
      IrSender.sendNEC(0x0, DARKER, 0);
      IrSender.sendNEC(0x0, DARKER, 0);
      IrSender.sendNEC(0x0, DARKER, 0);
      IrSender.sendNEC(0x0, DARKER, 0);
    } else if (brightness==40) {
      IrSender.sendNEC(0x0, DARKER, 0);
      IrSender.sendNEC(0x0, DARKER, 0);
      IrSender.sendNEC(0x0, DARKER, 0);
      IrSender.sendNEC(0x0, DARKER, 0);
      IrSender.sendNEC(0x0, BRIGHTER, 0);
    } else if (brightness==60) {
      IrSender.sendNEC(0x0, DARKER, 0);
      IrSender.sendNEC(0x0, DARKER, 0);
      IrSender.sendNEC(0x0, DARKER, 0);
      IrSender.sendNEC(0x0, DARKER, 0);
      IrSender.sendNEC(0x0, BRIGHTER, 0);
      IrSender.sendNEC(0x0, BRIGHTER, 0);
    } else if (brightness==80) {
      IrSender.sendNEC(0x0, DARKER, 0);
      IrSender.sendNEC(0x0, DARKER, 0);
      IrSender.sendNEC(0x0, DARKER, 0);
      IrSender.sendNEC(0x0, DARKER, 0);
      IrSender.sendNEC(0x0, BRIGHTER, 0);
      IrSender.sendNEC(0x0, BRIGHTER, 0);
      IrSender.sendNEC(0x0, BRIGHTER, 0);
    } else if (brightness==100) {
      IrSender.sendNEC(0x0, BRIGHTER, 0);
      IrSender.sendNEC(0x0, BRIGHTER, 0);
      IrSender.sendNEC(0x0, BRIGHTER, 0);
      IrSender.sendNEC(0x0, BRIGHTER, 0);
    }


    
     

    return(true);                            // return true to let HomeKit (and the Home App Client) know the update was successful
  
  } // end update()

  int colorLookup(int hue) {
    int colorCode;

    if(inRange(hue,-1,15)) {
      colorCode = RED;
    } else if (inRange(hue,15,32)) {
      colorCode = DARK_ORANGE;
    } else if (inRange(hue,32,45)) {
      colorCode = ORANGE;
    } else if (inRange(hue,45,55)) {
      colorCode = LIGHT_ORANGE;
    } else if (inRange(hue,55,70)) {
      colorCode = YELLOW;
    } else if (inRange(hue,70,140)) {
      colorCode = GREEN;
    } else if (inRange(hue,140,165)) {
      colorCode = LIGHT_GREEN;
    } else if (inRange(hue,165,175)) {
      colorCode = TORQUOIS;
    } else if (inRange(hue,175,190)) {
      colorCode = GREEN_BLUE;
    } else if (inRange(hue,190,210)) {
      colorCode = LIGHT_BLUE;
    } else if (inRange(hue,210,250)) {
      colorCode = BLUE;
    } else if (inRange(hue,250,260)) {
      colorCode = DARK_BLUE;
    } else if (inRange(hue,260,270)) {
      colorCode = PURPLE;
    } else if (inRange(hue,270,280)) {
      colorCode = DARK_PINK;
    } else if (inRange(hue,280,320)) {
      colorCode = PINK;
    } else if (inRange(hue,320,360)) {
      colorCode = RED;
    }
    return (colorCode);
}

bool inRange(int val, int minimum, int maximum)
{
  return ((minimum < val) && (val <= maximum));
}

  
};

void setup() {     
 
  Serial.begin(115200);       // start the Serial interface
  
  IrSender.begin(IR_SEND_PIN);

  homeSpan.begin();           // initialize HomeSpan

  new SpanAccessory();           // Table Lamp Accessory
  
    new Service::AccessoryInformation();            // HAP requires every Accessory to implement an AccessoryInformation Service  
      new Characteristic::Identify();               // HAP requires the Accessory Information Service to include the Identify Characteristic
        
    new TableLamp(17);                              // instantiate the TableLamp Service (defined below) with lampPin set to 17
  
} // end of setup()

void loop(){

 homeSpan.poll(); 

} // end of loop()
