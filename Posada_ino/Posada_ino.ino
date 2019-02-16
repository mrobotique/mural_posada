/*
     https://github.com/FastLED/FastLED/wiki
*/


// JSON Includes
#include <ArduinoJson.h>

// Fastled
#include <FastLED.h>

// LCD Includes
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>


#define LED_PIN     6
#define NUM_LEDS    6
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 100

/*Led STRIP*/
CRGB leds[NUM_LEDS];
/*  JSON */
const size_t capacity = JSON_OBJECT_SIZE(9) + 150;
DynamicJsonDocument doc(capacity);
/*LCD*/
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
/*Globals and Dict Def*/
char* nominis;

int first_led;
int last_led;
int red_value;
int green_value;
int blue_value;
int brightness_value;
int mode;
int mode_time;

/*Special char*/
byte heart[8] = { 0b00000, 0b01010,
                  0b11111, 0b11111,
                  0b11111, 0b01110,
                  0b00100, 0b00000
                };


/*Prototypes*/
void AssignValues(JsonObject);
void HeartBeat(unsigned long);
void delRow(int);
String formatInt(int);
void serialFlush();

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  serialFlush();

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setBacklight(true);
  lcd.setCursor(1, 0);
  lcd.print("Power-up safety");
  //LedStrip init
  delay( 1000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  // set up the LCD's number of columns and rows:

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setBacklight(true);
  lcd.setCursor(1, 0);
  lcd.print("Ready");
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  HeartBeat(currentMillis);
}

void serialEvent() {
  if (Serial.available() > 0) {
    String input_str = Serial.readStringUntil('\n');
    deserializeJson(doc, input_str);
    JsonObject obj = doc.as<JsonObject>();
    AssignValues(obj);
  }
}


void AssignValues(JsonObject obj) {
  /*
     The expected string is as follows:
      {'name':'String','init':int,'end':int,'R':int,'G':int,'B':int,'Brightness':60,'mode':0,'mode_time':10}
  */
  String nominis = obj["name"];
  first_led = obj[String("init")];
  last_led = obj[String("end")];
  red_value = obj[String("R")];
  green_value = obj[String("G")];
  blue_value = obj[String("B")];
  brightness_value = obj[String("Brightness")];
  mode = obj[String("mode")];
  mode_time = obj[String("mode_time")];
  first_led = obj[String("init")];
  last_led = obj[String("end")];
  red_value = min(obj[String("R")], 255);    //Saturated num <= 255
  green_value = min(obj[String("G")], 255); //Saturated num <= 255
  blue_value = min(obj[String("B")], 255); //Saturated num <= 255
  brightness_value =  min(obj[String("Brightness")], 255);
  mode = obj[String("mode")];
  mode_time = obj[String("mode_time")];
  //Print Frame Name and Brightness value
  delRow(0);
  lcd.setCursor(1, 0);
  lcd.print(nominis);
  lcd.setCursor(11, 0);
  String brightness_str = "L:" + formatInt(brightness_value);
  lcd.print(brightness_str);
  //Print RGB Values
  delRow(1);
  String rgb = "RGB[" + formatInt(red_value) + "," + formatInt(green_value) + "," + formatInt(blue_value) + "]";
  lcd.setCursor(0, 1);
  lcd.print(rgb);
  LedUpdate();
}


void LedUpdate() {
  for (int i = int(first_led); i < int(last_led); i++) {
    leds[i].setRGB(int(red_value), int(green_value), int(blue_value));
    leds[i].fadeLightBy(255 - int(brightness_value));
    //leds[0].setRGB(255, 0, 0);
    //leds[0].fadeLightBy(100);
  }
  FastLED.show();
  //FastLED.delay(10000 / UPDATES_PER_SECOND);
}


void HeartBeat(unsigned long currentMillis) {
  static bool systole = false;
  static unsigned long  previousMillis = millis();
  static int interval = 800;
  lcd.createChar(1, heart);
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the Heart
    previousMillis = currentMillis;
    // if the Heart is off turn it on and vice-versa:
    if (systole) {
      systole = false;
      interval = 400;
      lcd.setCursor(0, 0);
      lcd.write(1);
    } else {
      systole = true;
      interval = 900;
      lcd.setCursor(0, 0);
      lcd.print(" ");
    }
  }
}


void delRow(int row) {
  lcd.setCursor(0, row);
  lcd.print("                ");
}

String formatInt(int Number) {
  String Result = "";
  if (Number < 100) {
    Result += "0";
  }
  if (Number < 10) {
    Result += "0";
  }
  Result += String(Number);
  return Result;
}

void serialFlush() {
  while (Serial.available() > 0) {
    char t = Serial.read();
  }
}
