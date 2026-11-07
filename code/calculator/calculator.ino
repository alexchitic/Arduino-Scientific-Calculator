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
 * Last modified: 07.11.2026
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
#include <math.h>

//calculator special stuff
enum DisplayMode {
  NORM,
  SCI,
  ENG
};

//calculator variables
char typed_formula[65] = {};
int typed_formula_index = 0;
double result;
int precision = 2;
DisplayMode displaySetAs = NORM;

//keypad variables
char precisionKey;
bool shiftActive = false;

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

//object declarations
MathParser parser;  

//function to clear the display and reset the typed formula
void clearAll(){
  lcd.clear();
  lcd.setCursor(0, 0);
  typed_formula_index = 0;
  typed_formula[0] = '\0';
}

//function to put the typed character in the array and on the screen
void putCharInArrayAndOnScreen(char key){
  //put the character in the array
  typed_formula[typed_formula_index] = key;
  typed_formula_index++;

  //put the character on the screen
  lcd.print(key);
}

//function to terminate the typed formula and calculate the result
void terminateAndCalculate(){
  //terminate the typed formula
  typed_formula[typed_formula_index] = '\0';

  //formula parser and show the result
  parser.compile(typed_formula);
  result = parser.eval();
}

//function to change the precision of the result
void changePrecision(){
  //set text
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Precision: (1-9)");

  //wait for the user to type a number between 1 and 9
  while(true){
    precisionKey = customKeypad.getKey();
    if(precisionKey >= '1' && precisionKey <= '9'){
      precision = precisionKey - '0';
      break;
    }
  }
    lcd.setCursor(0, 1);
    lcd.print(precision);

    delay(500);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(typed_formula); 
}

//function to change the display mode
void changeDisplayMode(){
  //switch between the display modes and set the displaySetAs variable accordingly
  switch(displaySetAs){
    case NORM:
      displaySetAs = SCI;
    break;
    case SCI:
      displaySetAs = ENG;
    break;
    case ENG:
      displaySetAs = NORM;
    break;
  }

  //reveal the change on the display for 1000 milliseconds 
  unsigned long time  = millis();
  while(millis() - time < 1000){
  lcd.setCursor(0, 0);
  lcd.print("Display mode:");
  lcd.setCursor(0, 1);
  lcd.print(displaySetAs == NORM ? "Normal" : displaySetAs == SCI ? "Scientific" : "Engineering");
}
  //clear the display and show the typed formula again
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(typed_formula);
}


//function to display the result
void displayResult(double result, DisplayMode displayMode, int precision){
  
  lcd.setCursor(0, 1);
  switch(displayMode){
    //display the result in normal mode
    case NORM:
      lcd.print(result, precision);
    break;
    //display the result in scientific mode
    case SCI:
      int exponent = floor(log10(abs(result)));
      double mantissa = result / pow(10, exponent);
      lcd.print(mantissa, precision);
      lcd.print("e");
      lcd.print(exponent); 
    break;

    //display the result in engineering mode
    case ENG:
      int engExponent = floor(log10(abs(result)) / 3) * 3;
      double engMantissa = result / pow(10, engExponent);
      lcd.print(engMantissa, precision);
      lcd.print("e");
      lcd.print(engExponent);
    break;
  }

}

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

  //get the character typed on the keypad and the state of the key (pressed, held or released)
  key = customKeypad.getKey();
  //if a character is typed, see what it was and take the apropriate action
  if (key) {

    switch (key) {
      
      //if the user presses the D key, delete everything from memory and clear the screen
      case 'D':
      clearAll();
      shiftActive = false;
      break;

      //if the user presses the A key, change the display mode or precision depending on how long the key is held 
      case 'A':
          if (shiftActive) {
            changePrecision();
            shiftActive = false;
          }
          else {
            changeDisplayMode();
          }
        break;

      //use B as the shift key, to access the secondary functions
      case 'B':
        shiftActive = !shiftActive;
        //check if the shift key is active and put an indicator on the display if it is
        if (shiftActive) {
        lcd.setCursor(15, 1);
        lcd.print("S");
        } 
      break;
      
      //if the users presses the equals sign, the calculator puts equals on the screen, terminates the array and calls the algorithm to process
      case '=':
        terminateAndCalculate();
        displayResult(result, displaySetAs, precision);
        shiftActive = false;
      break;

      //put the characters in the array and the screen
      default:
        putCharInArrayAndOnScreen(key);
        shiftActive = false;
      break;
    }
  }
}