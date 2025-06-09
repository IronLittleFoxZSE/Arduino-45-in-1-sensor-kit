#include <Servo.h>  // add the servo libraries
Servo myservo1;  // create servo object to control a servo
Servo myservo2;
Servo myservo3;
Servo myservo4;
int pos1=90, pos2=90, pos3=90, pos4=90;  // define the variable of 4 servo angle and assign the initial value( that is the boot posture angle value)
char val;
int incomingByte = 0;          // Received data byte
String inputString = "";         // Used to store received content
boolean newLineReceived = false; // Previous data end flag
boolean startBit  = false;  //Acceptance Agreement Start Sign
int num_reveice=0;
void setup()
{
   // boot posture
  myservo1.write(pos1);  
  delay(1000);
  myservo2.write(pos2);
  myservo3.write(pos3);
  myservo4.write(pos4);
  delay(1500);

  Serial.begin(9600); //  set the baud rate to 9600
}

void loop() 
{
  myservo1.attach(3);  // set the control pin of servo 1 to D3
  myservo2.attach(5);  // set the control pin of servo 2 to D5
  myservo3.attach(6);   // set the control pin of servo 3 to D6
  myservo4.attach(9);   // set the control pin of servo 4 to D9

while (Serial.available())
  {
    incomingByte = Serial.read();              //One byte by byte, the next sentence is read into a string array to form a completed packet
    if (incomingByte == '%')
    {
      num_reveice = 0;
      startBit = true;
    }
    if (startBit == true)
    {
      num_reveice++;
      inputString += (char) incomingByte;     
    }
    if (startBit == true && incomingByte == '#')
    {
      newLineReceived = true;
      startBit = false;
    }
    
    if(num_reveice >= 20)
    {
      num_reveice = 0;
      startBit = false;
      newLineReceived = false;
      inputString = "";
    }  
  }



if(newLineReceived)
{
    switch(inputString[1])   
    {
      case 'B':  T_left();  break;   // turn left
      case 'C':  T_right();  break;//turn right 
      case 'A':  RB();  break;// the lower arm will draw back 
      case 'D':  RF();  break;// the lower arm will  stretch out
      case '5':  ZK();  break;//close the claw
      case '6':  ZB();  break;//close the claw
      case '4':  LB();  break;//the upper arm will lift up 
      case '7':  LF();  break;//the upper arm will go down 
      default:break;
    }

      inputString = "";   // clear the string
      newLineReceived = false;
}   
   




  
}
//**************************************************
// turn left
void T_left()
{
    pos1=pos1+8;
    myservo1.write(pos1);
    delay(5);
    if(pos1>180)
    {
      pos1=180;
    }
}
//turn right 
void T_right()
{
    pos1=pos1-8;
    myservo1.write(pos1);
    delay(5);
    if(pos1<1)
    {
      pos1=1;
    }
}
//********************************************
//close the claw

void ZK()
{
      pos4=pos4-8;
      Serial.println(pos4);
      myservo4.write(pos4);
      delay(5);
      if(pos4<45)
      {
        pos4=45;
      }
}
// open the claw
void ZB()
{
    pos4=pos4+8;
      Serial.println(pos4);
      myservo4.write(pos4);
      delay(5);
      if(pos4>120)
      {
        pos4=120;
      }
}

//******************************************
// the lower arm will  stretch out
void RF()
{
    pos2=pos2-8;
    myservo2.write(pos2);
    delay(5);
    if(pos2<25)
    {
      pos2=25;
    }
}
// the lower arm will draw back 
void RB()
{
    pos2=pos2+8;
    myservo2.write(pos2);
    delay(5);
    if(pos2>180)
    {
      pos2=180;
    }
}

//***************************************
//the upper arm will lift up  
void LB()
{
  pos3=pos3+8;
    myservo3.write(pos3);
    delay(5);
    if(pos3>135)
    {
      pos3=135;
    }
}

//the upper arm will go down  

void LF()
{
  pos3=pos3-8;
    myservo3.write(pos3);
    delay(5);
    if(pos3<0)
    {
      pos3=0;
    }
}
