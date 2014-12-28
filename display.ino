#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int hAddPin = 8;
const int hDecPin = 7;

const int gAddPin = 10;
const int gDecPin = 9;

const int LIMIT = 999;

int hAddState = 0;
int hAddOldState = 0;
int hDecState = 0;
int hDecOldState = 0;

int gAddState = 0;
int gAddOldState = 0;
int gDecState = 0;
int gDecOldState = 0;

int hCount = 0;
int gCount = 0;

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  
  pinMode(hAddPin, INPUT);
  pinMode(hDecPin, INPUT);
  pinMode(gAddPin, INPUT);
  pinMode(gDecPin, INPUT);
}

void loop() {
  
  hCount += wasButtonPressed(hAddPin, hAddOldState, hAddState) ? 1 : 0;
  hCount -= wasButtonPressed(hDecPin, hDecOldState, hDecState) ? 1 : 0;

  gCount += wasButtonPressed(gAddPin, gAddOldState, gAddState) ? 1 : 0;
  gCount -= wasButtonPressed(gDecPin, gDecOldState, gDecState) ? 1 : 0;

  limitGoals(hCount);
  limitGoals(gCount);
    
  //lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Eelis  -  Aatos");
  printGoal(hCount, 0, 1);
  printGoal(gCount, 10, 1);
}

void limitGoals(int &goals)
{
    if (goals < 0)
    goals = 0;
  else if (goals >= LIMIT)
    goals = LIMIT;
}

void printGoal(int goals, int x, int y)
{
  lcd.setCursor(x, y);
  if (goals < 10)
    lcd.print("  ");
  else if (goals < 100)
    lcd.print(" ");

  lcd.print(goals);
}

boolean wasButtonPressed(int pin, int &old, int &state) 
{
  state = digitalRead(pin);
  
  if (old == 1 && state == 0)
  {
    old = state;
    return true;
  }
  
  old = state;
  
  return false;
}

