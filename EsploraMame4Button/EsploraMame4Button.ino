
/* This sketch turns the Esplora into a Mame game controller with buttons added to
 the tinkerkit connections for adding Escape, Enter, start button [1] and add credits [5]
 And then adding the joystick button press for [Enter]
 You can find the Fritzing file on GitHub to see how I wired up the buttons:
 https://github.com/circuit69/EsploraTinkerkit
 
 Modified by Joel Krueger 12-8-2013.
 
 This code is a combination of the EsploraKart and
 Esplora Tinkerkit Input Read by Mike Barela created on 2013-03-24
 http://21stdigitalhome.blogspot.com/2013/03/reading-arduino-esplora-tinkerkit-inputs.html
 
 This example is in the public domain, please attribute
 */
#include <Esplora.h>
const int Tinkerkit1 = 11;        //Left orange Tinkerkit output pin 11 used as an input
const int Tinkerkit2 = 3;         //Right orange Tinkerkit output pin 3 used as an input
unsigned int button1 = 1;         //Left orange Tinkerkit
unsigned int lastButton1 = 1;
unsigned int button2 = 1;         //Right orange Tinkerkit
unsigned int lastButton2 = 1;
unsigned int buttonA = 1023;      //Left white Tinkerkit input
unsigned int lastButtonA = 1023;
unsigned int buttonB = 1023;      //Right white Tinkerkit input
unsigned int lastButtonB = 1023;
boolean buttonJoy;                //Joystick button input
boolean lastButtonJoy;
boolean buttonStates[8];          //Button states of original EsploraKart

/*
  This array holds the names of the buttons being read.
 Later in the sketch, you'll use these names with
 the method Esplora.readButton(x), where x
 is one of these buttons.
 Tinkerkit conections and joystick cannot use Esplora.readButton(x).
 */
const byte buttons[] = {
  JOYSTICK_DOWN,
  JOYSTICK_LEFT,
  JOYSTICK_UP,
  JOYSTICK_RIGHT,
  SWITCH_RIGHT, // left ctrl key
  SWITCH_LEFT, // left alt key
  SWITCH_UP, // spacebar
  SWITCH_DOWN, // left shift key
};

/*
  This array tells what keystroke to send to the PC when a
 button is pressed.
 If you look at this array and the above one, you can see that
 the "down arrow" keystroke is sent when the joystick is moved
 down, the "up arrow" keystroke when the joystick is moved up
 and so on.
 We can assign keystrokes here.
 */
const char keystrokes[] = {
  KEY_DOWN_ARROW,
  KEY_LEFT_ARROW,
  KEY_UP_ARROW,
  KEY_RIGHT_ARROW,
  KEY_LEFT_CTRL,
  KEY_LEFT_ALT,
  ' ',
  KEY_LEFT_SHIFT,
  KEY_ESC,            //keystroke for left orange tinkerkit button press
  KEY_RETURN,         //keystroke for right orange tinkerkit button press
  '1',                //keystroke for left white tinkerkit input button press
  '5',                //keystroke for right white tinkerkit input button press
  KEY_RETURN,         //keystroke for joystick button press
};

/* This sets up the functions to read the tinkerkit inputs.
 By Mike Barela
 */
const byte CH_TINKERKIT_INA = 8;   // Add values missing from Esplora.h
const byte CH_TINKERKIT_INB = 9;
const byte INPUT_A          = 0;
const byte INPUT_B          = 1;

unsigned int readTinkerkitInput(byte whichInput) {      // return 0-1023 from Tinkerkit Input A or B
  return readChannel(whichInput+CH_TINKERKIT_INA); 
}   //   as defined above

unsigned int readChannel(byte channel) {                // as Esplora.readChannel is a private function
  digitalWrite(A0, (channel & 1) ? HIGH : LOW);      //  we declare our own as a hack
  digitalWrite(A1, (channel & 2) ? HIGH : LOW);      //
  digitalWrite(A2, (channel & 4) ? HIGH : LOW);      // digitalWrite sets address lines for the input
  digitalWrite(A3, (channel & 8) ? HIGH : LOW);
  return analogRead(A4);               // analogRead gets value from MUX chip
}

void setup() {
  Keyboard.begin();
  //Initialize orange Tinkerkit outputs as inputs
  pinMode(Tinkerkit1,INPUT);
  pinMode(Tinkerkit2,INPUT);
}

void loop() { 
  // Iterate through all the buttons:
  for (byte thisButton=0; thisButton<8; thisButton++) {
    boolean lastState = buttonStates[thisButton];
    boolean newState = Esplora.readButton(buttons[thisButton]);
    if (lastState != newState) { // Something changed!
      if (newState == PRESSED) {
        Keyboard.press(keystrokes[thisButton]);
      }
      else if (newState == RELEASED) {
        Keyboard.release(keystrokes[thisButton]);
      }
    }

    // Store the new button state, so you can sense a difference later:
    buttonStates[thisButton] = newState;
  }

  // read the orange Tinkerkit input on the left:
  button1 = digitalRead(Tinkerkit1);
  if (lastButton1 != button1) {
    if (button1 == 0) {
      Keyboard.press(keystrokes[8]);
    }
    else if (button1 == 1) {
      Keyboard.release(keystrokes[8]);
    }
  }
  lastButton1 = button1;

  // read the orange Tinkerkit input on the right:
  button2 = digitalRead(Tinkerkit2);
  if (lastButton2 != button2) {
    if (button2 == 0) {
      Keyboard.press(keystrokes[9]);
    }
    else if (button2 == 1) {
      Keyboard.release(keystrokes[9]);
    }
  }
  lastButton2 = button2;
  
  // read the white Tinkerkit input on the left:
  buttonA = readTinkerkitInput(INPUT_A);
  if (lastButtonA != buttonA) {
    if (buttonA == 0) {
      Keyboard.press(keystrokes[10]);
    }
    else if (buttonA == 1023) {
      Keyboard.release(keystrokes[10]);
    }
  }
  lastButtonA = buttonA;

  // read the white Tinkerkit input on the right:
  buttonB = readTinkerkitInput(INPUT_B);
  if (lastButtonB != buttonB) {
    if (buttonB == 0) {
      Keyboard.press(keystrokes[11]);
    }
    else if (buttonB == 1023) {
      Keyboard.release(keystrokes[11]);
    }
  }
  lastButtonB = buttonB;

  // read the joystick button press:
  buttonJoy = Esplora.readJoystickButton();
  if (lastButtonJoy != buttonJoy) {
    if (buttonJoy == PRESSED) {
      Keyboard.press(keystrokes[12]);
    }
    else if (buttonJoy == RELEASED) {
      Keyboard.release(keystrokes[12]);
    }
  }
  lastButtonJoy = buttonJoy;

  delay(50);
}



