#include <ElnCommunicator.h>
#define LED_PIN 8
#define LIGHT_TIME 350
#define DIM_TIME 150

// Initializing the protocol
ElnCommunicator Eln = ElnCommunicator();

/*
Sometimes you need to provide direct interaction between the pins and the Arduino.
In order to make it impossible to change their state from the outside, there is a
LockPin(int pin) method that prohibits changing the state or mode of this pin from
the outside (reading remains allowed). There is also the UnlockPin(int pin) method,
which removes the ban on editing the pin from the outside, and the isLocked(int pin)
function, which determines whether the pin is blocked.

Try to check out this functionality:
 0) a) Connect LED to some PWN pin. Change the LED_PIN to the pin
       number that you connected the LED to.
    b) Make sure that the connection speed is 500000 baud,
       and that the newline character is "NL".
 1) First send a connection request: "C". Arduino will respond
    with "I%type%", where %type% is the board type. The connection
    is established, and the LED starts flashing.
 2) Send the command "S01XX0", which should (in theory) change the
    mode of pin XX to the input, but this will not happen, the LED
    will continue to blink.
 3) Send the command "W01YY0X", which should (in theory) send the signal
    X to pin YY, but this will not happen, the LED will continue to blink.
*/

void setup() {
  // Setting up and locking the pin
  pinMode(LED_PIN, OUTPUT);
  Eln.LockPin(LED_PIN);
  // Waiting for a connection request
  Eln.OpenConnection();
}

long tm = 0;
bool light = false;
void loop() {
  if(Serial.available()) {
    // Reading a message only if it has arrived
    // (or started to arrive), so as not to stop the program.
    Eln.WaitMessage();
  }
  /*
    IMPORTANT:
    The WaitMessage function must be called as often as possible,
    so the delay() function is not applicable. Use the millis() function instead,
    which returns the time in milliseconds since the board was started.
  */
  if(light) {
    // If the LED is lit...
    if(millis() - tm > LIGHT_TIME) {
      // ...and the specified time has passed since
      // it was turned on, then turn it off.
      digitalWrite(LED_PIN, LOW);
      light = false;
      tm = millis();
    }
  } else {
    // If the LED is not lit...
    if(millis() - tm > DIM_TIME) {
      // ...and the specified time has passed since
      // it was turned off, then turn it on.
      digitalWrite(LED_PIN, HIGH);
      light = true;
      tm = millis();
    }
  }
}