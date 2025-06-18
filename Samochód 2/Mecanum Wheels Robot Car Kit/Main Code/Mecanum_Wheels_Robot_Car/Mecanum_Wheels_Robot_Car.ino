#include <SoftwareSerial.h>

#define ULTRASONIC_TRIG_PIN A3
#define ULTRASONIC_ECHO_PIN A2

#define MOTOR_2A_PWM  11
#define MOTOR_2B_PWM  3
#define MOTOR_0A_PWM  6
#define MOTOR_0B_PWM  5

#define MOTOR_DIR_LATCH_PIN     12
#define MOTOR_DIR_CLOCK_PIN     4
#define MOTOR_DIR_DATA_PIN      8
#define MOTOR_DRIVER_ENABLE_PIN 7

#define MOVE_FORWARD         39
#define MOVE_BACKWARD       216
#define LEFT_MOVE           116
#define RIGHT_MOVE          139
#define RIGHT_ROTATE        149
#define LEFT_ROTATE         106
#define STOP                  0
#define UPPER_LEFT_MOVE      36
#define UPPER_RIGHT_MOVE      3
#define LOWER_LEFT_MOVE      80
#define LOWER_RIGHT_MOVE    136
#define DRIFT_LEFT           20
#define DRIFT_RIGHT          10

SoftwareSerial mySerial(A0,A1); // RX, TX

int Speed1 = 255;
int Speed2 = 255;
int Speed3 = 255;
int Speed4 = 255;

String serialData;
char command;

struct CommandAction {
  char command;
  void (*action)();
};

void Motor(int Dir,int Speed1,int Speed2,int Speed3,int Speed4);
void Ultrasonic_Avoidance();
void Ultrasonic_Follow();
void MotorStop();

void action_A() { Motor(MOVE_FORWARD, Speed1, Speed2, Speed3, Speed4); delay(200); }
void action_B() { Motor(MOVE_BACKWARD, Speed1, Speed2, Speed3, Speed4); delay(200); }
void action_C() { Motor(LEFT_MOVE, Speed1, Speed2, Speed3, Speed4); delay(200); }
void action_D() { Motor(RIGHT_MOVE, Speed1, Speed2, Speed3, Speed4); delay(200); }
void action_E() { Motor(LEFT_ROTATE, Speed1, Speed2, Speed3, Speed4); delay(100); }
void action_F() { Motor(RIGHT_ROTATE, Speed1, Speed2, Speed3, Speed4); delay(100); }
void action_G() { Motor(UPPER_LEFT_MOVE, Speed1, Speed2, Speed3, Speed4); delay(300); }
void action_H() { Motor(UPPER_RIGHT_MOVE, Speed1, Speed2, Speed3, Speed4); delay(300); }
void action_I() { Motor(LOWER_LEFT_MOVE, Speed1, Speed2, Speed3, Speed4); delay(300); }
void action_J() { Motor(LOWER_RIGHT_MOVE, Speed1, Speed2, Speed3, Speed4); delay(300); }
void action_K() { Motor(DRIFT_LEFT, Speed1, Speed2, Speed3, Speed4); delay(300); }
void action_L() { Motor(DRIFT_RIGHT, Speed1, Speed2, Speed3, Speed4); delay(300); }
void action_T() { Ultrasonic_Avoidance(); }
void action_W() { Ultrasonic_Follow(); }
void action_S() { MotorStop(); }

CommandAction commandMap[] = {
  { 'A', action_A },
  { 'B', action_B },
  { 'C', action_C },
  { 'D', action_D },
  { 'E', action_E },
  { 'F', action_F },
  { 'G', action_G },
  { 'H', action_H },
  { 'I', action_I },
  { 'J', action_J },
  { 'K', action_K },
  { 'L', action_L },
  { 'T', action_T },
  { 'W', action_W },
  { 'S', action_S }
};

const int numCommands = sizeof(commandMap) / sizeof(commandMap[0]);

void Motor(int Dir,int Speed1,int Speed2,int Speed3,int Speed4)
{
    analogWrite(MOTOR_2A_PWM,Speed1); //Motor PWM speed regulation
    analogWrite(MOTOR_2B_PWM,Speed2); //Motor PWM speed regulation
    analogWrite(MOTOR_0A_PWM,Speed3); //Motor PWM speed regulation
    analogWrite(MOTOR_0B_PWM,Speed4); //Motor PWM speed regulation
    
    digitalWrite(MOTOR_DIR_LATCH_PIN,LOW); //DIR_LATCH sets the low level and writes the direction of motion in preparation
    shiftOut(MOTOR_DIR_DATA_PIN,MOTOR_DIR_CLOCK_PIN,MSBFIRST,Dir);//Write Dir motion direction value
    digitalWrite(MOTOR_DIR_LATCH_PIN,HIGH);//DIR_LATCH sets the high level and outputs the direction of motion
}

void MotorStop()
{
    Motor(STOP, 0, 0, 0, 0);
}

float checkdistance() 
{
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  float distance = pulseIn(ULTRASONIC_ECHO_PIN, HIGH) / 58.00;
  delay(10);
  return distance;
}

void Ultrasonic_Avoidance() 
{ 
  int distance = checkdistance();
  if (distance > 0 && distance <= 45) 
  {
    MotorStop();
    delay(250);
    if (distance <= 20) 
    {
      Motor(MOVE_BACKWARD,Speed1,Speed2,Speed3,Speed4);
      delay(200);
      MotorStop();
      delay(250);
    }
    if (random(10) % 2 == 0) 
      Motor(LEFT_ROTATE,Speed1,Speed2,Speed3,Speed4);
    else 
      Motor(RIGHT_ROTATE,Speed1,Speed2,Speed3,Speed4); 
    delay(100);
    MotorStop();
    delay(250);
  } 
  else 
    Motor(MOVE_FORWARD,100,100,100,100);
}

void Ultrasonic_Follow() 
{
  int distance = checkdistance();
  if (distance >=0 && distance <= 10)
    Motor(MOVE_BACKWARD,Speed1,Speed2,Speed3,Speed4);
  else if (distance > 10 && distance <= 15) 
    MotorStop();
  else
    Motor(MOVE_FORWARD,170,170,170,170);
  delay(20);
}

String getData()
{
  String data = "";
  while (mySerial.available() > 0) 
  {
    data = data + ((char)(mySerial.read()));
    if (data.length() == 3)
      break;
    delay(2);
  }
  return data;
}

bool isDataCorrect(String data)
{
  if (data.length() == 0
      || data.length() > 3
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
  Serial.begin(9600);//Set the serial port baud rate 9600
  mySerial.begin(9600);

  randomSeed(analogRead(0));

  //Configure as output mode
  pinMode(MOTOR_DIR_CLOCK_PIN,OUTPUT);
  pinMode(MOTOR_DIR_DATA_PIN,OUTPUT);
  pinMode(MOTOR_DRIVER_ENABLE_PIN,OUTPUT);
  pinMode(MOTOR_DIR_LATCH_PIN,OUTPUT);
  pinMode(MOTOR_0B_PWM,OUTPUT);
  pinMode(MOTOR_0A_PWM,OUTPUT);
  pinMode(MOTOR_2A_PWM,OUTPUT);
  pinMode(MOTOR_2B_PWM,OUTPUT);
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
}

void loop()
{
  serialData = getData();
  if (!isDataCorrect(serialData))
  {
    MotorStop();
    return;
  }

  command = getCommand(serialData);

  bool found = false;
  for (int i = 0; i < numCommands; i++) 
  {
    if (commandMap[i].command == command) 
    {
      commandMap[i].action();
      found = true;
      break;
    }
  }
  if (!found)
    MotorStop(); // nieznana komenda – zatrzymaj
}
