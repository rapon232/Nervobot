//#include <FreqCount.h>

float read = 0;

int prevRead = 0;
int currentRead = 0;

int inputPin = 5;

unsigned long startMillis;
unsigned long currentMillis;

float period = 0;
float frequency = 0;

float prevAnalogValue = 0;
float currentAnalogValue = 0;

bool stateChange = false;

void setup() {
  // enable serial output
  Serial.begin(9600);

  Serial.println("Arduino Frequency Counter");
  Serial.println();

  pinMode(inputPin, INPUT);

  startMillis = millis();

  //FreqCount.begin(1000);
}

void loop() {
  //read = analogRead(0);
  //read = digitalRead(5);

  //Serial.println(read);

  /*if (FreqCount.available()) 
  {
    float count = FreqCount.read();
    //Serial.println(count/100);
  }*/

  currentRead = digitalRead(inputPin);
  frequency = 1000/period;

  currentAnalogValue = analogRead(0);


  //Serial.print("Input:");
  Serial.print(currentAnalogValue);
  Serial.print("\t");
  Serial.print(currentRead);
  Serial.print("\t");
  //Serial.print("Period:");
  Serial.print(period);
  Serial.print("\t");
  //Serial.print("Frequency:");
  Serial.println(frequency);

  currentMillis = millis();

  //Serial.println(val);

  /*if (currentRead != prevRead)
  {
    if (currentRead == 1)
    {
      
        period = currentMillis - startMillis;
        startMillis = currentMillis;
       
      
    }
    else 
    {
      //stateChange = true;
    }
  }

  prevRead = currentRead;*/

  if (currentAnalogValue != prevAnalogValue)
  {
    if (currentAnalogValue > 500)
    {
      
      period = currentMillis - startMillis;
      startMillis = currentMillis;
       
      
    }
    else 
    {
      //stateChange = true;
    }
  }

  prevAnalogValue = currentAnalogValue;


  //unsigned long count = FreqCount.read();
  //Serial.println(count);

  //delay(100);
}