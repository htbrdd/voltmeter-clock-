1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
54
55
56
57
58
59
60
61
62
63
64
65
66
67
68
69
70
71
72
73
74
75
76
77
78
79
80
81
82
83
84
85
86
87
88
89
90
91
92
93
94
95
96
97
98
99
100
101
102
103
104
105
106
107
108
109
110
111
112
113
114
115
116
117
118
119
120
121
122
123
124
125
126
127
128
129
130
131
132
133
134
135
136
137
138
139
140
141
142
143
144
145
146
147
148
149
150
151
152
153
154
155
156
157
158
159
160
161
162
163
164
165
166
167
168
169
170
171
172
173
174
175
176
177
178
179
180
181
182
183
184
185
186
187
188
189
190
191
192
193
194
195
196
197
198
199
200
201
202
203
204
205
206
207
208
209
210
211
212
213
214
215
216
217
218
219
220
221
222
223
224
225
226
227
228
229
230
231
232
233
234
235
236
237
238
239
240
241
242
243
244
245
246
247
248
249
250
251
252
253
254
255
256
257
258
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
