#include "LedControl.h"

const static byte numericBitmap[10][5] = {
  {B01100000, B10010000, B10010000, B10010000, B01100000}, // Number 0 
  {B11110000, B01000000, B01000000, B11000000, B01000000}, // Number 1   
  {B11110000, B10000000, B01100000, B10010000, B01100000}, // Number 2
  {B11110000, B00010000, B01100000, B00010000, B11110000}, // Number 3
  {B00010000, B00010000, B11110000, B10010000, B10010000}, // Number 4
  {B01100000, B00010000, B11110000, B10000000, B11110000}, // Number 5
  {B01100000, B10010000, B11110000, B10000000, B01100000}, // Number 6
  {B01000000, B01000000, B00100000, B00010000, B11110000}, // Number 7
  {B01100000, B10010000, B01100000, B10010000, B01100000}, // Number 8
  {B01100000, B00010000, B01110000, B10010000, B01100000}  // Number 9 
};

unsigned long previousMillis=0;
const long minPeriod=60000;
int workMode=1;
int workMinutes=50, breakMinutes=10;
int tenthMinute, onesMinute;

LedControl lc = LedControl(12, 11, 10, 1);

void setup() {
  Serial.begin(9600);
	lc.shutdown(0, false);
	lc.setIntensity(0, 1); 
	lc.clearDisplay(0);

  // Serial.print(workMinutes); // Testing Statments
  // Serial.println(" Minutes Left");
}

void loop() {
  unsigned long currentMillis=millis();
  
  if (currentMillis - previousMillis >= minPeriod) {
    previousMillis = currentMillis;

    if (workMode) {
      workMinutes -= 1; 

      if (workMinutes < 0) {
        Serial.println("break started! :)");
        workMode = 0;       
        breakMinutes = 10;  

        tenthMinute = breakMinutes / 10;
        onesMinute = breakMinutes % 10;
      } else {

        tenthMinute = workMinutes / 10;
        onesMinute = workMinutes % 10;
      }
    } else { 
      breakMinutes -= 1; 

      if (breakMinutes < 0) {
        Serial.println("work resume! :)");
        workMode = 1;       
        workMinutes = 60;   

        tenthMinute = workMinutes / 10;
        onesMinute = workMinutes % 10;
      } else {

        tenthMinute = breakMinutes / 10;
        onesMinute = breakMinutes % 10;
      }
    }
  }
  time_display(tenthMinute, onesMinute);

  Serial.print("Tenth Place: ");
  Serial.print(tenthMinute);
  Serial.print(" Minute Place: ");
  Serial.println(onesMinute);
  delay(100);
}

void time_display(int tenths, int ones) {
  /*
    Bitwise combine the bitmap byte for the tenths place and an right shift byte for the ones place
    Sets the column with the combine byte
  */ 
  for (int i = 0; i < 5; i++) {
    byte left_byte=numericBitmap[tenths][i];
    byte right_byte=numericBitmap[ones][i] >> 4;
    byte combined_byte=left_byte | right_byte;
    lc.setColumn(0, i, combined_byte);
  }
}

// Testing Functions - To connect the 2D numeric matric back to the LED matric
void display_Bit(int index){
  for (int i = 0; i < 5; i++) {
    lc.setColumn(0, i, numericBitmap[index][i]);
  }
}

void display_Bit_One(int index) {
  for (int i = 0; i < 5; i++) {
    byte shift_value = numericBitmap[index][i] >> 4;
    lc.setColumn(0, i, shift_value);
  }
}