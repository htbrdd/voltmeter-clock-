/*
 Analog Volt Meter Clock
  
 http://admw.uk/ib
  
 Credits:
  Instructables Link: http://www.instructables.com/id/Analog-VU-meter-and-Clock-Arduino-Powered/ 
  bildr - DS1307 RTC code http://bildr.org/2011/03/ds1307-arduino/
 _____________________
 Wiring goes as follows:
 
 - Positive power to RAW
 - Negative power to GND
 
 - Buttons - Up to Digital 2  (follow arduino tutorial: http://arduino.cc/en/uploads/Tutorial/button.png )
           - Down to Digital 3
 
 - DS1307 RTC  - SDA to Analog 4
               - SCL to Analog 5
 
 - Hour Analog Meter to Digital 11 (PWM)
 - Minute Analog Meter to Digital 10 (PWM)
 - Second Analog Meter to Digital 9 (PWM)
_______________________
 
 Notes:
  
  Because all meters are slightly different, the PWM values 
  need to be adjusted based on your design. Feel free to use 
  this code, but you will have to change some values because 
  it is set to work specifically with my meters.
   
  Check the value in hrPWM() !!
  
 */
 
#include "Wire.h"
#define DS1307_ADDRESS 0x68
byte zero = 0x00; //workaround for issue #527
 
int upButton = 2;      // up push button to adjust time (pin 2)
int downButton = 3;    // down push button to adjust time (pin 3)
 
int hours = 0;
int minutes = 0;
int seconds = 0;
int lastMinute = 0;
 
 
int hourMeter = 11;       // hour meter is attached to pin 11
int minMeter = 10;     // minute meter is attached to pin 10
int secMeter = 9;      // second meter is attached to pin 9
 
void setup(){
  Wire.begin();
  Serial.begin(9600); //view time in serial monitor
 
  /*
  If you need to set your DS1307RTC to the correct time, uncomment the setDateTime(); line and configure 
  the setDateTime loop at the bottom of this sketch. Remember to comment out this line again afterwards!
  */
   
  //setDateTime(); 
   
  pinMode(hourMeter, OUTPUT);      // Initialize Outputs
  pinMode(minMeter, OUTPUT);
  pinMode(secMeter, OUTPUT);
   
  digitalWrite(hourMeter, HIGH);   // Test meters on startup
  digitalWrite(minMeter, HIGH);
  digitalWrite(secMeter, HIGH);
  digitalWrite(13, HIGH);
  delay (1000);
  digitalWrite(hourMeter, LOW);
  digitalWrite(minMeter, LOW);
  digitalWrite(secMeter, LOW);
  digitalWrite(13, LOW);
  delay(1000);
}
 
void loop(){
  printDate();
  delay(75);
  
 // Changing the time with buttons
 if (digitalRead(downButton) == HIGH){ 
 byte second = 0;
 byte minute = minutes; // Don't get confused: minute is not the same as minutes
 byte hour = hours;     // Don't get confused: hour is not the same as hours
if (minutes > 0){ 
 --minute; 
}else{
  minute = 59;
 if (hours > 0){ 
  -- hour;
 }else{
   hour = 23;
}}
 Wire.beginTransmission(DS1307_ADDRESS);
 Wire.write(zero); //stop Oscillator
   
  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  
  Wire.write(zero); //start 
   
  Wire.endTransmission();
}
  
 if (digitalRead(upButton) == HIGH){ 
 byte second = 0; 
 byte minute = minutes; // Don't get confused: minute is not the same as minutes
 byte hour = hours;     // Don't get confused: hour is not the same as hours
if (minutes < 59){ 
 ++minute; 
}else{
  minute = 0;
 if (hours < 23){ 
  ++ hour;
 }else{
   hour = 0;
}}
 Wire.beginTransmission(DS1307_ADDRESS);
 Wire.write(zero); //stop Oscillator
   
  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  
  Wire.write(zero); //start 
   
  Wire.endTransmission();
}
  }
 
byte decToBcd(byte val){
// Convert normal decimal numbers to binary coded decimal
  return ( (val/10*16) + (val%10) );
}
 
byte bcdToDec(byte val)  {
// Convert binary coded decimal to normal decimal numbers
  return ( (val/16*10) + (val%16) );
}
 
void printDate(){
 
  // Reset the register pointer
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();
 
  Wire.requestFrom(DS1307_ADDRESS, 7);
   
  seconds = bcdToDec(Wire.read());
  minutes = bcdToDec(Wire.read());
  hours = bcdToDec(Wire.read() & 0b111111); //24 hour time
 
  //print the date
  Serial.print(hours);     // Un-comment to view time in serial monitor  
  Serial.print(":");       //
  Serial.println(minutes); // Don't forget to un-comment 'Serial.begin' in 'void setup' above
  Serial.print(":");       //
  Serial.println(seconds); // 
 
hrPWM();  //Call the hrPWM loop to display correct hours
minPWM(); //Call the minPWM loop to display correct minutes
secPWM(); //Call the secPWM loop to display correct seconds
  
}
 
void hrPWM()
{
      /* Specific meter levels for each hour 
         You will likely have to adjust these values.
         Play around with the values until you get what you want
         or you could attach a potentiometer to a spare input and 
         adjust the levels and read them over a serial connection.
      */ 
   
      if (hours == 0){
     analogWrite(hourMeter, 0); // adjust hour meter level
      }
      if (hours == 1 || hours == 13){
     analogWrite(hourMeter, 23); // adjust hour meter level
      }
      if (hours == 2 || hours == 14){
     analogWrite(hourMeter, 46); // adjust hour meter level
      }
       if (hours == 3 || hours == 15){
     analogWrite(hourMeter, 70); // adjust hour meter level
      }
      if (hours == 4 || hours == 16){
     analogWrite(hourMeter, 92); // adjust hour meter level
      }
       if (hours == 5 || hours == 17){
     analogWrite(hourMeter, 113); // adjust hour meter level
      }
      if (hours == 6 || hours == 18){
     analogWrite(hourMeter, 134); // adjust hour meter level
      }
      if (hours == 7 || hours == 19){
     analogWrite(hourMeter, 154); // adjust hour meter level
      }
      if (hours == 8 || hours == 20){
     analogWrite(hourMeter, 174); // adjust hour meter level
      }
       if (hours == 9 || hours == 21){
     analogWrite(hourMeter, 192); // adjust hour meter level
      }
      if (hours == 10 || hours == 22){
     analogWrite(hourMeter, 210); // adjust hour meter level
      }
       if (hours == 11 || hours == 23){
     analogWrite(hourMeter, 230); // adjust hour meter level
      }
       if (hours == 12){
     analogWrite(hourMeter, 250); // adjust hour meter level
      }
}
 
void minPWM()
{
  analogWrite(minMeter, minutes * 4.25);   // 255 divided by 60 is 4.25
}
 
void secPWM()
{
  analogWrite(secMeter, seconds * 4.25);   // 255 divided by 60 is 4.25 
}
 
void setDateTime(){
 
  byte second =      0; //0-59
  byte minute =      50; //0-59
  byte hour =        18; //0-23
  byte weekDay =     1; //1-7
  byte monthDay =    27; //1-31
  byte month =       9; //1-12
  byte year  =       15; //0-99
 
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero); //stop Oscillator
 
  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(weekDay));
  Wire.write(decToBcd(monthDay));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
 
  Wire.write(zero); //start 
 
  Wire.endTransmission();
 
}
