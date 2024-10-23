int buzzer=10;
void setup()
{
  pinMode(buzzer,OUTPUT);  
}
void loop()
{
  digitalWrite(buzzer,HIGH);
  delay(2000);
  digitalWrite(buzzer,LOW);
  delay(2000);
}
