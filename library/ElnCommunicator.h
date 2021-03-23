#ifndef ElnCommunicator_h
#define ElnCommunicator_h
#include <Arduino.h>
#define DEBUG false
#define SPEED 500000
class ElnCommunicator {
	public:
		bool IsConnected();
		bool CheckMessage();
		void WaitMessage(); // Stops the program waiting for a message
		bool TryOpenConnection(); 
		void WaitOpenConnection(); // Stops the program while waiting for the connection to open
		void LockPin(int pin); // Blocks the pin from being changed from the outside
		void UnlockPin(int pin); // Unblocks the pin
		bool IsLocked(int pin); // Determines whether the pin is blocked
		int16_t Channels[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // Numerical channels
	private:
		bool connected = false;
		void changePinModes();
		void readMessageWritePins();
		void readMessageReadPins();
		void editChannels();
		void intToBuffer(int value, int length);
		char buffer[512];
		int buffer_pointer = 0;
		bool serial_opened = false;
		uint8_t lock_flags[8] = {0,0,0,0,0,0,0,0};
		#if DEBUG
			String ElnCommunicator::intToString(int16_t value);
			void ElnCommunicator::debugPrintLockFlags();
			void ElnCommunicator::debugPrintChannels();
		#endif
};
#endif
