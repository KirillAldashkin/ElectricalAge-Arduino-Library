[back to README](./README.md)
# Protocol

## About
This protocol is an add-on over RS232, and operates at a speed of 115200 baud.
Messages are sent in packets. One packet from PC - one response from Arduino.

*IMPORTANT: When sending numbers, they must have a fixed length.
For example, if the number is to be a three-digit number, but you need to send "16", you need to add leading zeros and send "016".*

## Packet
The **packet** is an ASCII string ending with an end-of-line character ('\n'). It consists of **commands** that can (but should not) be separated by spaces.
For example, the packet *"S02081093W010801"* contains the commands *"S02081093"* and *"W010801"*. Arduino processes commands in the order in which they are specified in the packet.

## Connection establishment
The computer should send "C" packets until the Arduino sends an "I%type%" response, where &type& is the board type ("UNO", "MINI", and so on).
Now the Arduino can receive commands, the connection is established.

## Command
A **command** is a part of a packet in which several instructions are encoded, on which pin to write the value, from which to read, and so on.
Any command starts with a single-character header that identifies which command it is. List of commands:

### Set pin modes
A command that sets pin modes. Doesn't add anything to the response. Starts with the 'S' character.
Next comes a two-digit number - the number of pins whose modes need to be changed.
Each pin is described by three digits:
>1) The first is a two-digit: pin number.
>2) The second is a single-digit: pin mode number:
>>* 0 - Input
>>* 1 - Input with pull-up
>>* 2 - Output

For example, the command "S03032140081" means that:
1) Pin 3 should be switched to output mode;
2) Pin 14 should be switched to input mode;
3) Pin 8 should be switched to the input pull-up mode.

### Write values to pins
The command that writes values to pins. Doesn't add anything to the response. Starts with the 'W' character.
Next comes a two-digit number - the number of pins to write values to.
Each pin is described by several digits:
>1) The first is a two-digit: pin number.
>2) The second is a single-digit: pin write mode:
>>* 0 - Digital write
>>* 1 - PWM write
>3) The value to be written to the pin is one-digit (0 or 1, larger values will be interpreted as 1) if the write mode is digital, and three-digit (0...255, larger values will be interpreted as 255) if the write mode is PWM.

For example, the command "W020310960601" means that:
1) To pin 3, Arduino need to send a PWM signal with a duty cycle of 96/255.
2) To pin 6, Arduino need to send a high signal.

### Reading values from pins
A command that reads values from pins. Adds read results to the response. Starts with the 'R' character.
Next comes a two-digit number - the number of pins to read values from. Each pin is described by three digits:
>1) The first is a two-digit: pin number.
>2) The second is a single-digit: pin read mode:
>>* 0 - Digital read
>>* 1 - Analog read

The order of writing in the response:

The 'R' character is added first.
Then a two-digit number is added - the number of pins read. For each pin, the response is added:
>1) Two-digit number - pin number.
>2) The value read from the pin. One-digit (0...1) if the reading is digital, and four-digit (0...1023) if the reading is analog.

For example, the command "R02140081" means that:
1) Arduino need to read the analog value from pin 14
2) Arduino need to read the digital value from pin 8

The response "R02140768081" means that:
1) On pin 14, the signal is 768/1024.
2) On pin 8, the signal is high.

### Numerical channels
A command that allows you to operate with numerical channels. Starts with the 'N' character.
Next comes a two-digit number - the number of channels. Each channel is described by several numbers:
>1) The first is two-digit: channel number (0...31, larger values will be processed by the remainder of division by 32).
>2) The second is single-digit: mode: 0 - read, other values - write.
>3) If the mode is write, then 5 (6, if the number is negative) characters are the number to be written.
The number itself is a five-digit number, and the minus for negative numbers is placed before the leading zeros,
for example: "-00128".

The order of writing in the response:

The 'R' character is added first.
Then a two-digit number is added - the number of channels readed (can be zero if the values were only written).
A response is added for each channel read:
>1) Two-digit number - channel number.
>2) The value recorded in the channel. The same principle is used as when sending a request.

![logo](https://raw.githubusercontent.com/jrddunbr/electrical-age.github.io/master/assets/favicon.ico)
