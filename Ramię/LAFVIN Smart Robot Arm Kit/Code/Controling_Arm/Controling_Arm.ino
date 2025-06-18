//#include <SoftwareSerial.h>
#include <Servo.h>

Servo myservo1;  // create servo object to control a servo
Servo myservo2;
Servo myservo3;
Servo myservo4;

//SoftwareSerial mySerial(11,10); // RX, TX

const int right_X = A2; // define the right X pin to A2
const int right_Y = A5; // define the right Y pin to A5
const int right_key = 7; // define the right key pin to 7（that is the value of Z）
const int left_X = A3; // define the left X pin to A3
const int left_Y = A4; // define the left X pin to A4
const int left_key = 8; //define the left key pin to 8（that is the value of Z）
int x1,y1,z1; // define the variable, used to save the joystick value it read.
int x2,y2,z2;

int pos1=90, pos2=90, pos3=90, pos4=90;

enum ActionCommand
{
  TurnLeft,
  TurnRight,
  OpenClaw,
  CloseClaw,
  LowerArmStretchOut,
  LowerArmStretchBack,
  UpperArmLiftUp, 
  UpperArmGoDown
};

struct CommandAction {
  char command;
  void (*action)();
  bool active; 
};

void turnLeft();
void turnRight();
void openClaw();
void closeClaw();
void lowerArmStretchOut();
void lowerArmStretchBack();
void upperArmLiftUp();
void upperArmGoDown();

CommandAction commandMap[] = {
  { 'B', turnLeft , false},
  { 'C', turnRight, false },
  { '5', openClaw, false },
  { '6', closeClaw, false },
  { 'D', lowerArmStretchOut, false },
  { 'A', lowerArmStretchBack, false },
  { '4', upperArmLiftUp, false },
  { '7', upperArmGoDown, false }
};

const int numCommands = sizeof(commandMap) / sizeof(commandMap[0]);

// turn left
void turnLeft()
{
  Serial.println("upperArmGoDown");
  pos1=pos1+8;
  myservo1.write(pos1);
  delay(5);
  if(pos1>180)
  {
    pos1=180;
  }
}
//turn right 
void turnRight()
{
  Serial.println("upperArmGoDown");
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
void closeClaw()
{
  Serial.println("upperArmGoDown");
  pos4=pos4-8;
  myservo4.write(pos4);
  delay(5);
  if(pos4<45)
  {
    pos4=45;
  }
}
// open the claw
void openClaw()
{
  Serial.println("upperArmGoDown");
  pos4=pos4+8;
  myservo4.write(pos4);
  delay(5);
  if(pos4>120)
  {
    pos4=120;
  }
}

//******************************************
// the lower arm will  stretch out
void lowerArmStretchOut()
{
  Serial.println("upperArmGoDown");
  pos2=pos2-8;
  myservo2.write(pos2);
  delay(5);
  if(pos2<25)
  {
    pos2=25;
  }
}
// the lower arm will draw back 
void lowerArmStretchBack()
{
  Serial.println("upperArmGoDown");
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
void upperArmLiftUp()
{
  Serial.println("upperArmGoDown");
  pos3=pos3+8;
  myservo3.write(pos3);
  delay(5);
  if(pos3>135)
  {
    pos3=135;
  }
}

//the upper arm will go down  
void upperArmGoDown()
{
  Serial.println("upperArmGoDown");
  pos3=pos3-8;
  myservo3.write(pos3);
  delay(5);
  if(pos3<0)
  {
    pos3=0;
  }
}

String getSerialData()
{
  String data = "";
  while (Serial.available() > 0) 
  {
    data = data + ((char)(Serial.read()));
    if (data.length() == 3)
      break;
    delay(2);
  }
  return data;
}

bool isSerialDataCorrect(String data)
{
  if (data.length() != 3
      || data.charAt(0) != '%'
      || data.charAt(data.length() - 1) != '#')
    return false;
  return true;
}

char getCommand(String data)
{
  return data.charAt(1);
}

void setup() 
{
  Serial.begin(9600);
  // put your setup code here, to run once:
  //mySerial.begin(9600);

  pinMode(right_key, INPUT); // set the right/left key to INPUT
  pinMode(left_key, INPUT);

  myservo1.attach(3);  // set the control pin of servo 1 to D3
  myservo2.attach(5);  // set the control pin of servo 2 to D5
  myservo3.attach(6);   // set the control pin of servo 3 to D6
  myservo4.attach(9);   // set the control pin of servo 4 to D9

  myservo1.write(90);
  delay(1000);
  myservo2.write(90);
  myservo3.write(90);
  myservo4.write(90);
}

void loop() 
{
  String serialData = getSerialData();
  if (isSerialDataCorrect(serialData))
  {
    char command = getCommand(serialData);
    Serial.println(command);
    if (command == 'B')
      commandMap[ActionCommand::TurnLeft].active = true;
    if (command == 'C')
      commandMap[ActionCommand::TurnRight].active = true;
    if (command == '5')
      commandMap[ActionCommand::CloseClaw].active = true;
    if (command == '6')
      commandMap[ActionCommand::OpenClaw].active = true;
    if (command == 'D')
      commandMap[ActionCommand::LowerArmStretchOut].active = true;
    if (command == 'A')
      commandMap[ActionCommand::LowerArmStretchBack].active = true;
    if (command == '4')
      commandMap[ActionCommand::UpperArmLiftUp].active = true;
    if (command == '7')
      commandMap[ActionCommand::UpperArmGoDown].active = true;
  }  

  x2 = analogRead(right_X); //read the right X value
  y2 = analogRead(right_Y); // read the right Y value
  z2 = digitalRead(right_key); //// read the right Z value
  x1 = analogRead(left_X); //read the left X value
  y1 = analogRead(left_Y); //read the left Y value
  z1 = digitalRead(left_key); // read the left Z value

  if (x1 < 50)
    commandMap[ActionCommand::OpenClaw].active = true;
  if (x1 > 1000)
    commandMap[ActionCommand::CloseClaw].active = true;

  if (x2 < 50)
    commandMap[ActionCommand::TurnLeft].active = true;
  if (x2 > 1000)
    commandMap[ActionCommand::TurnRight].active = true;

  if (y2 < 50)
    commandMap[ActionCommand::LowerArmStretchOut].active = true;
  if (y2 > 1000)
    commandMap[ActionCommand::LowerArmStretchBack].active = true;

  if (y1 < 50)
    commandMap[ActionCommand::UpperArmGoDown].active = true;
  if (y1 > 1000)
    commandMap[ActionCommand::UpperArmLiftUp].active = true;

  
  for (int i = 0; i < numCommands; i++) 
  {
    if (commandMap[i].active) 
    {
      commandMap[i].active = false;
      commandMap[i].action();
    }
  }
  delay(100);
}