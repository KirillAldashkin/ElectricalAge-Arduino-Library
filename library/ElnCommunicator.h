#ifndef ElnCommunicator_h
#define ElnCommunicator_h
#include <Arduino.h>
#define DEBUG false
#define SPEED 500000
// Provides the ability to communicate between the Arduino board and the ElectricalAge mod.
class ElnCommunicator {
	public:
		// If the connection was established, returns true, otherwise returns false.
		bool IsConnected();
		/* 
		  If the input buffer is not empty, the message is read and processed from it,
		  and the method returns true. If the input buffer is empty, returns false.
		*/
		bool CheckMessage();
		// Waits for a message, then processes it.
		void WaitMessage();
		/*
		  If the connection is already established, it does nothing and returns true.
		  Otherwise, if there is data in the input buffer, the connection request is
		  read from it and processed, and method returns true. If the input buffer is
		  empty, method returns false.
		*/
		bool TryOpenConnection(); 
		/*
		  If the connection is already established, does nothing, otherwise waits for
		  the connection request and processes it.
		*/
		void WaitOpenConnection();
		// Blocks the pin, forbidding any operations on it from the PC.
		void LockPin(int pin);
		// Unlocks the pin, allowing operations on it from the PC.
		void UnlockPin(int pin);
		// If the pin is blocked, returns true, otherwise returns false.
		bool IsLocked(int pin);
		/*
		  Provides 32 [0...31] numeric channels that store integers from -32768 to 32767.
		  Can be changed both by the PC and Arduino.
		*/
		int16_t Channels[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
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
			String intToString(int16_t value);
			void debugPrintLockFlags();
			void debugPrintChannels();
		#endif
};
#endif
