/***********************************************************************************************************************************\
|*                                                       Securico - Arduino Project                                                *|
|*                                                             LeonidasEng                                                         *|
|*                                                                                                                                 *|
|*                                                                                                                                 *|
|*                                                       Automated Security Door System                                            *|
|*                                     Inputs: Keypad, Potentiometer, Motion Sensor, (Button for Doorbell)                         *|
|*                                               Outputs: Servo, Buzzer, LEDs, LCD Display                                         *|
|*                                                                                                                                 *|
|*                                                                                                                                 *|
\***********************************************************************************************************************************/

#include <Arduino.h>

#include <LiquidCrystal.h> // Library for LCD display, download from Arduino IDE, For Dev: see image in Teams chat

#include <Keypad.h> // Keypad library, download from Arduino IDE, For Dev: see image in Teams chat

#include <Servo.h> // Library for Servo Motor


/* Project can be viewed at:  https://wokwi.com/projects/378511339193348097/
 
/*Define LED pins */ 
#define redLED 10  
#define greenLED 11 

/* Motor, Buzzer Variables */
Servo myservo;
int servoPin = 12;
int servo_postion = 0;
int buzzerPin = 9;

/* PIR Sensor */
int pirInput = 13;
int pirState = LOW;
int motion_counter = 0;


/* Set Variables for keypad and LCD */ 
char password[] ="1234"; // Standard password 
int key_position = 0; // key_position on keypad 

const byte rows = 4; // Rows of Keypad 
const byte columns = 4; // Columns of Keypad

/* Button Variables */
const int buttonPin = 8; 
int buttonState = 0;

char key_map[rows][columns] = { // Define the buttons of the keypad 

{'1', '2', '3', 'A'}, 

{'4', '5', '6', 'B'}, 

{'7', '8', '9', 'C'}, 

{'*', '0', '#', 'D'} 

}; 
void motion(void);

byte rowPins [rows] = {22, 23, 24, 25}; // Key pad pins
byte colPins [columns] = {26, 27, 28, 29}; 

/* Define key map for library from the declared 2D array */
Keypad myKeypad = Keypad(makeKeymap(key_map), rowPins, colPins, rows, columns); 

/* Declare LCD analogue pins */
LiquidCrystal lcd (A0, A1, A2, A3, A4, A5); // LCD pins are: (RS, E, D4, D5, D6, D7) 

void setup() 
{
    /*-------------------        IO Setup        -------------------*/ 
    lcd.begin(16, 2); 

    pinMode(redLED, OUTPUT);  // Red LED as output for LOCKED
    pinMode(greenLED, OUTPUT); // Green LED as output for OPEN

    pinMode(pirInput, INPUT);   // Sets PIR sensor as input   
    pinMode(buzzerPin,OUTPUT);  // Sets Buzzer as an Output    

    pinMode(buttonPin, INPUT); // Sets Button as input
    
    Serial.begin(9600);   
    myservo.attach(servoPin);   // Servo pin attached to servo control
    setLocked(true); // state of the password to TRUE, logic (1)
    
    /*-------------------      Calibration of Sensors      -------------------*/
    Serial.println("Setup");
    Serial.print("Calibrating Sensors");
    for (int i = 0; i < 10; i++)
    { 
      delay(1000);
      Serial.print(".");
    }
    tone(buzzerPin, 1047, 250);
    delay(300);
    tone(buzzerPin, 1319, 250);
    delay(300);
    tone(buzzerPin, 1568, 150);
    delay(200);
    noTone(buzzerPin);
    Serial.println("Sensor ACTIVE");
} 

void loop() 
{ 
    /* Ensures button is held low on every loop */
    buttonState = LOW;
    buttonState = digitalRead(buttonPin);
    
    /* Doorbell (notes) tune: E5 C5 D5 G4 G4 D5 E5 C5*/
    if (buttonState == HIGH)
    {
    tone(buzzerPin, 466, 250); 
    delay(150);
    tone(buzzerPin, 466, 250);
    delay(150);
    tone(buzzerPin, 415, 250);
    delay(150);
    tone(buzzerPin, 415, 250);
    delay(150);
    tone(buzzerPin, 698, 750);
    delay(450);
    tone(buzzerPin, 698, 750);
    delay(450);
    tone(buzzerPin, 622, 1500);
    delay(900);
    tone(buzzerPin, 466, 250);
    delay(150);
    tone(buzzerPin, 466, 250);
    delay(150);
    tone(buzzerPin, 415, 250);
    delay(150);
    tone(buzzerPin, 415, 250);
    delay(150);
    tone(buzzerPin, 622, 750);
    delay(450);
    tone(buzzerPin, 622, 750);
    delay(450);
    tone(buzzerPin, 554, 750);
    delay(450);
    tone(buzzerPin, 523, 250);
    delay(150);
    tone(buzzerPin, 466, 500); 
    delay(300);
    tone(buzzerPin, 554, 250);
    delay(150);
    tone(buzzerPin, 554, 250);
    delay(150);
    tone(buzzerPin, 554, 250);
    delay(150);
    tone(buzzerPin, 554, 250);
    delay(150);
    tone(buzzerPin, 554, 750);
    delay(450);
    tone(buzzerPin, 622, 750);
    delay(450);
    tone(buzzerPin, 523, 750);
    delay(450);
    tone(buzzerPin, 466, 250); 
    delay(150);
    tone(buzzerPin, 415, 500); 
    delay(300);
    tone(buzzerPin, 415, 500); 
    delay(300);
    tone(buzzerPin, 415, 500); 
    delay(300);
    tone(buzzerPin, 622, 1000); 
    delay(600);
    tone(buzzerPin, 554, 2000); 
    delay(1200);
    noTone(buzzerPin);
    }

    char selectKey = myKeypad.getKey(); // set getKey to Keypad to find which key is pressed

    setLocked(true); 

    lcd.setCursor(0, 0); // Position on LCD Display

    lcd.print(" Enter Password"); // Prints string to Display

    lcd.setCursor(0, 1); // Moves to next line

    lcd.print(" \'#\' to Reset"); // Prints string to Display
    if(selectKey == '1' || selectKey == '2' || selectKey == '3' || selectKey == '4' ||
    selectKey == '5' || selectKey == '6' || selectKey == '7' || selectKey == '8' ||
    selectKey == '9')
    {
      setLocked(true);

      lcd.setCursor(0, 0); 
      tone(buzzerPin, 1200);
      delay(50);
      noTone(buzzerPin);
    }
    /* Invalid button - Buzzer plays negative sound */
    if(selectKey == '*' || selectKey == 'A' ||
    selectKey == 'B' || selectKey == 'C' || selectKey == 'D') 
    { 

        key_position=0; 

        setLocked(true); 

        lcd.clear(); 

        lcd.setCursor(0, 0); 

        lcd.print(" Invalid Key!"); 
        /* Negative sound from buzzer goes here */
        tone(buzzerPin, 250); /// Music note Bb2 (117) for 0.5 seconds
        delay(50);
        noTone(buzzerPin);
        delay(500); // Allows LCD time to state Invalid message
        lcd.clear(); 
    }
    /* Workaround for Password not resetting automatically */
    if(selectKey == '#') 
    {
      key_position=0; 

      setLocked(true); 

      lcd.clear(); 
      lcd.setCursor(0, 0); 
      
      tone(buzzerPin, 1800);
      delay(50);
      noTone(buzzerPin);
      lcd.print("Resetting...");
      delay(500);       
    }
    /* If Selected key is equal to the password at key position 0 increment up */
    if(selectKey == password[key_position]) 
    { 
        key_position++; 
    } 

    if(key_position == 4) 
    { 
        setLocked(false); 
        lcd.clear(); 
        lcd.setCursor(0, 0); 
        lcd.print("*** Verified ***"); 
        /* Once password is verified the user can enter through doorway */

        /*-------------------           Door OPENING           -------------------*/

        servo_postion = 180; // Door is opened - 180 degrees is equal to UNLOCKED
        myservo.write(servo_postion);
        tone(buzzerPin, 587, 250); // Music note D5 (587) then G5 (784) for ENTER
        noTone(buzzerPin); // Turns buzzer off
        tone(buzzerPin, 784, 250); // Music note D5 (587) then G5 (784) for ENTER
        noTone(buzzerPin);
        delay(3000); 

        lcd.clear(); 
        lcd.setCursor(0, 0); 
        lcd.print("    Securico "); 
        lcd.setCursor(0, 1); 
        lcd.print(" ER1630 Project"); 
        delay(1000); // Delay before starts to close again
        
        motion();


        /*-------------------           Door CLOSING           -------------------*/

        Serial.println("\n");
        delay(1000); 
        servo_postion = 0;
        myservo.write(servo_postion);
        
        noTone(buzzerPin);
        setLocked(true); // Door has has time to CLOSE - will now LOCK
        lcd.clear(); 
    } 
    delay(100);
}

void setLocked(int locked) 
{ 
        if(locked) 
        { 
            /* If locked = TRUE (1) then Red LED is ON and Green LED is OFF */
            digitalWrite(redLED, HIGH); 
            digitalWrite(greenLED, LOW); 
        } else 
        { 
            /* If unlocked = FALSE (0) then Red LED is OFF and Green LED is ON */
            digitalWrite(redLED, LOW); 
            digitalWrite(greenLED, HIGH); 
        } 
} 

void motion(void)
{
    /* Sensor MOTION is detected - DOOR WON'T CLOSE */
    motion_counter = 0; // Ensures motion sensor is reset to 0 - checks for obstructions in next loop
    do
    {
        pirState = digitalRead(pirInput);
        if (pirState == HIGH)
        {
            Serial.println("Motion Detected!");
            delay(1000);
            motion_counter = 0; // resets counter to 0 if motion is detected!
        } else if (pirState == LOW && motion_counter < 5)
        {
            Serial.println("No obstruction.");
            delay(1000);
            motion_counter++; // increases to 1 if motion is NOT detected
        } else
        {
            // Intentionally blank
        }
        Serial.println(motion_counter);
            
    } while (motion_counter < 5);
      Serial.print("Door closing");
      lcd.clear(); 
      lcd.setCursor(0, 0); 
      lcd.print("Door Closing!");
      /* Two tones played together for 0.5 seconds to simulate door closing sound */
      for (int i = 0; i < 10; i++)
      {
          tone(buzzerPin, 1568, 250); // Music Note G6
          tone(buzzerPin, 1760, 250); // Music Note A6  
          delay(500);
          Serial.print(".");
          pirState = digitalRead(pirInput);
        if (pirState == HIGH)
        {
        motion();
        break;
        }
       }
}

