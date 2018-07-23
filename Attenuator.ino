
//Libraries to run LCD
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>


#define WHITE 0x7
#define A_Mode_Loc 4
#define B_Mode_Loc 12

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

int ChAAutoFeedback = 0; //Analog Feedback
int ChBAutoFeedback = 1;

int ChAManFeedback = 2; //Analog
int ChBManFeedback = 3; //Analog

int ChAManual = 5; //Digital Auto/Manual Pins
int ChBManual = 4; //Digital Auto/Manual Pin

int LEPinA = 2; //Latch Enable Pins
int LEPinB = 3;

int ChAFeedbackChoice = 0;
int ChBFeedbackChoice = 0;

double feedbackValA = 0; //Stores analog value
double feedbackValB = 0;

int writeValA = 0; //Used to convert double feedbackVal to an integer
int writeValB = 0;
int writeValAOld = 0; //Used to store old values to check whether to update the attenuator
int writeValBOld = 0;

unsigned long initTime = 0;

void setup() {

  //Serial.begin(9600);

  //Set Up Pins
  pinMode(ChAAutoFeedback, INPUT);
  pinMode(ChBAutoFeedback, INPUT);
  pinMode(ChAManFeedback, INPUT);
  pinMode(ChBManFeedback, INPUT);

  pinMode(LEPinA, OUTPUT);
  pinMode(LEPinB, OUTPUT);

  pinMode(13, OUTPUT); //Digital pins for attenuator
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);

  pinMode(ChAManual, INPUT);
  pinMode(ChBManual, INPUT);

  // set pins 8..13 as output...
  //DDRB = B00111111;  // digital pins -,-,13,12,11,10,9,8

  digitalWrite(LEPinA, LOW); //Pulls Latch Enable LOW to disable changing values
  digitalWrite(LEPinB, LOW);

  //Write to LCD
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.print("A: Auto");
  lcd.setCursor(9, 0);
  lcd.print("B: Auto");

  lcd.setBacklight(WHITE);

  //Select which feedback to use initially
  ChAFeedbackChoice = ChAAutoFeedback;
  ChBFeedbackChoice = ChBAutoFeedback;


  //Test code for debugging

}

void loop() {

  // Disable writing
  //Get the analog value for each channel
  feedbackValA = analogRead(ChAFeedbackChoice);
  feedbackValB = analogRead(ChBFeedbackChoice);

  //Maps the analog value to a value between 0 and 63,
  feedbackValA = round((feedbackValA / 1023) * 31.5 * 2);
  feedbackValB = round((feedbackValB / 1023) * 31.5 * 2);

  //Cast each double value to an integer
  writeValA = feedbackValA;
  writeValB = feedbackValB;

  //Write to attenuator A
  if (writeValA != writeValAOld)
  {
    //PORTB = writeValA;   // Writes number 1-30.5 to pins 13-9 simultaneously on CHA
    digitalWrite(13, HIGH && (writeValA & B100000));
    digitalWrite(12, HIGH && (writeValA & B010000));
    digitalWrite(11, HIGH && (writeValA & B001000));
    digitalWrite(10, HIGH && (writeValA & B000100));
    digitalWrite(9, HIGH && (writeValA & B000010));
    digitalWrite(8, HIGH && (writeValA & B000001));

    digitalWrite(LEPinA, HIGH); //Pulls Latch Enable HIGH to enable changing values
    delay(5);
    digitalWrite(LEPinA, LOW);
    writeValAOld = writeValA;
  }


  //Write to attenuator B
  //PORTB = writeValB;   // Writes number 1-30.5 to pins 13-8 simultaneously on CHA
  if (writeValB != writeValBOld)
  {
    digitalWrite(13, HIGH && (writeValB & B100000));
    digitalWrite(12, HIGH && (writeValB & B010000));
    digitalWrite(11, HIGH && (writeValB & B001000));
    digitalWrite(10, HIGH && (writeValB & B000100));
    digitalWrite(9, HIGH && (writeValB & B000010));
    digitalWrite(8, HIGH && (writeValB & B000001));

    digitalWrite(LEPinB, HIGH); //Pulls Latch Enable HIGH to enable changing values
    delay(5);
    digitalWrite(LEPinB, LOW);
    writeValBOld = writeValB;
  }

  delay(5);

  //Write Values to LCD
  if (( millis() > initTime + 250))
  {
    //CH A
    lcd.setCursor(0, 1);
    lcd.print(feedbackValA / 2);
    lcd.setCursor(4, 1);
    lcd.print(" dB");

    if (digitalRead(ChAManual) == HIGH)
    {
      lcd.setCursor(0, 0);
      lcd.print("A: Auto");

      ChAFeedbackChoice = ChAAutoFeedback;

    }
    if (digitalRead(ChAManual) == LOW)
    {
      lcd.setCursor(0, 0);
      lcd.print("A: Man ");

      ChAFeedbackChoice = ChAManFeedback;
    }



    //CH B
    lcd.setCursor(9, 1);
    lcd.print(feedbackValB / 2);
    lcd.setCursor(13, 1);
    lcd.print(" dB");

    if (digitalRead(ChBManual) == HIGH)
    {
      lcd.setCursor(9, 0);
      lcd.print("B: Auto");

      ChBFeedbackChoice = ChBAutoFeedback;

    }
    if (digitalRead(ChBManual) == LOW)
    {
      lcd.setCursor(9, 0);
      lcd.print("B: Man ");

      ChBFeedbackChoice = ChBManFeedback;
    }

    initTime = millis();
  }


  //Serial.print(time2);
  //Serial.print("<>");


}
