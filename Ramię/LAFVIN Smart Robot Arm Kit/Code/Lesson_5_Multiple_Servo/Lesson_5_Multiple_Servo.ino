#include <Servo.h>
Servo myservo1;  // create servo object to control a servo
Servo myservo2;
Servo myservo3;
Servo myservo4;
int pos1=90, pos2=90, pos3=90, pos4=90; 

void setup()
{
  myservo1.attach(3);    // set the control pin of servo 1 to 3 digital I/0
  myservo2.attach(5);    // set the control pin of servo 1 to 3 digital I/0
  myservo3.attach(6);    // set the control pin of servo 1 to 3 digital I/0
  myservo4.attach(9);    // set the control pin of servo 1 to 3 digital I/0
  
  myservo1.write(pos1);
  myservo2.write(pos2);
  myservo3.write(pos3);
  myservo4.write(pos4);
  delay(1500);
}

void loop() 
{

  // close the claw 
  for(pos4;pos4>45;pos4--)
  {
    myservo4.write(pos4);
  }
  delay(1000);
  
 // open the claw
 for(pos4;pos4<120;pos4++)
 {
   myservo4.write(pos4);
 }
  delay(1000);


// turn right
  for(pos1;pos1>30;pos1--)
  {
    myservo1.write(pos1);
    delay(5);      // delay 5ms（used to adjust the servo speed）
  }
  delay(1000);



   // turn to left 
  for(pos1;pos1<150;pos1++)
  {
    myservo1.write(pos1);
    delay(5);
  }
  delay(1000);

  //  stretch out the arm
  for(pos2;pos2<130;pos2++)
  {
    myservo2.write(pos2);
    delay(5);
  }
  delay(1000);


  // retracte the arm
  for(pos2;pos2>80;pos2--)
  {
    myservo2.write(pos2);
    delay(5);
  }
  delay(1000);




  // raise the arm 
  for(pos3;pos3<100;pos3++)
  {
    myservo3.write(pos3);
    delay(5);
  }
  delay(1500);

 // Lower the arm 
  for(pos3;pos3>40;pos3--)
  {
    myservo3.write(pos3);
    delay(5);
  }
  delay(1000);

}
  
  
 
  
  



  
  


 

 
 
