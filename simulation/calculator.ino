
#include <LiquidCrystal.h>
#include <Keypad.h>

//LCD Pins
const int rs = 9, e = 10, d4 = 11, d5 = 12, d6 = 13, d7 = A0;
LiquidCrystal lcd(rs, e, d4, d5, d6, d7);

//buttons
const byte ROWS = 5;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'7', '8', '9', '+'},
  {'4', '5', '6', '-'},
  {'1', '2', '3', '*'},
  {'0', '.', '=', '/'},
  {'A', 'B', 'C', 'D'}
 
};

byte rowPins[ROWS] = {4, 5, 6, 7, 8};
byte colPins[COLS] = {0, 1, 2, 3};
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup() {
  
  //initialise the display
  lcd.begin(16, 2);

  lcd.print("hello");

} 

void loop() {
  char key = customKeypad.getKey();

  if(key){
    lcd.print(key);
  }

}