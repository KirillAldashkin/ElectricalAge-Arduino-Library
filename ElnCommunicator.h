#ifndef ElnCommunicator_h
#define ElnCommunicator_h
#include <Arduino.h>
class ElnCommunicator {
	public:
		//ElnCommunicator(); // Constructor
		void WaitMessage(); // Stops the program waiting for a message
		void OpenConnection(); // Stops the program while waiting for the connection to open
		int16_t Channels[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // Numerical channels
	private:
		void readSettings();
		void readMessageWritePins();
		void readMessageReadPins();
		void editChannels();
		void intToBuffer(int value, int length);
		char buffer[512]; // The response buffer
		int buffer_pointer = 0;
		
};
#endif
