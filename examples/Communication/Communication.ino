#include <ElnCommunicator.h>

// Initializing the protocol
ElnCommunicator Eln = ElnCommunicator();

void setup() {
  // You do not need to start Serial,
  // it starts automatically at a speed of 115200
  // ATTENTION: the serial connection is completely
  // occupied by the protocol, you can not write or
  // read anything directly into it!
}
void loop() {
  // Waiting for a message
  Eln.GetMessage();
}