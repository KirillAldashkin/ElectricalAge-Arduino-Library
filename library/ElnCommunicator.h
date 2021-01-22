#ifndef ElnCommunicator_h
#define ElnCommunicator_h
#include <Arduino.h>
class ElnCommunicator {
	public:
		void WaitMessage(); // Stops the program waiting for a message
		void OpenConnection(); // Stops the program while waiting for the connection to open
		void LockPin(int pin); // Blocks the pin from being changed from the outside
		void UnlockPin(int pin); // Unblocks the pin
		bool IsLocked(int pin); // Determines whether the pin is blocked
		int16_t Channels[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // Numerical channels
	private:
		void readSettings();
		void readMessageWritePins();
		void readMessageReadPins();
		void editChannels();
		void intToBuffer(int value, int length);
		char buffer[512]; // The response buffer
		int buffer_pointer = 0;
		uint8_t lock_flags[8] = {0,0,0,0,0,0,0,0};
};
#endif
