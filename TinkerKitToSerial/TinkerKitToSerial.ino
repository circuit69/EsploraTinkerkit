/* This sketch read the Esplora tinkerkit connections and sends the values
to the serial monitor.
 
 Modified by Joel Krueger 12-8-2013.
*/ 

#include <Esplora.h>

const int Tinkerkit1 = 11;        //Left orange Tinkerkit output pin 11 used as an input
const int Tinkerkit2 = 3;         //Right orange Tinkerkit output pin 3 used as an input

/* This sets up the functions to read the tinkerkit inputs.
 Esplora Tinkerkit Input Read by Mike Barela created on 2013-03-24
 http://21stdigitalhome.blogspot.com/2013/03/reading-arduino-esplora-tinkerkit-inputs.html
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
  Serial.begin(9600);
  //Initialize orange Tinkerkit outputs as inputs
  pinMode(Tinkerkit1,INPUT);
  pinMode(Tinkerkit2,INPUT);  
}

void loop() {
  Serial.print("Left Orange = " );                       
  Serial.print(digitalRead(Tinkerkit1));      
  Serial.print("\t Right Orange = ");      
  Serial.print(digitalRead(Tinkerkit2));   
  Serial.print("\t Left White = " );                       
  Serial.print(readTinkerkitInput(INPUT_A));      
  Serial.print("\t Right White = ");      
  Serial.println(readTinkerkitInput(INPUT_B)); 
  delay(500);
}
