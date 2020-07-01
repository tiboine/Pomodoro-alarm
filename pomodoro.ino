#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_TFTLCD.h>
#include <Fonts/FreeSans24pt7b.h>
#include <arduino-timer.h>
#include <TimeLib.h>
#include "pitches.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

auto timer = timer_create_default();
const int buzzer = 10;
const int button = 5;
int buttonState = 0;
int lastButtonState = 0;
int counter = 3600;
const long interval = 1000;
long prevMillis;
int startTime = 0;
bool start = false;
unsigned long startMillis;
unsigned long currentMillis;

int work = 0;





void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  pinMode(button, INPUT);
  pinMode(buzzer, OUTPUT);
}

//Mario main theme melody
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7, 
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0, 

  NOTE_C7, 0, 0, NOTE_G6, 
  0, 0, NOTE_E6, 0, 
  0, NOTE_A6, 0, NOTE_B6, 
  0, NOTE_AS6, NOTE_A6, 0, 

  NOTE_G6, NOTE_E7, NOTE_G7, 
  NOTE_A7, 0, NOTE_F7, NOTE_G7, 
  0, NOTE_E7, 0,NOTE_C7, 
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6, 
  0, 0, NOTE_E6, 0, 
  0, NOTE_A6, 0, NOTE_B6, 
  0, NOTE_AS6, NOTE_A6, 0, 

  NOTE_G6, NOTE_E7, NOTE_G7, 
  NOTE_A7, 0, NOTE_F7, NOTE_G7, 
  0, NOTE_E7, 0,NOTE_C7, 
  NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
int tempo[] = {
  12, 12, 12, 12, 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12, 

  12, 12, 12, 12,
  12, 12, 12, 12, 
  12, 12, 12, 12, 
  12, 12, 12, 12, 

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};

void loop() {
 

       
  //delay(1000);
  // melodyNote();
  currentMillis = millis();
  buttonState = digitalRead(button);

  display.display();
  display.clearDisplay();   // clears the screen and buffer
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      
      work = 1;
      setTime(0, 0, 0, 1, 1, 2010);
    }
  };

  lastButtonState = buttonState;

  if (work == 1) {

    tomato();
  }
  if (work == 2) {
    workBreak();
  }
  
}


void tomato() {

  int tomatoMin = startTime - minute();
  int tomatoSec = 5 - second() - 1;
  String sec = String(tomatoSec);
  String mins = String(tomatoMin);

  if (tomatoSec < 10) {
    sec = 0 + sec;
  }


  display.println( mins + ":" + sec );

  if ((mins == "0") && (sec == "00")) {
    work = 2;
  }
}

void workBreak() {

    playMario();

  
  display.clearDisplay();
  display.print("tomato");
  work = 1;
}

void playMario() {
   int size = sizeof(melody) / sizeof(int);
     for (int thisNote = 0; thisNote < size; thisNote++) {

       // to calculate the note duration, take one second
       // divided by the note type.
       //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
       int noteDuration = 1000/tempo[thisNote];

       tone(buzzer, melody[thisNote],noteDuration);

       // to distinguish the notes, set a minimum time between them.
       // the note's duration + 30% seems to work well:
       int pauseBetweenNotes = noteDuration * 1.30;
       delay(pauseBetweenNotes);

       // stop the tone playing:
       tone(buzzer, 0,noteDuration);

    }
}
