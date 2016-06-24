/*************************************************** 
 This is the code for the Cube Hour Glass gizmo. The idea is to simulate an hour glass on a LED matrix diplay
 taking  4 hours instead of one. Then taking on hour to rotate the LED matrix display and counting down for 
 another 4 hours before rotating back to the orginal position. This simulated my last working experience in
 an office cube. Counting down 4 hours till noon; then 4 hours till five, with an hour for lunch. 
 The main sequence code was written by David McDougal and ported to the Arduino by me. 
 The hardware is an Arduino Uno, servo and the LED Backpack and Matrix.
 Besides the Adafruit libraries listed below it uses Wire.h and Servo.h
 
 This code includes libraries from Adafruit for the LED back pack I used. Adding required notifictaion:

 ***
 This is a library for our I2C LED Backpacks
  
  Designed specifically to work with the Adafruit LED Matrix backpacks 
  ----> http://www.adafruit.com/products/872
  ----> http://www.adafruit.com/products/871
  ----> http://www.adafruit.com/products/870

  These displays use I2C to communicate, 2 pins are required to 
  interface. There are multiple selectable I2C addresses. For backpacks
  with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
  with 3 Address Select pins: 0x70 thru 0x77

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ***

 Jan 2013. - robboOriginal code developed October 2012. Serious porting and hardware set up 
 2/27/13 -robbo: Updated servo settings and delays. 
 Adding demo mode switch to make it perform the operation in minutes instead of hours. 
 LED 13 will light in demo mode.
 timing worked out as follows:
 4 hours divided by 16 changes = one pixel change per 15 minutes or 900000 miliseconds
 
 Servo should take hour jumping 3 degrees at a time
 
 3/4/13 Final delays set.
 
 ****************************************************/
#include <SoftwareSerial.h>
#include <Servo.h> 

#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

#define MORNING     0
#define AFTERNOON   1
#define MAX_SEQ_NUM 17


Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

Servo myservo;
int pos = 0;
int interval;
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
int buttonState = 0;


// the setup routine runs once when you press reset:
void setup() {                

 
  Serial.begin(9600);
  
  
  Serial.println("8x8 LED Matrix Test mod by robbo");
  
  matrix.begin(0x70);  // pass in the address of the I2C 
  matrix.setBrightness(2);
  matrix.clear();
  myservo.attach(9,544,2300); // Set up Servo
  myservo.write(pos);
  
// initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
// initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT); 
  digitalWrite(buttonPin,HIGH);
}


typedef struct _led
{
   char order;
   int  x;
   int  y;
} led_t;

// An array to define the LED layout (a pin number of 0 means no LED)
// Each array member has a sequence number and a LED pin number.
// The sequence number determines the order in which they switch on or off

const led_t ledArray [8][7] = {
/* elem 0 */ {{6, 0, 0},   {4, 0, 1},   {2, 0, 2},   {1, 0, 3},  {3, 0, 4},   {5, 0, 5},   {7, 0, 6}}, 
/* elem 1 */ {{0, 1, 0},   {11, 1, 1},  {9, 1, 2},   {8, 1, 3},  {10, 1, 4},  {12, 1, 5},  {0, 1, 6}}, 
/* elem 2 */ {{0, 2, 0},   {0, 2, 1},   {14, 2, 2},  {13, 2, 3}, {15, 2, 4},  {0, 2, 5},   {0, 2, 6}}, 
/* elem 3 */ {{0, 3, 0},   {0, 3, 1},   {0, 3, 2},   {16, 3, 3}, {0, 3, 4},   {0, 3, 5},   {0, 3, 6}}, 
/* elem 4 */ {{0, 4, 0},   {0, 4, 1},   {0, 4, 2},   {16, 4, 3}, {0, 4, 4},   {0, 4, 5},   {0, 4, 6}}, 
/* elem 5 */ {{0, 5, 0},   {0, 5, 1},   {15, 5, 2},  {9, 5, 3},  {14, 5, 4},  {0, 5, 5},   {0, 5, 6}}, 
/* elem 6 */ {{0, 6, 0},   {12, 6, 1},  {8, 6, 2},   {4, 6, 3},  {7, 6, 4},   {13, 6, 5},  {0, 6, 6}}, 
/* elem 7 */ {{11, 7, 0},  {5, 7, 1},   {3, 7, 2},   {1, 7, 3},  {2, 7, 4},   {6, 7, 5},   {10, 7, 6}}
}; 


// the loop routine runs over and over again forever:
void loop() 
{
   

   char interval = MORNING;
   int seqNum = 0; // seqNum of 0 initialise the LED array to have the top half on.
   int  i, j, k, limit;
   int delay_mult = 60; //demo mode option.
   matrix.clear();
   matrix.blinkRate(0);
   matrix.writeDisplay(); 
//Demo Mode button test
   buttonState = digitalRead(buttonPin);

// check if the pushbutton is pressed.
// if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {     
 // turn LED on:    
    digitalWrite(ledPin, LOW); 
    Serial.println("Cube Hour Glass!\n");  matrix.setTextSize(1);
    matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
    matrix.setTextColor(LED_ON);
    for (int8_t x=0; x>=-36; x--) {
        matrix.clear();
        matrix.setCursor(x,0);
	matrix.print("Club");
        matrix.writeDisplay();
        delay(100);
        }
//  matrix.setRotation(3);
   for (int8_t x=7; x>=-36; x--) {
       matrix.clear();
       matrix.setCursor(x,0);
       matrix.print("Robbo");
       matrix.writeDisplay();
       delay(100);
        }
   matrix.setRotation(0);
   } 
   else {
 // turn LED off:
    digitalWrite(ledPin, HIGH); 
 //   delay_mult = 60;
    Serial.println("Cube Hour Glass demo!\n");
     matrix.setTextSize(1);
     matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
     matrix.setTextColor(LED_ON);
     for (int8_t x=0; x>=-36; x--) {
     matrix.clear();
     matrix.setCursor(x,0);
     matrix.print("DEMO");
     matrix.writeDisplay();
     delay(100);
      }
 //   matrix.setRotation(3);
    for (int8_t x=7; x>=-36; x--) {
     matrix.clear();
     matrix.setCursor(x,0);
     matrix.print("Mode");
     matrix.writeDisplay();
    delay(100);
   }
   matrix.setRotation(0);
   }
  
//End of button test  
  
   while (1) //do this bit forever!
   {
      for (k = 0; k < MAX_SEQ_NUM; k++)
      {
         if (interval == MORNING)
         {
            for (i = 0; i < 4; i++)
            {
               for (j = 0; j < 7; j++)
               {
                  if (ledArray[i][j].order  > seqNum)
                  {
                      matrix.drawPixel(ledArray[i][j].x, ledArray[i][j].y, LED_ON);
                      matrix.writeDisplay(); 
                  }
                  else
                  {
                      matrix.drawPixel(ledArray[i][j].x, ledArray[i][j].y, LED_OFF);
                      matrix.writeDisplay(); 
                  }
               }
            }
            for (i = 4; i < 8; i++)
            {
               for (j = 0; j < 7; j++)
               {
                  if (ledArray[i][j].order <= seqNum && ledArray[i][j].order != 0)
                  {
                      matrix.drawPixel(ledArray[i][j].x, ledArray[i][j].y, LED_ON);
                      matrix.writeDisplay(); 
                  }
                  else
                  {
                      matrix.drawPixel(ledArray[i][j].x, ledArray[i][j].y, LED_OFF);
                      matrix.writeDisplay(); 
                  }
               }
            }
         }
         else
         {
            for (i = 4; i < 8; i++)
            {
               for (j = 0; j < 7; j++)
               {
                  if (ledArray[i][j].order  > seqNum)
                  {
                      matrix.drawPixel(ledArray[i][j].x, ledArray[i][j].y, LED_ON);
                      matrix.writeDisplay(); 
                  }
                  else
                  {
                      matrix.drawPixel(ledArray[i][j].x, ledArray[i][j].y, LED_OFF);
                      matrix.writeDisplay(); 
                  }
               }
            }
            for (i = 0; i < 4; i++)
            {
               for (j = 0; j < 7; j++)
               {
                  if (ledArray[i][j].order  <= seqNum && ledArray[i][j].order != 0)
                  {
                      matrix.drawPixel(ledArray[i][j].x, ledArray[i][j].y, LED_ON);
                      matrix.writeDisplay(); 
                  }
                  else
                  {
                      matrix.drawPixel(ledArray[i][j].x, ledArray[i][j].y, LED_OFF);
                      matrix.writeDisplay(); 
                  }
               }
            }
         }

         seqNum++;
         if (buttonState == HIGH)
         {
 //        delay((3750 * delay_mult * 4)/16);// Looks close to one minute. Therefore multiple by 60 for an hour ~= 225,000.
           delay(900000);
         }
         else
         {
            delay(3750); //Demo mode.
         }
      }


      interval = (interval == MORNING) ? AFTERNOON : MORNING;

      if (interval == AFTERNOON)
      {
 //         pos = 180;
 //         myservo.write(pos);
 //         delay(1000);
       for(pos = 0; pos < 180; pos += 3)  // goes from 0 degrees to 180 degrees 
      {                                   // in steps of 3 degree 
       myservo.write(pos);                // tell servo to go to position in variable 'pos' 
                                         // waits 1 minute(debug) for the servo to reach the position Use the hour mulitplier 225k
      if (buttonState == HIGH)
         {
//           delay(3750 * delay_mult);// Looks close to one minute. Therefore multiple by 60 for an hour ~= 225,000.
            delay(60000);
         }
         else
         {
            delay(1000);//demo mode.
         }
      }
     }
      else
      {

       
       for(pos = 179; pos > 0; pos -= 3)  // goes from 180 degrees to 0 degrees 
      {                                   // in steps of -3 degree 
       myservo.write(pos);                // tell servo to go to position in variable 'pos' 
                                          // waits 1m (debug) for the servo to reach the position Use the hour mulitplier 225k
      if (buttonState == HIGH)
         {

           delay(60000);
         }
       else
         {
            delay(1000); //demo mode.
         } 
      }
      }

      seqNum = 0;
   }

}
