

#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

//bool loopRunning = 0;
//bool startButtonState = 0;
//bool prevStartButtonState = 0;
//bool stopButtonState = 0;
//bool prevStopButtonState = 0;

int preLedPin = 10;
int postLedPin = 11;
//int startButtonPin = 9;
//int stopButtonPin = 8;

int interDelay = 500;
int postDelay = 100;
int silenceDelay =1000;

int iterations = 0;
int setIterations = 10;

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long waitPeriod = 2000;  
bool instrMessage = 0;

int totalColumns = 16;
int totalRows = 2;

const byte numRows= 4;
const byte numCols= 4;

char keymap[numRows][numCols] = 
{
{'1', '2', '3', 'A'}, 
{'4', '5', '6', 'B'}, 
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};

byte rowPins[numRows] = {7,6,5,4};
byte colPins[numCols] = {3,2,1,0};

void intro();
void printMessage();
void printDelayedMessage();
int setParam();
void experiment();

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

LiquidCrystal_I2C lcd(0x3E, totalColumns, totalRows);

void setup()
{
  pinMode(preLedPin, OUTPUT);
  pinMode(postLedPin, OUTPUT);
  //pinMode(stopButtonPin, INPUT_PULLUP);
  //pinMode(startButtonPin, INPUT_PULLUP);
  
  lcd.begin(totalColumns, totalRows);
  lcd.init(); 
  lcd.backlight();
  
  intro(interDelay, postDelay, silenceDelay, setIterations, "SETUP COMPLETE");
  
  startMillis = millis();
}

void loop ()
{
  currentMillis = millis();
  
   if (currentMillis - startMillis >= waitPeriod)
   {
     if(instrMessage == 0)
     {
		  lcd.clear();
      lcd.setCursor(0, 0); 
  	 	lcd.print("PRESS C FOR INFO");
  	 	lcd.setCursor(0,1);
  	 	lcd.print("PRESS D TO START");
        startMillis = currentMillis;
        instrMessage = 1;
     }
     else
     {
        lcd.clear();
		instrMessage = 0;
        startMillis = currentMillis;
     }
   }
  
   if(instrMessage == 0)
   {
     //lcd.clear();
     lcd.setCursor(0, 0); 
     lcd.print("PRESS A TO SET");
     lcd.setCursor(0,1);
     lcd.print("PRESS B TO ENTER");
   }
  
  /*lcd.setCursor(0, 0); 
  lcd.print("PRESS A TO SET");
  lcd.setCursor(0,1);
  lcd.print("PRESS D TO START");*/
  
  /*if(loopRunning) 
  {  
    experiment(interDelay, postDelay, silenceDelay, setIterations);
  }

  startButtonState = digitalRead(startButtonPin);
    
  if((startButtonState == LOW) && (prevStartButtonState == HIGH)) 
  {  
    iterations = 0;
    loopRunning = !loopRunning;
  }
  
  prevStartButtonState = startButtonState;*/
  
  char keypressed = myKeypad.getKey();
  
  if (keypressed != NO_KEY)
  {
    if (keypressed == 'A') 
    {
      interDelay = setParam(interDelay, "SET INTER DELAY", "ms");
      printDelayedMessage(interDelay, 1000, "INTER DELAY =", "ms");
      
      postDelay = setParam(postDelay, "SET HOLD DELAY", "ms");
      printDelayedMessage(postDelay, 1000, "HOLD DELAY =", "ms");
      
      silenceDelay = setParam(silenceDelay, "SET PAUSE DELAY", "ms");
      printDelayedMessage(silenceDelay, 1000, "PAUSE DELAY =", "ms");
      
      setIterations = setParam(setIterations, "SET ITER COUNT", "");
      printDelayedMessage(setIterations, 1000, "ITER COUNT =", "");
      
      lcd.clear();
      lcd.setCursor(0, 0); 
      lcd.print("SETTINGS APPLIED");
      lcd.setCursor(0,1);
      delay(1000);
      
    }
    if (keypressed == 'C') 
    {
      intro(interDelay, postDelay, silenceDelay, setIterations, "SETTINGS SAVED");
    }
    if (keypressed == 'D') 
    {
      experiment(interDelay, postDelay, silenceDelay, setIterations);
    }
  }
  
  }

void intro(int delay1, int delay2, int delay3, int iter, String endMessage)
{
  lcd.clear();
  
  lcd.setCursor(0, 0); 
  lcd.print("INTER DELAY =");
  lcd.setCursor(0, 1); 
  lcd.print(delay1);
  lcd.print("ms");
  delay(1000);
  lcd.clear();
  
  lcd.setCursor(0, 0); 
  lcd.print("HOLD DELAY =");
  lcd.setCursor(0, 1); 
  lcd.print(delay2);
  lcd.print("ms");
  delay(1000);
  lcd.clear();
  
  lcd.setCursor(0, 0); 
  lcd.print("PAUSE DELAY =");
  lcd.setCursor(0, 1); 
  lcd.print(delay3);
  lcd.print("ms");
  delay(1000);
  lcd.clear();
  
  lcd.setCursor(0, 0); 
  lcd.print("ITERATION COUNT =");
  lcd.setCursor(0, 1); 
  lcd.print(iter);
  delay(1000);
  lcd.clear();

  lcd.setCursor(0, 0); 
  lcd.print(endMessage);
  delay(500);
  lcd.clear();
}

void printMessage(int value, String message, String dimension)
{
  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print(message);
  lcd.setCursor(0, 1); 
  lcd.print(value);
  lcd.print(dimension);
  lcd.print(" -> ");
}

void printDelayedMessage(int value, int delay1, String message, String dimension)
{
  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print(message);
  lcd.setCursor(0, 1); 
  lcd.print(value);
  lcd.print(dimension);
  delay(delay1);
  lcd.clear();
}

int setParam(int inter, String message, String dimension)
{
  int value = 0; 
  String inputString = "";
  
  char keypressed = myKeypad.getKey();
  
  printMessage(inter, message, dimension);

  do  
  {
    keypressed = myKeypad.getKey();
    
    if (keypressed != NO_KEY)
  	{ 
        if (keypressed >= '0' && keypressed <= '9') 
        {
          lcd.print(keypressed);
          inputString += String(keypressed);
          delay(1000);
        } 
      	else if (keypressed == '*') 
        {
          inputString = "";
          printMessage(inter, message, dimension);
      	}
    }

  }
  while (keypressed != 'B');
  
  if(inputString == "")
  {
    value = inter;
  }
  else
  {
    value = inputString.toInt();
  }
  
  lcd.clear();
  return value;
  
}

void experiment(int delay1, int delay2, int delay3, int iter)
{
  iterations = 0;

  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("SETUP RUNNING...");
  lcd.setCursor(0,1);
  
  for( int i = 0; i < iter; i++)
  {
    digitalWrite(preLedPin, HIGH);
    delay(delay1);
    digitalWrite(postLedPin, HIGH);
    delay(delay2);
    digitalWrite(preLedPin, LOW);
    digitalWrite(postLedPin, LOW);
    delay(delay3);
    iterations++;
    lcd.setCursor(0,1);
    lcd.print(iterations);

    char keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY)
  	{ 
      if (keypressed == '#') 
      {
        break;
      }
    }
  }
  
  //loopRunning = !loopRunning;
  
  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("SETUP FINISHED...");
  delay(1000);
}
