#include <Servo.h>
Servo myservo;  // create servo object to control a servo

void setup()
{
  Serial.begin(9600); 
  delay(1000);
}

void loop() 
{
  myservo.attach(3);  // modify each pin to adjust 
  myservo.write(0);  // the servo will automatically rotate to 0°
  //myservo.write(90);  //  the servo will automatically rotate to 90°
  //myservo.write(180);  //  the servo will automatically rotate to 180°
  delay(1000);
  
}
