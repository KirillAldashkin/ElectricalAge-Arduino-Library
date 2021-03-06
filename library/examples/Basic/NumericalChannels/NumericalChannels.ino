#include <ElnCommunicator.h>
#define LED_PIN 9
#define POT_PIN A0

// Initializing the protocol
ElnCommunicator Eln = ElnCommunicator();

/*
Unfortunately, ElectricalAge can directly perform simple work with pins, but
it cannot work with complex devices, such as a servomotor and humidity sensor.

There is a solution - numerical channels. There are 32 of them in total, and
each of them can store a integer value from -32768 to 32767. Both Arduino and
PC can write and read values from them.

For example, the PC writes a value to channel 0, and the Arduino rotates the
servomotor angle recorded in this channel. And the Arduino reads the humidity
level from the sensor and writes this value to channel 1, and the PC reads it.

Try controlling the numerical channels through the port monitor:
 0) a) Connect LED to some PWN pin and potentiometer to some analog input.
       Change the LED_PIN and POT_PIN to the pin numbers that you connected
       the LED and the potentiometer to.
    b) Make sure that the connection speed is 500000 baud, and that the newline
       character is "NL".
 1) First send a connection request: "C". Arduino will respond with "I%type%",
    where %type% is the board type. The connection is established.
 2) Send the command "N0100100127". The LED will light up halfway. Also you will
    get a response of "N00", which  means that you did not read any channels, 
    only wrote them down. Try entering a different number instead of 00127, but
    add up to 5 digits with leading zeros.
 3) Send the command "N01010". You will get the answer "N0101XXXXX", where XXXXX
    is the value from the potentiometer.
*/

void setup() {
  // Setting up and locking the pin
  pinMode(LED_PIN, OUTPUT);
  Eln.LockPin(LED_PIN);
  // Waiting for a connection request
  Eln.WaitOpenConnection();
}
int val;
void loop() {
  // Write the value from the potentiometer to channel 1
  // (it can be a complex device instead).
  Eln.Channels[1] = analogRead(POT_PIN);
  // Waiting for a message
  Eln.WaitMessage();
  // Processing the value from channel 0 and writing it to the LED
  // (it can be a complex device instead).
  val = constrain(Eln.Channels[0], 0, 255);
  analogWrite(LED_PIN, val);
}