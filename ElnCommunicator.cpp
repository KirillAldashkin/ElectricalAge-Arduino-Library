#include <ElnCommunicator.h>
//settings
#define DEBUG false
//defines for reading decimal digits
#define readOneDigit (Serial.read() - 48)
#define readTwoDigits (readOneDigit * 10 + readOneDigit)
#define readThreeDigits (readTwoDigits * 10 + readOneDigit)
#define readFourDigits (readThreeDigits * 10 + readOneDigit)
#define readFiveDigits (readFourDigits * 10 + readOneDigit)

void ElnCommunicator::OpenConnection() {
    #if DEBUG
		Serial.println("Wait connection request");
	#endif
	Serial.begin(115200);
	// Clearing the serial port
	char reading;
	do {
		while(Serial.available() == 0);
		reading = Serial.read();
	} while(reading != '\n');
    // Waiting for the connection request
	while(Serial.available() < 2);
	char a = Serial.read();
	char b = Serial.read();
	while(true) {
		#if DEBUG
			Serial.println("Wait two symbols");
		#endif
		while(Serial.available() > 0) {
			a = b;	
			b = Serial.read();
		}
		#if DEBUG
			Serial.print("Symbols are: "); Serial.print(a); Serial.println(b);
		#endif
		if(a == 'C' && b == '\n') break;
	}
	// Sending the type of board
	Serial.write('I'); // Header
	#if defined(__AVR_ATmega328__) // Arduino Uno
		Serial.print("UNO_");
	#elif defined(__AVR_ATmega2560__) // Arduino Mega
		Serial.print("MEGA");
	#elif defined(__AVR_ATmega32u__) // Arduino Nano / Arduino Mini
		Serial.print("NANO");
	#else
		Serial.print("UNKNOWN"); // Unknown board type (or my code doesn't work lol)
	#endif
    Serial.write('\n');
}
void ElnCommunicator::WaitMessage() {
	while(true) {
		while(Serial.available() == 0);
		char header = Serial.read();
		if(header == '\n') {
			// End of the packet
			#if DEBUG
				Serial.println("End of the packet");
			#endif
			break;
		} else if(header == ' ' || header == '\t') {
			// Not to do anything
			// You can format queries with whitespace symbols when entering them manually
			#if DEBUG
				Serial.println("Whitespace");
			#endif
		} else if(header == 'S' || header == 's') {
			// Commands for setting pin modes
			#if DEBUG
				Serial.println("Commands for setting pin modes");
			#endif
			readSettings();
		} else if(header == 'W' || header == 'w') {
			// Commands to write pin values
			#if DEBUG
				Serial.println("Commands to write pin values");
			#endif
			readMessageWritePins();
		} else if(header == 'R' || header == 'r') {
			// Commands to read pin values
			#if DEBUG
				Serial.println("Commands to read pin values");
			#endif
			readMessageReadPins();
		} else if(header == 'N' || header == 'n') {
			// Commands to read/write numerical channels
			#if DEBUG
				Serial.println("Commands to read/write numerical channels");
			#endif
			editChannels();
		} else {
			// Cleaning out the trash
			#if DEBUG
				Serial.println("Cleaning out the trash");
				Serial.write(header);
			#endif
			char reading;
			do {
				while(Serial.available() == 0);
				reading = Serial.read();
				#if DEBUG
					Serial.write(reading);
				#endif
			} while(reading != '\n');
			break;
		}
	}
	// Write the response and
	// reset the pointer to the buffer
	for(int i = 0; i < buffer_pointer; i++) {
		Serial.write(buffer[i]);
	}
	Serial.write('\n');
	buffer_pointer = 0;
}
void ElnCommunicator::readSettings() {
	while(Serial.available() < 2);
	int count = readTwoDigits;
	#if DEBUG
		Serial.print("  Count: "); Serial.println(count);
	#endif
	for(int i = 0; i < count; i++)
	{
		//First two digits: pin number
		//Last digit: pin mode
		while(Serial.available() < 3);
		int pin_number = readTwoDigits;
		int pin_mode = readOneDigit;
		#if DEBUG
			Serial.print("  "); Serial.print(i); Serial.println(":");
			Serial.print("    Pin number: "); Serial.println(pin_number);
			Serial.print("    Pin mode: "); Serial.println(pin_mode);
		#endif
		if(pin_mode == 0) {
			// Input
			#if DEBUG
				Serial.println("      Input");
			#endif
			pinMode(pin_number, INPUT);
		} else if(pin_mode == 1) {
			// Input with pullup
			#if DEBUG
				Serial.println("      Input w/h pullup");
			#endif
			pinMode(pin_number, INPUT_PULLUP);
		} else if (pin_mode == 2) {
			// Output
			#if DEBUG
				Serial.println("      Output");
			#endif
			pinMode(pin_number, OUTPUT);
        }
	}
}
void ElnCommunicator::readMessageWritePins() {
	while(Serial.available() < 2);
	int count = readTwoDigits;
	#if DEBUG
		Serial.print("  Count: "); Serial.println(count);
	#endif
	for(int i = 0; i < count; i++) {
		// First two digits: pin number
		// Third digit: write mode
		// The last three (for PWM) or one (for digital) digits: value
		while(Serial.available() < 3);
		int pin_number = readTwoDigits;
		int write_mode = readOneDigit;
		#if DEBUG
			Serial.print("  "); Serial.print(i); Serial.println(":");
			Serial.print("    Pin number: "); Serial.println(pin_number);
			Serial.print("    Write mode: "); Serial.println(write_mode);
		#endif
		int write_value;
		if(write_mode == 0) {
			// Digital Write
			while(Serial.available() == 0);
			write_value = readOneDigit;
			#if DEBUG
				Serial.print("    Digital Write: "); Serial.println(write_value);
			#endif
			digitalWrite(pin_number, (write_value != 0));
		} else if (write_mode == 1) {
			// PWM Write
			while(Serial.available() < 3);
			write_value = readThreeDigits;
			if(write_value > 255)
				write_value = 255;
			#if DEBUG
				Serial.print("    PWM Write: "); Serial.println(write_value);
			#endif
			analogWrite(pin_number, write_value);
		}
	}
}
void ElnCommunicator::readMessageReadPins() {
	while(Serial.available() < 2);
	int count = readTwoDigits;
	#if DEBUG
		Serial.print("  Count: "); Serial.println(count);
		Serial.print("  Buffer pointer: "); Serial.println(buffer_pointer);
	#endif
	buffer[buffer_pointer++] = 'R';
	intToBuffer(count, 2);
	for(int i = 0; i < count; i++) {
		// First two digits: pin number
		// Third digit: read mode
		while(Serial.available() < 3);
		int pin_number = readTwoDigits;
		int read_mode = readOneDigit;
		#if DEBUG
			Serial.print("  "); Serial.print(i); Serial.println(":");
			Serial.print("    Pin number: "); Serial.println(pin_number);
			Serial.print("    Read mode: "); Serial.println(read_mode);
		#endif
		intToBuffer(pin_number, 2);
		if(read_mode == 0) {
			// Digital Read
			bool read_value = digitalRead(pin_number);
			#if DEBUG
				Serial.print("    Digital Read: "); Serial.println(read_value);
			#endif
			buffer[buffer_pointer++] = (char)(48 + read_value);
		} else if (read_mode == 1) {
			// Analog read
			int read_value = analogRead(pin_number);
			#if DEBUG
				Serial.print("    Analog Read: "); Serial.println(read_value);
			#endif
			intToBuffer(read_value, 4);
		}
		#if DEBUG
			Serial.print("    Buffer pointer: "); Serial.println(buffer_pointer);
		#endif
	}
}
void ElnCommunicator::editChannels() {
	while(Serial.available() < 2);
	int count = readTwoDigits;
	#if DEBUG
		Serial.print("  Count: "); Serial.println(count);
		Serial.print("  Buffer pointer: "); Serial.println(buffer_pointer);
	#endif
	buffer[buffer_pointer++] = 'N';
	int count_pointer = buffer_pointer; // Memorizing the position to record the number of channels read later
	buffer_pointer += 2;
	int readed_count = 0;
	for(int i = 0; i < count; i++) {
		while(Serial.available() < 3);
		int channel_number = readTwoDigits % 32;
		int mode = readOneDigit;
		#if DEBUG
			Serial.print("  "); Serial.print(i); Serial.println(':');
			Serial.print("    Channel number: "); Serial.println(channel_number);
			Serial.print("    Mode: "); Serial.println(channel_number);
		#endif
		if(mode == 0) {
			// Reading value
			readed_count++;
			#if DEBUG
				Serial.print("    Reading value "); Serial.println(readed_count);
			#endif
			intToBuffer(channel_number, 2);
			int16_t val = Channels[channel_number];
			if(val < 0) {
				val = -val;
				buffer[buffer_pointer++] = '-';
				#if DEBUG
					Serial.println("    Is negative");
				#endif
			}
			intToBuffer(val, 5);
			#if DEBUG
				Serial.print("    Absolute value: "); Serial.println(val);
			#endif
		} else {
			// Writing value
			#if DEBUG
				Serial.println("    Writing value");
			#endif
			bool negative = false;
			while(Serial.available() == 0);
			if(Serial.peek() == '-') {
				Serial.read();
				negative = true;
				#if DEBUG
					Serial.println("    Is negative");
				#endif
			}
			while(Serial.available() < 5);
			int16_t val = readFiveDigits;
			#if DEBUG
				Serial.print("    Absolute value: "); Serial.println(val);
			#endif
			if(negative) val = -val;
			Channels[channel_number] = val;
		}
		// Recording the number of channels read
		int cur_pointer = buffer_pointer;
		buffer_pointer = count_pointer;
		intToBuffer(readed_count, 2);
		buffer_pointer = cur_pointer;
	}
}
void ElnCommunicator::intToBuffer(int value, int length) {
	// Write this number to the response buffer,
	// adding leading zeros to the specified length
	for(int i = 0; i < length; i++)
		buffer[buffer_pointer++] = '0';
	int count = 0;
	while(value > 0) {
		buffer[--buffer_pointer] = (char)(value % 10 + 48);
		value /= 10;
		count++;
	}
	buffer_pointer += count;
}
