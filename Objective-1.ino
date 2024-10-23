#define Sober 200   // Define min value that we consider sober
#define Drunk 400   // Define max value that we consider drunk
#define MQ3 0
#define ledPin 6
int buzzer=10;    //Defining variable and the GPIO pin on Arduino
float sensorValue;
#include <LiquidCrystal.h> // include the library code
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
char array1[]="alcohol limit"; //the string to print on the LCD
char array2[]="limit exceeded"; //the string to print on the LCD
int tim = 250; //the value of delay time, 500 for 250 ms or 2 for each 1 ms
// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display
//int relayPin = 7; // Pin to control the relay
void setup() {
  Serial.begin(9600); // sets the serial port to 9600
 // pinMode(ledPin, OUTPUT);
  //digitalWrite(ledPin, HIGH);
 // delay(2000); // put your setup code here, to run once:
  pinMode(buzzer, OUTPUT);   //Defining the pin as OUTPUT
  lcd.init();  //initialize the lcd
  lcd.backlight(); //open the backlight
  //pinMode(relayPin, OUTPUT);
}

void loop() {
  //digitalWrite(relayPin,HIGH); // Turn on the relay
  delay(1000); // Wait for a second
  sensorValue = analogRead(MQ3); // read analog input pin 0
  Serial.println(sensorValue);    // Return analog moisture value
 if (sensorValue > 100.00)
 {
  Serial.println("Status: DRUNK");
  analogWrite(ledPin, sensorValue); // generate PWM signal
  digitalWrite(buzzer, HIGH);   //Setting the pin HIGH
  delay(1000);
  digitalWrite(buzzer, LOW);    //Setting the pin HIGH
  delay(1000);
  lcd.setCursor(0,0); // set the cursor to column 15, line 0
  for (int positionCounter1 = 0; positionCounter1 < 12; positionCounter1++)
  {
  lcd.print(array1[positionCounter1]); // Print a message to the LCD.
  }
  lcd.setCursor(0,1); // set the cursor to column 15, line 1
  for (int positionCounter = 0; positionCounter<14; positionCounter++)
  {
  lcd.print(array2[positionCounter]); // Print a message to the LCD.
  }
  delay(1000);//wait for tim which is set above
  lcd.clear(); //Clears the LCD screen and positions the cursor in the upper-left corner.
//  digitalWrite(relayPin, HIGH); // Turn off the relay
  delay(1000);// Wait for a second
 }
 else
 {
  digitalWrite(buzzer, LOW);
 }
  /*
  //digitalWrite(ledPin, LOW);
  //delay(1000);
  digitalWrite(ledPin, HIGH);
  //delay(1000);
 // if (button == HIGH)
  //begin

   digitalWrite(relayPin, LOW);
    delay(2000);
  //end
  
  delay(2000); // wait 2s for next reading */

}
