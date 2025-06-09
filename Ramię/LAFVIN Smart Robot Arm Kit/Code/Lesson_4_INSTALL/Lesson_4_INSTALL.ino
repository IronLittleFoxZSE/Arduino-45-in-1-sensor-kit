#include <Servo.h>
Servo myservo1; // create servo object to control a servo
Servo myservo2;
Servo myservo3;
Servo myservo4;
int pos1=90, pos2=90, pos3=90, pos4=90; //define the variable of 4 servo angle and assign the initial value for installing
void setup()
{
  Serial.begin(9600);
  myservo1.attach(3); // set the control pin of servo 1 to 3 digital I/0
  myservo2.attach(5); // set the control pin of servo 2 to 5 digital I/0
  myservo3.attach(6); // set the control pin of servo 3 to 6 digital I/0
  myservo4.attach(9); // set the control pin of servo 4 to 9 digital I/0
  delay(1000);
}

void loop() 
{
  myservo1.write(pos1);  // Control servo motor rotation to specified angle
  myservo2.write(pos2);
  myservo3.write(pos3);
  myservo4.write(pos4);
  delay(1000);
 
  
}
