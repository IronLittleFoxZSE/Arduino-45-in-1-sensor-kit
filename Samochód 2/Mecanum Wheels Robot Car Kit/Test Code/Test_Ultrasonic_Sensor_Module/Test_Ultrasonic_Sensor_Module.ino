/*
    Ultrasonic sensor Pins:
        VCC : +5
        Trig: A3
        Echo: A2
        GND : GND
 */

long distance;
const int Trig = A3;
const int Echo= A2; 


float checkdistance() 
{
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  float distance = pulseIn(Echo, HIGH) / 58.00;
  delay(10);
  return distance;
}

void setup() {
  //Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
}
 
void loop() {
  distance = checkdistance();
  Serial.print(distance);
  Serial.println("cm");
  delay(1000);
}
