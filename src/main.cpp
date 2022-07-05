#include <Arduino.h>
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include "./creds.h"
#include "./secrets.h"

#pragma region Variables

#define NUM_LEDS 1

const char* hostName = "petfooddispenser";

// wiring
const int SWITCH_PIN = A0;
const int MOTOR_PINA = D7;  
const int LED_PIN = D0;
const int BUTTON_PINS[] = {D2, D3, D4, D5, D1};
const int BUTTON_COUNT = (sizeof(BUTTON_PINS) / sizeof(BUTTON_PINS[0]));
const int SW_IDLE = 20;

// only thing you need to modify to increase or decrease the amount of food per poriton
int PORTION_SIZE = 5;

int FEED_PORTION, SNACK_PORTION;
int buttonState, switchState, lastSwitchState;
int FEED_TRIGGER = 0, SNACK_TRIGGER = 0;;
int data[5] = {0, 0, 3, 1, 5};
int dataLen = sizeof(data)/sizeof(data[0]);
int fields[5] = {1, 2, 3, 4, 5};
int fieldsLen = sizeof(fields)/sizeof(fields[0]);

unsigned long currentMillis = millis();
unsigned long lastCall = millis();
unsigned long lastTriggered = millis();
unsigned long previousMillis = millis();
int call_interval = 15250;
int one_second = 1000;
int api_delay = 0;
int feed_delay = 0;

boolean triggered = false, skipCall = false, readyStatus = true;

WiFiClient client;

CRGB LED[NUM_LEDS];

#pragma endregion

#pragma region LED Functions
void ledOff()
{
  LED[0] = CRGB::Black;
  FastLED.show();
  delay(30);
}

void ledBlue()
{
  LED[0] = CRGB::Blue;
  FastLED.show();
  delay(30);
}

void ledGreen()
{
  LED[0] = CRGB::Green;
  FastLED.show();
  delay(30);
}

void ledRed()
{
  LED[0] = CRGB::Red;
  FastLED.show();
  delay(30);
}

void ledYellow()
{
  LED[0] = CRGB::Yellow;
  FastLED.show();
  delay(30);
}

void ledPurple()
{
  LED[0] = CRGB::Purple;
  FastLED.show();
  delay(30);
}

void ledYellowPurple()
{
  for(int j=0;j<=4;j++){
    LED[0] = CRGB::Purple;
    FastLED.show();
    delay(25);
    LED[0] = CRGB::Yellow;
    FastLED.show();
    delay(25);
  }
}

void ledRedBlue()
{
  for(int j=0;j<=4;j++){
    LED[0] = CRGB::Red;
    FastLED.show();
    delay(25);
    LED[0] = CRGB::Blue;
    FastLED.show();
    delay(25);
  }
}

void ledYellowBlue()
{
  for(int j=0;j<=4;j++){
    LED[0] = CRGB::Yellow;
    FastLED.show();
    delay(25);
    LED[0] = CRGB::Blue;
    FastLED.show();
    delay(25);
  }
}

void ledRedYellow()
{
  for(int j=0;j<=4;j++){
    LED[0] = CRGB::Yellow;
    FastLED.show();
    delay(25);
    LED[0] = CRGB::Red;
    FastLED.show();
    delay(25);
  }
}

void ledRedPurple()
{
  for(int j=0;j<=4;j++){
    LED[0] = CRGB::Purple;
    FastLED.show();
    delay(25);
    LED[0] = CRGB::Red;
    FastLED.show();
    delay(25);
  }
}

void ledYellowGreen()
{
  for(int j=0;j<=4;j++){
    LED[0] = CRGB::Yellow;
    FastLED.show();
    delay(25);
    LED[0] = CRGB::Green;
    FastLED.show();
    delay(25);
  }
}

void ledBlueGreen()
{
  for(int j=0;j<=4;j++){
    LED[0] = CRGB::Blue;
    FastLED.show();
    delay(25);
    LED[0] = CRGB::Green;
    FastLED.show();
    delay(25);
  }
}

void ledRedGreen()
{
  for(int j=0;j<=4;j++){
    LED[0] = CRGB::Red;
    FastLED.show();
    delay(25);
    LED[0] = CRGB::Green;
    FastLED.show();
    delay(25);
  }
}

void ledRedBlink()
{
  for(int i=0;i<=5;i++)
  {
    LED[0] = CRGB::Red;
    FastLED.show();
    delay(25);
    LED[0] = CRGB::Black;
    FastLED.show(); 
    delay(25);
  }  
}

void ledBlinkDelay(CRGB color, int _delay) {
  unsigned long _start = millis();
  unsigned long _last = _start;  
  while(_delay >= _last - _start) {
    LED[0] = color;
    FastLED.show();
    delay(100);
    LED[0] = CRGB::Black;  
    FastLED.show();
    delay(100);
    _last = millis();
  }
}

void ledYellowBlink()
{
  for(int i=0;i<=5;i++)
  {
    LED[0] = CRGB::Yellow;
    FastLED.show();
    delay(25);
    LED[0] = CRGB::Black;
    FastLED.show(); 
    delay(25);
  }  
}

void ledBlueBlink()
{
  for(int i=0;i<=5;i++)
  {
    LED[0] = CRGB::Blue;
    FastLED.show();
    delay(25);
    LED[0] = CRGB::Black;
    FastLED.show(); 
    delay(25);
  }  
}

void ledGreenBlink()
{
  for(int i=0;i<=5;i++)
  {
    LED[0] = CRGB::Green;
    FastLED.show();
    delay(25);
    LED[0] = CRGB::Black;
    FastLED.show(); 
    delay(25);
  }  
}

void ledPurpleBlink()
{
  for(int i=0;i<=5;i++)
  {
    LED[0] = CRGB::Purple;
    FastLED.show();
    delay(25);
    LED[0] = CRGB::Black;
    FastLED.show(); 
    delay(25);
  }  
}

void ledStatusBlink(int j = 0)
{
  for(int k=0;k<=j;k++)
  {
    ledBlue();
    delay(25);
    ledOff();
    delay(25);
  }
}

#pragma endregion

#pragma region Hardware Functions
void motorStart()
{
  digitalWrite(MOTOR_PINA, LOW); 
}

void motorStop()
{
  digitalWrite(MOTOR_PINA, HIGH); 
}

int getSwitchState()
{
  int ret = (analogRead(SWITCH_PIN) > SW_IDLE) ? 1 : 0;
  if(!triggered) {
    motorStop();
  }
  return ret;
}

void advanceMotor(int pos = 1)
{        
    unsigned long advanceStart = millis();
    motorStart();

    lastSwitchState = getSwitchState();
    
    ledRed();
    for(int k=1;k<=pos;k++){   
      Serial.printf("Advancing position %d of %d", k, pos);
      while(getSwitchState() == lastSwitchState)
      {      
        ledOff();
        delay(50);
        ledRed();
        Serial.print(".");        
        if(!triggered) break;
      }
      lastSwitchState = getSwitchState();
      Serial.printf("Done.\n");
    }      
    feed_delay = millis() - advanceStart;
    Serial.printf("Feed Delay: %d\n", feed_delay);
    motorStop();
    ledGreen();    
    Serial.printf("All Done Feeding: %d\n", pos);
}  

#pragma endregion

void setup() {
  Serial.begin(115200);
  Serial.printf("Starting...");

  ledRedBlue();

  WiFi.hostname(hostName);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    os_printf("STA: Failed!\n");
  }

  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client);  

  FastLED.addLeds<NEOPIXEL, LED_PIN>(LED, NUM_LEDS);   

  for(int i=0;i<BUTTON_COUNT;i++){
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }

  pinMode(SWITCH_PIN, INPUT);

  pinMode(MOTOR_PINA, OUTPUT);
  
  digitalWrite(MOTOR_PINA, HIGH);
  
  pinMode(LED_PIN, OUTPUT);  
  
  int statusCode = ThingSpeak.readMultipleFields(myChannelNumber, READ_API_KEY);
  
  int zeros = 0;
  
  if(statusCode == 200){
    for(int i=0;i<fieldsLen;i++){
      data[i] = ThingSpeak.getFieldAsInt(fields[i]);
      Serial.printf("Field %d value: %d\n", fields[i], data[i]);
      if(data[i] == 0) zeros++;
    }
    
    if(zeros <= 2) {
      FEED_TRIGGER  = data[0];
      SNACK_TRIGGER = data[1];
      FEED_PORTION  = data[2];
      SNACK_PORTION = data[3];
      PORTION_SIZE  = data[4];
    } else {
      Serial.printf("Error with data. Too many zero values.\nFEED_TRIGGER: %d\nSNACK_TRIGGER: %d\nFEED_PORTION: %d\nSNACK_PORTION: %d");      

      delay((millis() - lastCall) + 2000);

      ThingSpeak.setField(1, 0);
      ThingSpeak.setField(2, 0);
      ThingSpeak.setField(3, (FEED_PORTION == 0)  ? 3 : FEED_PORTION);
      ThingSpeak.setField(4, (SNACK_PORTION == 0) ? 1 : SNACK_PORTION);
      ThingSpeak.setField(5, (PORTION_SIZE == 0)  ? 1 : PORTION_SIZE);
      
      ThingSpeak.setStatus("This update was a correction.");

      int statusCode = ThingSpeak.writeFields(myChannelNumber, WRITE_API_KEY);
      readyStatus = false;

    }
  }
  delay(30);
  ledOff();

  Serial.printf("Started.\n");    
  delay(30);
  ledGreen();    
}

void loop() {
  
  triggered = false; 
 
  currentMillis = millis();

  if(currentMillis - previousMillis >= one_second) {
    ledGreen();   
    int zeros = 0; 
    int statusCode = ThingSpeak.readMultipleFields(myChannelNumber, READ_API_KEY);
    
    if(statusCode == 200){
      for(int i=0;i<fieldsLen;i++){
        data[i] = ThingSpeak.getFieldAsInt(fields[i]);
        Serial.printf("Field %d value: %d\n", fields[i], data[i]);
        if(data[i] == 0) zeros++;
      }
      Serial.println("-");
      if(zeros<3) {
        FEED_TRIGGER  = data[0];
        SNACK_TRIGGER = data[1];
        FEED_PORTION  = data[2];
        SNACK_PORTION = data[3];
        PORTION_SIZE  = data[4];
      } else {
        Serial.printf("Error with data. Too many zero values.\nFEED_TRIGGER: %d\nSNACK_TRIGGER: %d\nFEED_PORTION: %d\nSNACK_PORTION: %d", data[0], data[1], data[2], data[3]);        
        ThingSpeak.setField(1, 0);
        ThingSpeak.setField(2, 0);
        ThingSpeak.setField(3, (FEED_PORTION == 0)  ? 3 : FEED_PORTION);
        ThingSpeak.setField(4, (SNACK_PORTION == 0) ? 1 : SNACK_PORTION);
        ThingSpeak.setField(5, (PORTION_SIZE == 0)  ? 5 : PORTION_SIZE);
      
        api_delay = call_interval - (millis() - lastCall);
        Serial.printf("Delay for: %d seconds for api update throttling\n", (api_delay / 1000));
        ThingSpeak.setStatus("This update was a correction.");
        ledBlinkDelay(CRGB::Yellow ,api_delay);
  
        int statusCode = ThingSpeak.writeFields(myChannelNumber, WRITE_API_KEY);
        if(statusCode == 200){
          lastCall = millis();
          readyStatus = false;
        }
      }
    }

    previousMillis = millis();
  }
 
  for(int i=0;i<BUTTON_COUNT;i++)
  {
    buttonState = digitalRead(BUTTON_PINS[i]);

    if(buttonState == 0) 
    {
      Serial.printf("You pressed button %d!\n", i);
      switch(i)
      {
        case 0: //snack
          ledGreenBlink();          
          SNACK_TRIGGER=1;
          lastTriggered = millis();
          break;
        case 1: //nothing
          break;
        case 2: // nothing
          break;        
        case 3: //
          ledRedBlue();
          motorStop();
          triggered = false;
          break;
        case 4: //feed
          ledBlueBlink();          
          FEED_TRIGGER=1;
          lastTriggered = millis();
          break;
      }
      ledGreen();
      while(digitalRead(BUTTON_PINS[i]) == 0) { delay(5); };
    }    
  }

  if(FEED_TRIGGER == 1) {
    Serial.printf("\nFeed triggered!\nFEED_PORTION: %d\nPORTION_SIZE: %d\n", FEED_PORTION, PORTION_SIZE);
    FEED_TRIGGER = 0;
    triggered = true;
    advanceMotor(FEED_PORTION  * PORTION_SIZE);
    api_delay = call_interval - feed_delay;
    Serial.printf("Delay for: %dms for api update throttling\n", api_delay);
    ledBlinkDelay(CRGB::Yellow, api_delay);
  } 

  if(SNACK_TRIGGER == 1) {
    Serial.printf("\nSnack triggered!\n\nSNACK_PORTION: %d\nPORTION_SIZE: %d\n", SNACK_PORTION, PORTION_SIZE);
    SNACK_TRIGGER = 0;
    triggered = true;
    advanceMotor(SNACK_PORTION  * PORTION_SIZE);
    api_delay = call_interval - feed_delay;
    Serial.printf("Delay for: %dms for api update throttling\n", api_delay);
    ledBlinkDelay(CRGB::Yellow, api_delay);
  }

  if(triggered){    

    triggered = false;

    ThingSpeak.setField(1, 0);
    ThingSpeak.setField(2, 0);
    ThingSpeak.setField(3, (FEED_PORTION == 0)  ? 3 : FEED_PORTION);
    ThingSpeak.setField(4, (SNACK_PORTION == 0) ? 1 : SNACK_PORTION);
    ThingSpeak.setField(5, (PORTION_SIZE == 0)  ? 1 : PORTION_SIZE);

    String statusMsg = "\nUnit Wrote: FEED_TRIGGER: " + String(FEED_TRIGGER) 
    + "\nSNACK_TRIGGER: " + String(SNACK_TRIGGER)
    + "\nFEED_PORTION: " + String(FEED_PORTION)
    + "\nSNACK_PORTION: " + String(SNACK_PORTION)
    + "\nPORTION_SIZE: " + String(PORTION_SIZE);

    ThingSpeak.setStatus(statusMsg);
    Serial.printf("%s", statusMsg);
      
    int statusCode = ThingSpeak.writeFields(myChannelNumber, WRITE_API_KEY);
      
    if(statusCode == 200){  
      lastCall = millis();
      readyStatus = false;
      }
  }

  if(((millis() - lastCall) > call_interval) && !readyStatus){
    Serial.printf("API Available for writing.\n");
    readyStatus = true;
    ledRedBlue();
  }

  
}
