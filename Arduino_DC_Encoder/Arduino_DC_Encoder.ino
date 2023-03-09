
//The sample code for driving one way motor encoder
const byte encoder0pinA = 2;//A pin -> the interrupt pin 0
const byte encoder0pinB = 3;//B pin -> the digital pin 3
byte encoder0PinALast;
int duration;//the number of the pulses
boolean Direction;//the rotation direction

int motorPin1 = 5;
int motorPin2 = 6;

void setup()
{
  Serial.begin(57600);//Initialize the serial port
  EncoderInit();//Initialize the module
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
}

void loop()
{
  Serial.print("Pulse: ");
  Serial.println(duration);
  duration = 0;
  delay(100);

  int speed = 50;

  //Motor Control A in both directions 
  analogWrite(motorPin1, speed); 
  delay(3000);
  analogWrite(motorPin1, 0); 
  delay(500);
  analogWrite(motorPin2, speed); 
  delay(3000);
  analogWrite(motorPin2, 0);

  //val_z = analogRead(A_Z)/4;
  //analogWrite(motorPin1, speed);

}

void EncoderInit()
{
  Direction = true;//default -> Forward
  pinMode(encoder0pinB,INPUT);
  attachInterrupt(0, wheelSpeed, CHANGE);
}

void wheelSpeed()
{
  int Lstate = digitalRead(encoder0pinA);
  if((encoder0PinALast == LOW) && Lstate==HIGH)
  {
    int val = digitalRead(encoder0pinB);
    if(val == LOW && Direction)
    {
      Direction = false; //Reverse
    }
    else if(val == HIGH && !Direction)
    {
      Direction = true;  //Forward
    }
  }
  encoder0PinALast = Lstate;

  if(!Direction)  duration++;
  else  duration--;
}