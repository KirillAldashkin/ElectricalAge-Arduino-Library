![logo](https://raw.githubusercontent.com/KirillAldashkin/ElectricalAge-Arduino-Library/main/graphics/big_logo.png)
## About
This is library for Arduino that allows it to communicate with the **Electrical Age**.

**Electrical Age** is a mod for Minecraft, developed by [jrddunbr](https://github.com/jrddunbr) and [his team](https://github.com/jrddunbr/ElectricalAge#main-developers). It is available [here](https://github.com/jrddunbr/ElectricalAge).

## How to get started
1) **Download** this repository as a ZIP archive (Code > Download ZIP) then unpack it.
2) **Import** it into the Arduino IDE using `setup.bat` file\* then re-open Arduino IDE.
3) **Learn** the examples and create your own program to interact with the game!

\* - Only works on Windows and the default installed non-store version of the Arduino IDE. Otherwise, pack the contents of the `library` folder in a zip archive and import it via the Arduino IDE.

## Features
### 1) Working with pins
You can read, write values and set pin modes from ElectricalAge.
### 2) Numerical channels
You will have 32 channels at your disposal, where both the ElectricalAge and the Arduino will be able to write and read values.
### 3) Pin Blocking
You will be able to lock pins on the Arduino and work with them without fear that its value or mode will change from the ElectricalAge.

## This is important!
This library uses its own [protocol](./PROTOCOL.md), which completely occupies the serial port.

## Files
* `library\` - the entire library for Arduino
* `test\` - stress test of the protocol. Read more [here](./STRESSTEST.md)
* `*.md` - description files
* `setup.bat` - the installation script

![logo](https://raw.githubusercontent.com/KirillAldashkin/ElectricalAge-Arduino-Library/main/graphics/bottom.png)
