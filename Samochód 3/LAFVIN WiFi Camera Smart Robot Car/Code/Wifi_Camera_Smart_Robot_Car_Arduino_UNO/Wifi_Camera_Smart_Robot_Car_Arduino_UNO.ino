#include <IRremote.h>
IRrecv irrecv(3);

#define IR_Go           0x00FF18E7 //going forward
#define IR_Back         0x00FF4AB5  //going backward
#define IR_Left         0x00FF10EF//turning left
#define IR_Right        0x00FF5AA5  //turning right
#define IR_Stop         0x00FF38C7  //stop 
#define IR_CAMERA_UP    0x00FF6897
#define IR_CAMERA_DOWN  0x00FFB04F

#include <Servo.h>

#define CAMERA_PIN  10
Servo servoCamera;
volatile float cameraAngle;

#define LEFT_TRACKING_PIN 7
#define CENTER_TRACKING_PIN 8
#define RIGHT_TRACKING_PIN 7

volatile int leftTrackingValue;
volatile int centerTrackingValue;
volatile int rightTrackingValue;
volatile int Black = 1;

#define LEFT_MOTOR_PVM_PIN 5
#define RIGHT_MOTOR_PVM_PIN 6
#define MOTOR_CONTROL_1_PIN 2
#define MOTOR_CONTROL_2_PIN 4

#define ULTRASONIC_TRIG_PIN 12
#define ULTRASONIC_ECHO_PIN 13

String serialData;
char command;

void Infrared_Tracing()
{
  leftTrackingValue = digitalRead(LEFT_TRACKING_PIN);
  centerTrackingValue = digitalRead(CENTER_TRACKING_PIN);
  rightTrackingValue = digitalRead(RIGHT_TRACKING_PIN);
  if (leftTrackingValue != Black && centerTrackingValue == Black && rightTrackingValue != Black)
    Move_Forward(120);
  else if (leftTrackingValue == Black && centerTrackingValue == Black && rightTrackingValue != Black)
    Rotate_Left(80);
  else if (leftTrackingValue == Black && centerTrackingValue != Black && rightTrackingValue != Black)
    Rotate_Left(120);
  else if (leftTrackingValue != Black && centerTrackingValue != Black && rightTrackingValue == Black)
    Rotate_Right(120);
  else if (leftTrackingValue != Black && centerTrackingValue == Black && rightTrackingValue == Black)
    Rotate_Right(80);
  else if (leftTrackingValue == Black && centerTrackingValue == Black && rightTrackingValue == Black)
    STOP();
}

void Set_Speed(int speed)
{
  analogWrite(LEFT_MOTOR_PVM_PIN,speed);
  analogWrite(RIGHT_MOTOR_PVM_PIN,speed);
}

void Move_Forward(int car_speed) 
{
  digitalWrite(MOTOR_CONTROL_1_PIN,HIGH);
  digitalWrite(MOTOR_CONTROL_2_PIN,LOW);
  Set_Speed(car_speed);
}

void Move_Backward(int car_speed) 
{
  digitalWrite(MOTOR_CONTROL_1_PIN,LOW);
  digitalWrite(MOTOR_CONTROL_2_PIN,HIGH);
  Set_Speed(car_speed);
}

void Rotate_Left(int car_speed) 
{
  digitalWrite(MOTOR_CONTROL_1_PIN,LOW);
  digitalWrite(MOTOR_CONTROL_2_PIN,LOW);
  Set_Speed(car_speed);
}

void Rotate_Right(int car_speed) 
{
  digitalWrite(MOTOR_CONTROL_1_PIN,HIGH);
  digitalWrite(MOTOR_CONTROL_2_PIN,HIGH);
  Set_Speed(car_speed);
}

void STOP() 
{
  Set_Speed(0);
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
  int frontDistance = checkdistance();
  if (frontDistance <= 10) 
  {
    Move_Backward(100);
    delay(200);
    if (50 >= random(1, 100)) 
    {
      Rotate_Left(100);
    } 
    else 
    {
      Rotate_Right(100);
    }
    delay(500);
  } 
  else if (frontDistance >= 10 && frontDistance <= 20) 
  {
    STOP();
    delay(200);
    if (50 >= random(1, 100)) 
    {
      Rotate_Left(100);
    } 
    else 
    {
      Rotate_Right(100);
    }
    delay(500);
  }
  else if (frontDistance > 20) 
  {
    Move_Forward(70);
  }
}

void Ultrasonic_Follow() 
{
  int frontDistance = checkdistance();
  if (frontDistance <= 5) 
  {
    Move_Backward(80);
  } 
  else if (frontDistance >= 5 && frontDistance <= 10) 
  {
    STOP();
  } 
  else if (frontDistance > 20) 
  {
    Move_Forward(100);
  }
}

void IR_remote_control()
{
  decode_results results;
  if(irrecv.decode(&results)) 
  {
    Serial.println(results.value);
    switch(results.value)
    {
      case IR_Go:
        Move_Forward(110);
        delay(300);
        STOP();
        break;
      case IR_Back: 
        Move_Backward(110);
        delay(300);
        STOP();
        break;
      case IR_Left:
        Rotate_Left(110);
        delay(300);
        STOP();
        break;
      case IR_Right:
        Rotate_Right(110);
        delay(300);
        STOP();
        break;
      case IR_Stop:
        STOP();
        break;
      case IR_CAMERA_UP:
        cameraUp();
        break;
      case IR_CAMERA_DOWN:
        cameraDown();
        break;
      default: 
        break;      
    } 
    irrecv.resume(); // Receive the next value
  }
}

String getData()
{
  String data = "";
  while (Serial.available() > 0) 
  {
    data = data + ((char)(Serial.read()));
    if (String(data).length() == 3)
      break;
    delay(2);
  }
  return data;
}

bool isDataCorrect(String data)
{
  if (String(data).length() == 0
      || String(data).length() > 3
      || String(data).charAt(0) != '%'
      || String(data).charAt((String(data).length() - 1)) != '#')
    return false;
  return true;
}

char getCommand(String data)
{
  return String(data).charAt(1);
}

void cameraUp()
{
  cameraAngle = cameraAngle + 4;
  if (cameraAngle >= 180)
    cameraAngle = 180;
  servoCamera.write(round(cameraAngle));
}

void cameraDown()
{
  cameraAngle = cameraAngle - 4;
  if (cameraAngle <= 0)
    cameraAngle = 0;
  servoCamera.write(round(cameraAngle));
}

void setup()
{
  Serial.begin(115200);
  
  irrecv.enableIRIn(); 

  cameraAngle = 90;
  servoCamera.attach(CAMERA_PIN);
  servoCamera.write(cameraAngle);
  
  pinMode(LEFT_TRACKING_PIN, INPUT);
  pinMode(CENTER_TRACKING_PIN, INPUT);
  pinMode(RIGHT_TRACKING_PIN, INPUT);

  pinMode(MOTOR_CONTROL_1_PIN, OUTPUT);
  pinMode(MOTOR_CONTROL_2_PIN, OUTPUT);
  pinMode(LEFT_MOTOR_PVM_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_PVM_PIN, OUTPUT);

  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
}



void loop()
{
  IR_remote_control();

  serialData = getData();
  if (!isDataCorrect(serialData))
  {
    STOP();
    return;
  }

  command = getCommand(serialData);
  switch (command) {
    case 'H':
      cameraUp();
      break;
    case 'G':
      cameraDown();
      break;
    case 'F':
      Move_Forward(110);
      delay(400);
      break;
    case 'B':
      Move_Backward(110);
      delay(400);
      break;
    case 'L':
      Rotate_Left(110);
      delay(250);
      break;
    case 'R':
      Rotate_Right(110);
      delay(250);
      break;
    case 'T':
      Infrared_Tracing();
      break;
    case 'S':
      STOP();
      break;
    case 'A':
      Ultrasonic_Avoidance();
      break;
    case 'Z':
      Ultrasonic_Follow();
      break;
  }
}
