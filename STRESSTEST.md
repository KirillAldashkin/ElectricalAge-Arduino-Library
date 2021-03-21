[back to main](https://github.com/KirillAldashkin/ElectricalAge-Arduino-Library)
# Stress-test

## About
This utility performs a protocol stress test by sending the next packet immediately after the Arduino responds to the previous one.

This is important because in a game, one tick lasts 1/20 of a second, and the Arduino should not be late in sending a response.

## How to get started
* This program is written in C# and requires `.Net 5`. First install the SDK, it is available [here](https://dotnet.microsoft.com/download/dotnet/5.0).
* Download the repository
* Open the `\test\Program.cs` file and change the lines

```C#
// Pin parameters
int[] DIGITAL_READ = {2, 4, 7, 8, 12, 13};
int[] ANALOG_READ = {14, 15, 16, 17, 18, 19};
int[] PWM_WRITE = {3, 5, 6, 9, 10, 11};
```

by entering the pins that are suitable for your board (By default, the pins for the *Arduino Uno* are entered).

* Open the console in the `\test` folder and enter the command: `dotnet build`. It compiles the code.
* Then enter the command `dotnet bin\Debug\net5.0\test.dll`. It will launch the program. Follow the program instructions.

![logo](https://raw.githubusercontent.com/KirillAldashkin/ElectricalAge-Arduino-Library/main/graphics/bottom.png)
