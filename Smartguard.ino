#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <ezButton.h>
#include <LiquidCrystal.h> // include the library code
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define USE_ARDUINO_INTERRUPTS true // Set-up low-level interrupts for most acurate BPM math
#include <PulseSensorPlayground.h> // Includes the PulseSensorPlayground Library

TinyGPSPlus gps;
SoftwareSerial SIM800A(10,11);
SoftwareSerial ss(12,13);
double flat,flong;

ezButton toggleSwitch(8);  // create ezButton object that attach to pin 7;

char array1[]=" ALCOHOL  LIMIT "; //the string to print on the LCD
char array2[]="    EXCEEDED    "; //the string to print on the LCD
int tim = 250; //the value of delay time, 500 for 250 ms or 2 for each 1 ms
// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display

float sensorValue;

PulseSensorPlayground pulseSensor;  // Creates an object
const int PulseWire = 0;       // 'S' Signal pin connected to A0
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore

int relayPin = 7; // Pin to control the relay

int enA=3;
int in1=2;
int in2=4;

void setup() {
  toggleSwitch.setDebounceTime(50); // set debounce time to 50 milliseconds

  pinMode(9, OUTPUT);   //Defining the pin as OUTPUT
  lcd.init();  //initialize the lcd
  lcd.backlight(); //open the backlight
  pinMode(relayPin, OUTPUT);
  Serial.begin (9600);
  SIM800A.begin(9600);
  ss.begin(9600);
  pinMode(enA,OUTPUT);
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pulseSensor.analogInput(PulseWire); 
  pulseSensor.setThreshold(Threshold);   // Double-check the "pulseSensor" object was created and began seeing a signal
  if (pulseSensor.begin()) {
  }
}

void loop() {
  toggleSwitch.loop(); // MUST call the loop() function first
  if (toggleSwitch.isPressed())
    Serial.println("The Switch: OFF -> ON");
  if (toggleSwitch.isReleased())
    Serial.println("The Switch: ON -> OFF");
    int state = toggleSwitch.getState();
  sensorValue = analogRead(1); // read analog input pin 0
  int myBPM = pulseSensor.getBeatsPerMinute();      // Calculates BPM
  if (state == LOW){
    if (sensorValue > 100.00) {
      Serial.print("Alcohol Level: ");
      Serial.println(sensorValue);
      Serial.println("Status:DRUNK");
     
      digitalWrite(9, HIGH);   //Setting the pin HIGH
      lcd.setCursor(0,0); // set the cursor to column 15, line 0
      for (int positionCounter1 = 0; positionCounter1 < 16; positionCounter1++)
      {
      lcd.print(array1[positionCounter1]); // Print a message to the LCD.
      }
      lcd.setCursor(0,1); // set the cursor to column 15, line 1
      for (int positionCounter = 0; positionCounter < 16; positionCounter++)
      {
      lcd.print(array2[positionCounter]); // Print a message to the LCD.
      }
      digitalWrite(relayPin, HIGH); // Turn off the relay
    }
   
   else
   {
    Serial.print("Alcohol Level: ");
    Serial.println(sensorValue);
    Serial.println("Status: NOT DRUNK");
    delay(1000);
    digitalWrite(relayPin, LOW);
  //    delay(2000);
    Serial.print("Heart Beat is: ");
    Serial.println(myBPM);
    if((myBPM<110) && myBPM !=0){
      Serial.println("DRIVING SLOWLY");
      analogWrite(enA,60);
      digitalWrite(in1,HIGH);
      digitalWrite(in2,LOW);
      delay(20);
  
      digitalWrite(9, LOW);    //Setting the pin HIGH

      while (ss.available()>0)
      if (gps.encode(ss.read())){
        Serial.print(F("Location: ")); 
        if (gps.location.isValid()) {
          flat = gps.location.lat();
          flong = gps.location.lng();
          delay(500);
        } 
      else {
        Serial.println(F("INVALID"));
      }
      }
        if (millis() > 5000 && gps.charsProcessed() < 10) {
      Serial.println(F("No GPS detected: check wiring."));
      while(true);
      }
      
      Serial.println ("Sending Message");   
      SIM800A.println("AT+CMGF=1");  //Sets the GSM Module in Text Mode  
      delay(1000);  
      Serial.println ("Set SMS Number");  
      SIM800A.println("AT+CMGS=\"+917893366621\"\r"); //Type Your Mobile number to send message  
      delay(1000);  
      Serial.println ("Set SMS Content");  
      SIM800A.println("BP is abnormal..ALERT");// Messsage content  
      //SIM800A.print("http://maps.google.com/maps?q=loc:");
      delay(1000);
      SIM800A.print(flat);  
      delay(1000);
      SIM800A.print(",");
      delay(1000);
      SIM800A.print(flong);
      Serial.println ("Finish");  
      delay(1000);
      SIM800A.println((char)26);// ASCII code of CTRL+Z  
      delay(1000);  
      Serial.println ("Message has been sent ->SMS prasanna");
      

    } // BPM
    else{
    Serial.println("DRIVING FAST");
    delay(1000);
    analogWrite(enA,250);
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    delay(20);
    }
  } //NOT DRUNK
}//IGNITION ON
else{
    Serial.println("The Ignition is OFF");
    delay(1000);
    digitalWrite(relayPin, HIGH); // Turn off the relay
    analogWrite(enA,0);
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    }
}//LOOP
