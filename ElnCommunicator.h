#ifndef ElnCommunicator_h
#define ElnCommunicator_h
#include <Arduino.h>
class ElnCommunicator {
	public:
		ElnCommunicator();
		void WaitMessage();
		void OpenConnection();
	private:
		void readSettings();
		void readMessageWritePins();
		void readMessageReadPins();
		void intToBuffer(int value, int length);
		char buffer[512];
		int buffer_pointer;
};
#endif
