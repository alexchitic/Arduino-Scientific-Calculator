/*
 * ============================================================
 * Project: Arduino-Based Programmable Scientific Calculator
 * Author: Chitic Alexandru-Daniel
 *
 * Description:
 * A programmable scientific calculator based on the
 * Arduino Uno (ATmega328P), featuring a 16x2 LCD display,
 * a 5x4 matrix keypad and mathematical expression evaluation.
 *
 * Project start: 05.09.2026
 * Last modified: 06.11.2026
 *
 * THIRD-PARTY LIBRARIES:
 *
 * 1. MathParser
 *    Author: Meer Zafarullah Noohani
 *    Purpose: Mathematical expression parsing and evaluation.
 *    GitHub: https://github.com/meerzafarnoohani/MathParser
 *
 * 2. LiquidCrystal
 *    Authors: Arduino, Adafruit
 *    Purpose: Controlling HD44780-compatible LCD displays.
 *    GitHub: https://github.com/arduino-libraries/LiquidCrystal
 *
 * 3. Keypad
 *    Authors: Mark Stanley, Alexander Brevig
 *    Purpose: Reading inputs from matrix keypads.
 *    GitHub: https://github.com/Chris--A/Keypad
 *
 * ============================================================
 */

#include <MathParser_Arduino.h>
#include <LiquidCrystal.h>
#include <Keypad.h>


//calculator variables
char typed_formula[65] = {};
int typed_formula_index = 0;
double result;


//LCD Pins
const int rs = 9, e = 10, d4 = 11, d5 = 12, d6 = 13, d7 = A0;
LiquidCrystal lcd(rs, e, d4, d5, d6, d7);

//matrix keypad size
const byte ROWS = 5;
const byte COLS = 4;

//tree position slideswitch pins
const byte SLIDE_POS1 = A1;
const byte SLIDE_POS2 = A2;
const byte SLIDE_POS3 = A3;

//keypad character table
char key; 
char hexaKeys[ROWS][COLS] = {
  {'7', '8', '9', '+'},
  {'4', '5', '6', '-'},
  {'1', '2', '3', '*'},
  {'0', '.', '=', '/'},
  {'A', 'B', 'C', 'D'}
};

//matrix keypad pins
byte rowPins[ROWS] = {4, 5, 6, 7, 8};
byte colPins[COLS] = {0, 1, 2, 3};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

//create the parser object
MathParser parser;  

void setup() {
  //pins configuration for the slide switch
  pinMode(SLIDE_POS1, INPUT_PULLUP);
  pinMode(SLIDE_POS2, INPUT_PULLUP);
  pinMode(SLIDE_POS3, INPUT_PULLUP);

  //initialise the display and turn on cursor
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.cursor();    
} 

void loop() {
  //get the character typed on the keypad
  key = customKeypad.getKey();

  //if a character is typed, see what it was and take the apropriate action
  if (key) {

    switch (key) {
      
      //if the user presses the D key, delete everything from memory and clear the screen
      case 'D':
         lcd.clear();
         lcd.setCursor(0, 0);
         typed_formula_index = 0;
         typed_formula[0] = '\0';      

      break;
      
      //if the users presses the equals sign, the calculator puts equals on the screen, terminates the array and calls the algorithm to process
      case '=':
        lcd.print(key);
        typed_formula[typed_formula_index] = '\0';

        //TODO: IMPLEMENT THE MATH ALGORITHM AND DISPLAY STUFF
        //formula parser and show the result
        parser.compile(typed_formula);
        result = parser.eval();
        lcd.setCursor(0, 1);
        lcd.print(result);
            
      break;
      //put the characters in the array and the screen
      default:
        typed_formula[typed_formula_index] = key;
        typed_formula_index++;
        lcd.print(key);
      break;
    }
  }

}