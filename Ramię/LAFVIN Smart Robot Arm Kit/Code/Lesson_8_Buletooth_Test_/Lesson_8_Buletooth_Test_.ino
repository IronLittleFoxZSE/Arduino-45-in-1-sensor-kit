int pos1=90, pos2=90, pos3=90, pos4=90;  // define the variable of 4 servo angle and assign the initial value( that is the boot posture angle value)
char val;
int incomingByte = 0;          // Received data byte
String inputString = "";         // Used to store received content
boolean newLineReceived = false; //  Previous data end flag
boolean startBit  = false;  //Acceptance Agreement Start Sign
int num_reveice=0;
void setup()
{
  Serial.begin(9600); //  set the baud rate to 9600
}

void loop() 
{
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
      Serial.println(inputString);
      inputString = "";   // clear the string
      newLineReceived = false;
}   
   

















  
}
