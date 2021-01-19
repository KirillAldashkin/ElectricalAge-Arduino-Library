#include <ElnCommunicator.h>

// Initializing the protocol
ElnCommunicator Eln = ElnCommunicator();

/*
Try to communicate with Arduino via this protocol via the port monitor:
 0) Make sure that the connection speed is 115200 baud,
    and that the newline character is "NL".
 1) First send a connection request: "C". Arduino will respond
    with "I%type%", where %type% is the board type.
    The connection is established.
 2) Set the pin modes. Send the command "R02082090", which will set
    pin 8 to output mode and pin 9 to input mode. Arduino will send
    an empty response (newline character).
 3) Send the command "W010801", which will set a high signal on pin 8.
    Arduino will send an empty response (newline character). Then send
    the command "W010800", which will set the low signal on pin 8.
    Arduino will again send an empty response (a newline character).
 4) Read the value from pin 9. To do this, send the command "R01090".
    Arduino will send the response "R0109X", where X is the digital
    value on pin 9. Attach something to this pin and send this command
    a few more times.
Learn more about this protocol here: https://github.com/KirillAldashkin/ElectricalAge-Arduino-Library/blob/main/PROTOCOL.md
*/
void setup() {
  /*
  You do not need to start Serial,
  it starts automatically at a speed of 115200
  ATTENTION: the serial connection is completely
  occupied by the protocol, you can not write or
  read anything directly into it!
  */
  Eln.OpenConnection();
}
void loop() {
  // Waiting for a message
  Eln.WaitMessage();
}
