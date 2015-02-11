#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

const char texts[2][17] = {{"Eelis  -  Aatos"}, {"Aatos  -  Eelis"}};

const char TEAMS = 5;
const char teams[TEAMS][8] = {
  "Classic",
  " Ilves ",
  " KooVee",
  " Eelis ",
  " Aatos "
};

int textIndex = 0;

enum availableButtons {
    HOME_ADD,
    HOME_DEC,
    GUEST_ADD,
    GUEST_DEC,
    NUM_BUTTONS
};

enum display_mode {
  SETUP,
  RUN_GAME,
  STOP_GAME
};

display_mode mode = SETUP;

const int hAddPin = 8;
const int hDecPin = 7;

const int gAddPin = 10;
const int gDecPin = 9;

struct Button {
  int pin;
  int old_state;
  unsigned long time_pressed;
};

Button buttons[4] = {
  {hAddPin, 0, 0},
  {hDecPin, 0, 0},
  {gAddPin, 0, 0},
  {gDecPin, 0, 0},
};

enum buttonStates {
  NO_PRESS,
  SHORT_PRESS,
  LONG_PRESS,
  PIN = 0,
  OLD_STATE,
  TIME
};

const int LIMIT = 999;

struct Game {
  char home_team_id;
  char guest_team_id;
  int hCount;
  int gCount;
};

Game game = {0, 0};

buttonStates buttonState(int pin);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  
  pinMode(hAddPin, INPUT);
  pinMode(hDecPin, INPUT);
  pinMode(gAddPin, INPUT);
  pinMode(gDecPin, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  
  switch(mode) {
    case SETUP:
      runSetupMode();
      break;
    case RUN_GAME:
      runGame();
      break;
    case STOP_GAME:
      runStopGame();
      break;
    default:
      runError();
  };
}

void runGame() {    
  for (char i = 0; i < NUM_BUTTONS; i++) {
    buttonStates state = buttonState(i);
    if (i == HOME_ADD) {
      if (state == SHORT_PRESS) {
        game.hCount += 1;
        Serial.println("home add");
      } else if (state == LONG_PRESS) {
        mode = SETUP;
        Serial.println("mode change");
        lcd.clear();
        return;
      }
    }
    if (i == HOME_DEC) {
      if (state == SHORT_PRESS) {
        game.hCount -= 1;
               Serial.println("home dec");
      }
    }
    if (i == GUEST_ADD) {
      if (state == SHORT_PRESS) {
        game.gCount += 1;
               Serial.println("guest add");
      }
    }
    if (i == GUEST_DEC) {
      if (state == SHORT_PRESS) {
        game.gCount -= 1;
               Serial.println("guest dec");
      }
    }
  }
  limitGoals(game.hCount);
  limitGoals(game.gCount);

  printTeams();
  printGoal(game.hCount, 0, 1);
  printGoal(game.gCount, 10, 1);
}

buttonStates buttonState(int pin)
{
  int state = digitalRead(buttons[pin].pin);
//  Serial.print("pin: ");
//  Serial.println(pin);
//  Serial.print("state: ");
//  Serial.println(state);
  
  enum buttonStates button = NO_PRESS;
  
  if (state == 1 && buttons[pin].old_state == 1) {
    if ((millis() - buttons[pin].time_pressed) >= 1000) {
      button = LONG_PRESS;
      buttons[pin].time_pressed = millis();
    }
  } else if (state == 0 && buttons[pin].old_state == 1) {
    button = SHORT_PRESS;
    buttons[pin].time_pressed = millis();
  } else {
    buttons[pin].time_pressed = millis();
  }
  buttons[pin].old_state = state;
  
  return button;
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

void printTeams()
{
  lcd.setCursor(0,0);
  lcd.print(teams[game.home_team_id]);
  lcd.setCursor(8,0);
  lcd.print("-");
  lcd.print(teams[game.guest_team_id]);
}

void runSetupMode()
{
  buttonStates state = buttonState(HOME_ADD);
  if (state == SHORT_PRESS) {
    ;
  } else if (state == LONG_PRESS) {
    mode = RUN_GAME;
    lcd.clear();
    return;
  }
  
  state = buttonState(HOME_DEC);
  if (state == SHORT_PRESS) {
    game.home_team_id += 1;
    if (game.home_team_id >= TEAMS)
      game.home_team_id = 0;
  }
  state = buttonState(GUEST_DEC);
  if (state == SHORT_PRESS) {
    game.guest_team_id += 1;
    if (game.guest_team_id >= TEAMS)
      game.guest_team_id = 0;
  }

    printTeams();  
    lcd.setCursor(0, 1);
    lcd.print("setup");
}

void runStopGame()
{
}
void runError()
{
}

