int inputPin1 = 12;
int neuronPin1 = 13;

unsigned long startMillis;
unsigned long currentMillis;

float highPeriod;  
float lowPeriod;

bool inputState = LOW;
bool neuronState = LOW;

void fire(float frequency)
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


void setup() 
{
  pinMode(neuronPin1, OUTPUT);
  pinMode(inputPin1, INPUT);
  startMillis = millis();

  Serial.begin(115200);
}

void loop() 
{
  currentMillis = millis();
  inputState = digitalRead(inputPin1);

  if(inputState == HIGH)
  {
    fire(100);
  }
  else
  {
    digitalWrite(neuronPin1, HIGH);
  }
  
}



