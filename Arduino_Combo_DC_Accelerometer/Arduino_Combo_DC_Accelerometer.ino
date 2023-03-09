const int unitsPerG = 163; // depends on your ADC and the range (1.5g vs 6g)
int axisPin[] = {A0, A1, A2};
int selfTestPin = 8;
int gSelectPin = 9;
int sleepPin = 10;
int startVal[3] = {0};
char result[8]; // for format of the output

const byte encoder0pinA = 2;//A pin -> the interrupt pin 0
const byte encoder0pinB = 3;//B pin -> the digital pin 3
byte encoder0PinALast;
int duration;//the number of the pulses
boolean Direction;//the rotation direction

int motorPin1 = 5;
int motorPin2 = 6;

int neuronPin1 = 7;

unsigned long startMillis, startMillis1;
unsigned long currentMillis, currentMillis1;

int notificationDuration;

float frequency, cycle, lowPeriod, highPeriod;  

bool inputState = LOW;
bool neuronState = LOW;

void setup()
{
  Serial.begin(9600);
  pinMode(selfTestPin, OUTPUT);
  pinMode(gSelectPin, OUTPUT);
  pinMode(sleepPin, OUTPUT);

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  EncoderInit();//Initialize the module

  pinMode(neuronPin1, OUTPUT);
  startMillis = millis();
  startMillis1 = millis();


  digitalWrite(sleepPin, HIGH);
  digitalWrite(selfTestPin, LOW);
  digitalWrite(gSelectPin, LOW);
  getStartVals(); 

  frequency = 80;
  cycle = 1000/frequency;
  lowPeriod  = 5*cycle/6;
  highPeriod = cycle/6;

  notificationDuration = 10000;
}

void loop()
{
  currentMillis = millis();
  currentMillis1 = millis();

  int angles[3] = {0};
  int raw[3] = {0};
  float gVal[3] = {0.0};

  getRawVals(raw);
  getAngles(angles,raw);
  getGValues(gVal, raw); 

  if (currentMillis1 - startMillis1 >= notificationDuration)
  {
    
    Serial.println("");

    Serial.print("x-Raw:  "); sprintf(result,"%6d", raw[0]); // sprintf for format
    Serial.print(result);
    Serial.print("     ");
    Serial.print("y-Raw:  "); sprintf(result,"%6d", raw[1]); 
    Serial.print(result);
    Serial.print("     ");
    Serial.print("z-Raw:  "); sprintf(result,"%6d", raw[2]); 
    Serial.println(result);
    
    Serial.print("x-Angle:  "); sprintf(result,"%4d", angles[0]);
    Serial.print(result);
    Serial.print("     ");
    Serial.print("y-Angle:  "); sprintf(result,"%4d", angles[1]);
    Serial.print(result);
    Serial.print("     ");
    Serial.print("z-Angle:  "); sprintf(result,"%4d", angles[2]);
    Serial.println(result);

    Serial.print("x-g:      "); 
    Serial.print(gVal[0]); 
    Serial.print("     ");
    Serial.print("y-g:      "); 
    Serial.print(gVal[1]);
    Serial.print("     ");
    Serial.print("z-g:      ");
    Serial.println(gVal[2]);
    //Serial.println();

    Serial.print("Motor Encoder Pulses: ");
    Serial.println(duration);
    duration = 0;
    
    //delay(500);

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

    startMillis1 = currentMillis1;

  }

  /*if(neuronState == LOW)
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
  }*/

  //fire(60);
}

void getGValues(float gValueArray[], int rawArray[])
{
  for(int i=0; i<3; i++){
    int diff = rawArray[i] - startVal[i];
    gValueArray[i] = diff*1.0/unitsPerG;
  }
}

void getRawVals(int rawVals[])
{
  for(int i=0; i<3; i++){
    rawVals[i] = readMMA7361(axisPin[i]);
  }
}

void getAngles(int angleArray[], int rawArray[])
{
  bool positive = true;
  for(int i=0; i<3; i++){
    int diff = rawArray[i] - startVal[i];
    positive = (diff>=0);
    diff = abs(diff);
    if(diff > unitsPerG){
      diff = unitsPerG;
    }
    angleArray[i] = int((asin(diff*1.0/unitsPerG))*57.296);
    if(positive==false){
      angleArray[i] = -angleArray[i];
    }
  } 
}

int readMMA7361(int pin)
{
  long sum = 0;
  int result = 0;
  for(int i=0; i<50; i++){  // mean value of 50 measurements
    sum += analogRead(pin);
  }
  result = sum / 50;
  return result;
}

void getStartVals()
{
  Serial.println("");
  Serial.println("");
  Serial.println("Your MMA71361 should now be placed flat on the ground, i.e.:");
  Serial.println("x/y-axis = 0 degrees, z-axis = 90 degrees ");
  Serial.print("Wait....");
  delay(2000); // to have enough time to position the MMA7361
  for(int i=0; i<3; i++){
    startVal[i] = readMMA7361(axisPin[i]);
  }
  startVal[2] -= unitsPerG; // Z-axis is at 90° in start position! 
  Serial.println("ready!");
  Serial.println("");
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

void spikeForward()
{

}