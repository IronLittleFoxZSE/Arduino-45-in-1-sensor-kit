#include <IRremote.h>

#define LED1 13
#define LED2 12
#define LED3 11
#define LED4 10

#define   BUTTON1   A1 
#define   BUTTON2   A2 
#define   BUTTON3   A3

#define   Pot1   0 

#define LATCH_DIO 4
#define CLK_DIO 7
#define DATA_DIO 8 

#define BUZZER 3

#define IR_RECV 2

const byte SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90}; 
const byte SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};

//czujnik tyłem do wyświetlacza.
IRrecv irrecv(IR_RECV);
decode_results results;

void setup() 
{
  Serial.begin(9600);

  pinMode(LED1, OUTPUT);  
  pinMode(LED2, OUTPUT); 
  pinMode(LED3, OUTPUT); 
  pinMode(LED4, OUTPUT); 

  pinMode(LATCH_DIO,OUTPUT); 
  pinMode(CLK_DIO,OUTPUT); 
  pinMode(DATA_DIO,OUTPUT); 

  pinMode(BUZZER, OUTPUT);

  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);

  irrecv.enableIRIn();

}

void loop() 
{
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }

  if(digitalRead(BUTTON1) == LOW)
  {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
  }
  else
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
  }

  if(digitalRead(BUTTON2) == LOW)
  {
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(BUZZER, LOW);
  }
  else
  {
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    digitalWrite(BUZZER, HIGH);
  }

  if(digitalRead(BUTTON3) == LOW)
  {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
  }
  else
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
  }

  int PotValue = analogRead(Pot1);
  WriteNumberToSegment(0 , PotValue / 1000);
  WriteNumberToSegment(1 , (PotValue / 100) % 10);
  WriteNumberToSegment(2 , (PotValue / 10) % 10); 
  WriteNumberToSegment(3 , PotValue % 10);

}

void WriteNumberToSegment(byte Segment, byte Value) 
{ 
  digitalWrite(LATCH_DIO,LOW);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_MAP[Value]); 
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[Segment] ); 
  digitalWrite(LATCH_DIO,HIGH); 
}