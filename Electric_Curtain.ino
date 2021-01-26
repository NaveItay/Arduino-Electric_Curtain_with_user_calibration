/*
 * Electric_Curtain_Itay_V5
 */

#include <EEPROM.h>
int Down, Up;

#include <OneButton.h>
OneButton button(A1, true);                         //attach a button on pin A1 to the library

// RGB LED
#include <Adafruit_NeoPixel.h>
#define LED_PIN 5 
#define PIXEL_COUNT 1
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// constants won't change. They're used here to set pin numbers:
const int ButtonUp = 2;     // the number of the pushbutton pin
const int ButtonDown = 3;     // the number of the pushbutton pin

// variables will change:
int ButtonUpState = 0;         // variable for reading the pushbutton status
int ButtonDownState = 0;         // variable for reading the pushbutton status

// variables for times
int CountUp = 0;
int UpValue;
int CountDown = 0;
int DownValue;

// System Modes:
//  'A' - 'Active_Mode'
//  'U' - 'Learning_Mode Up'
//  'D' - 'Learning_Mode Down'
//  'R' - 'Reset_Mode'

char Mode = 'A';

void setup() {
  Serial.begin(9600);

  button.attachDoubleClick(doubleclick);            // link the function to be called on a doubleclick event.
  button.attachClick(singleclick);                  // link the function to be called on a singleclick event.
  button.attachLongPressStop(longclick);            // link the function to be called on a longpress event.

  // RGB LED
      strip.begin();
      strip.setBrightness(70);
      strip.show(); // Initialize all pixels to 'off'
      pinMode(LED_PIN, OUTPUT);
        // Set output to LOW
             // Strip 1
                 strip.setPixelColor(0, 0, 0, 0, 0);  // reset leds       
                 strip.show();
             

  // initialize the pushbutton pin as an input:
       pinMode(ButtonUp, INPUT);
       pinMode(ButtonDown, INPUT);

  // read from EEPROM
  Down = EEPROM.read(0);
  Up = EEPROM.read(5);
  Serial.println("");
  Serial.print("EEPROM Down value at Adress '0' is:  ");
  Serial.println(Down);
  Serial.print("EEPROM Up value at Adress '5' is:  ");
  Serial.println(Up);
       
}

void loop() {
  
// System button
  button.tick();        // check the status of the button
  delay(10);            // a short wait between checking the button
  
  switch (Mode)
  {
  case 'A':                                   //      'Active_Mode'
  {
    // read the state of the pushbutton value:
      ButtonUpState = digitalRead(ButtonUp);
      ButtonDownState = digitalRead(ButtonDown);
    
    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
      if (ButtonUpState == HIGH) {
        // turn LED on:
        LED(0);
        Serial.print("Up");
      } else {
        // do nothing
      }
      
    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:    
        if (ButtonDownState == HIGH) {
        // turn LED on:
        LED(0);
        Serial.print("Down");
      } else {
        // do nothing
      }
      break;
  }
  case 'D':                                   //      'Learning_Mode Down'
  {
    // read the state of the pushbutton value:
      ButtonUpState = digitalRead(ButtonUp);
      ButtonDownState = digitalRead(ButtonDown);
    
    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
      if (ButtonUpState == HIGH) {
        // turn LED on:
        LED(0);
        Serial.print("Up");
        DownValue = CountDown--;
      } else {
        // do nothing
      }
      
    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
        if (ButtonDownState == HIGH) {
        // turn LED on:
        LED(0);
        Serial.print("Down");
        DownValue = CountDown++;
      } else {
        // do nothing
      }

    //  Save times to 'DownValue'    
      if ( (ButtonDownState && ButtonUpState) == HIGH )
        {
          LED(2);  // Blink twice
          delay(2000);
          Serial.println("");
          Serial.print("DownValue is: ");
          Serial.print(DownValue);
          delay(25);
          // EEPROM
          Serial.print("       ...  Saving to EEPROM  Adress: '0' ");
          EEPROM.write(0, DownValue);
          Serial.println("");
          Serial.print("EEPROM Down value at Adress '0' is:  ");
          Down = EEPROM.read(0);
          Serial.println(Down);
          Serial.println("Moving to 'Learning_Mode Up' ... ");
          LED(2);  // Blink twice
          CountDown = 0; // reset counter for next event
          Mode = 'U';   
        }
      break;
  }
  case 'U':                                   //      'Learning_Mode Up'
      {
    // read the state of the pushbutton value:
      ButtonUpState = digitalRead(ButtonUp);
      ButtonDownState = digitalRead(ButtonDown);
    
    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
      if (ButtonUpState == HIGH) {
        // turn LED on:
        LED(0);
        Serial.print("Up");
        UpValue = CountUp++;
      } else {
        // do nothing
      }
      
    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
        if (ButtonDownState == HIGH) {
        // turn LED on:
        LED(0);
        Serial.print("Down");
        UpValue = CountUp--;
      } else {
        // do nothing
      }

    //  Save times to 'UpValue'    
      if ( (ButtonDownState && ButtonUpState) == HIGH )
        {
          LED(3);   // Blink 3 times
          delay(2000);
          Serial.println("");
          Serial.print("UpValue is: ");
          Serial.print(UpValue);
          Serial.println("");
          // EEPROM
          Serial.print("       ...  Saving to EEPROM  Adress: '5' ");
          EEPROM.write(5, UpValue);
          Serial.println("");
          Serial.print("EEPROM Up value at Adress '5' is:  ");
          Up = EEPROM.read(5);
          Serial.println(Up);
          Serial.println("Moving back to 'Active_Mode' ");
          LED(5);   // Green LED, rdy to go
          CountUp = 0; // reset counter for next event
          Mode = 'A';   
        }  
        break;
      }
  case 'R':                                 //      'Reset_Mode'
  {
   // Reset values
      UpValue = 0;
      DownValue = 0;
      EEPROM.write(5, 0);
      EEPROM.write(0, 0);
      Serial.println("EEPROM values are empty");
      
      // read from EEPROM
      Down = EEPROM.read(0);
      Up = EEPROM.read(5);
      Serial.println("");
      Serial.print("EEPROM Down value at Adress '0' is:  ");
      Serial.println(Down);
      Serial.print("EEPROM Up value at Adress '5' is:  ");
      Serial.println(Up);
      
      // Moving back to 'Active_Mode'
      Serial.println("Moving back to 'Active_Mode' ");
      Mode = 'A'; 
      LED(5);   // Green LED, rdy to go     
      break;
  }
  
  } // Close swtich
}   // Close loop

void LED(int Blink){
  if (Blink == 0){                              // Light Blue LED, motor is working
    strip.setPixelColor(0, 0, 255, 255, 0);
    strip.show();
    delay(250);
    strip.setPixelColor(0, 0, 0, 0, 0);
    strip.show();
  }
  else if (Blink == 1) {                        // Orange LED
      strip.setPixelColor(0, 255, 127, 39, 0);  
      strip.show();
      delay(1000);
      strip.setPixelColor(0, 0, 0, 0, 0);
      strip.show();
      delay(1000);
      }
  else if (Blink == 2) {                        // Orange LED, blink twice
    for (int b=0;b<2;b++){                      // 'Learning_Mode Down' Mode
      strip.setPixelColor(0, 255, 127, 39, 0);   
      strip.show();
      delay(1000);
      strip.setPixelColor(0, 0, 0, 0, 0);
      strip.show();
      delay(1000);
      }
  }
  else if (Blink == 3) {                       // Orange LED, blink 3 times
    for (int b=0;b<3;b++){                     // 'Learning_Mode Up' Mode
      strip.setPixelColor(0, 255, 127, 39, 0); 
      strip.show();
      delay(1000);
      strip.setPixelColor(0, 0, 0, 0, 0);
      strip.show();
      delay(1000);
      }
  }
    else if (Blink == 4) {                     // RED LED, 'Reset' Mode 
      strip.setPixelColor(0, 255, 0, 0, 0);  
      strip.show();
      delay(2000);
      strip.setPixelColor(0, 0, 0, 0, 0);
      strip.show();
      delay(100);
      }
    else if (Blink == 5) {                     // Green LED, 'Active' Mode
      strip.setPixelColor(0, 0, 255, 0, 0);   
      strip.show();
      delay(1000);
      strip.setPixelColor(0, 0, 0, 0, 0);
      strip.show();
      delay(100);
      }
}

void singleclick(){         // what happens when the button is clicked
 Serial.println(""); 
 Serial.print("SingleClick detect: 'Active_Mode' ");
 Serial.println("");                                                     
 Mode = 'A';          
 LED(5);   // Green LED, rdy to go                                                                                                                         
}

void doubleclick() {        // what happens when button is double-clicked
 Serial.println(""); 
 Serial.print("DoubleClick detect: 'Learning_Mode Down' ");                        
 Serial.println("");
 LED(1);      // Orange light
 Mode = 'D';  
 delay(1000);                                                          
} 
 
void longclick(){           // what happens when button is long-pressed
 Serial.println(""); 
 Serial.print("LongClick detect: 'Reset_Mode' ");                            
 Serial.println("");
 LED(4);         // Red light for 2 seconds
 Mode = 'R';
 delay(1000);                                      
}
