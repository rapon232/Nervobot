#include<Wire.h>

int inputPin1 = 5;
int neuronPin1 = 13;
int neuronPin2 = 12;

unsigned long startMillis;
unsigned long currentMillis;

int frequency;
float highPeriod;  
float lowPeriod;
float cycle;

int angle;

bool inputState = LOW;
bool neuronState = LOW;

bool fire1, fire2;

void fireNeurons(float frequency)
{
  Serial.println("fired");

  bool exit = false;

  if(exit == false)
  {
    float cycle = 1000/frequency;
    lowPeriod  = 5*cycle/6;
    highPeriod = cycle/6;

    Serial.println(cycle);
    Serial.println(lowPeriod);
    Serial.println(highPeriod);
    exit = true;
  }

  if(neuronState == LOW)
  {
    if (currentMillis - startMillis >= lowPeriod)
    {
      neuronState = HIGH;
      digitalWrite(neuronPin1, LOW);
      startMillis = currentMillis;
    }
  }
  else
  {
    if (currentMillis - startMillis >= highPeriod)
    {
      neuronState = LOW;
      digitalWrite(neuronPin1, HIGH);
      startMillis = currentMillis;
    }
  }
}

void receiveEvent(int bytes) 
{
  angle = Wire.read();    // read one character from the I2C

  if (angle > 127) {
		angle = 256 - angle;
		angle *= -1;
	}

  Serial.println(angle);
}


void setup() 
{
  pinMode(neuronPin1, OUTPUT);
  pinMode(neuronPin2, OUTPUT);

  pinMode(inputPin1, INPUT);

  startMillis = millis();

  Serial.begin(9600);

  frequency = 1;
  cycle = 1000/frequency;
  lowPeriod  = 5*cycle/6;
  highPeriod = cycle/6;

  Serial.print("Frequency: ");
  Serial.print(frequency);
  Serial.print(" , ");
  Serial.print("Cycle Period: ");
  Serial.print(cycle);
  Serial.print(" , ");
  Serial.print("Low Period: ");
  Serial.print(lowPeriod);
  Serial.print(" , ");
  Serial.print("High Period: ");
  Serial.println(highPeriod);

  Serial.println("");

  Wire.begin(9); 
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);

  fire1 = LOW;
  fire2 = LOW;

}

void loop() 
{
  currentMillis = millis();
  inputState = digitalRead(inputPin1);

  //fire(100);

  /*if(inputState == HIGH)
  {
    fire(100);
  }
  else
  {
    digitalWrite(neuronPin1, HIGH);
  }*/

  //frequency = 80;

  if(angle < 90 && angle >-90 )
  {
    if(angle > 0)
    {
      frequency = angle/2;
      fire1 = HIGH;
      fire2 = LOW;
    }
    else if (angle < 0)
    {
      frequency = (angle * -1)/2;
      fire1 = LOW;
      fire2 = HIGH;
    }
    else
    {
      fire1 = LOW;
      fire2 = LOW;
    }

    if(frequency != 0)
    {
      cycle = 1000/frequency;
      lowPeriod  = 5*cycle/6;
      highPeriod = cycle/6;
    }
    else
    {
      //lowPeriod = 0;
      //highPeriod = 0;
      fire1 = LOW;
      fire2 = LOW;
    }
  }
  

if (fire1 == HIGH && fire2 == LOW)
{

  digitalWrite(neuronPin2,  HIGH);

  if(neuronState == LOW)
  {
    if (currentMillis - startMillis >= lowPeriod)
    {
      neuronState = HIGH;
      digitalWrite(neuronPin1, LOW);
      startMillis = currentMillis;
    }
  }
  else
  {
    if (currentMillis - startMillis >= highPeriod)
    {
      neuronState = LOW;
      digitalWrite(neuronPin1, HIGH);
      startMillis = currentMillis;
    }
  }

}
else if (fire2 == HIGH && fire1 == LOW)
{
  digitalWrite(neuronPin1,  HIGH);

  if(neuronState == LOW)
  {
    if (currentMillis - startMillis >= lowPeriod)
    {
      neuronState = HIGH;
      digitalWrite(neuronPin2, LOW);
      startMillis = currentMillis;
    }
  }
  else
  {
    if (currentMillis - startMillis >= highPeriod)
    {
      neuronState = LOW;
      digitalWrite(neuronPin2, HIGH);
      startMillis = currentMillis;
    }
  }
}
else
{
  digitalWrite(neuronPin1, HIGH);
  digitalWrite(neuronPin2, HIGH);
}
  
}



