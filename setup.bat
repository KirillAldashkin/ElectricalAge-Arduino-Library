@rmdir /S /Q "%appdata%\..\..\Documents\Arduino\libraries\ElectricalAge-Arduino-Library"
@mkdir "%appdata%\..\..\Documents\Arduino\libraries\ElectricalAge-Arduino-Library"
@copy /B ElnCommunicator.cpp /B "%appdata%\..\..\Documents\Arduino\libraries\ElectricalAge-Arduino-Library\" /B
@copy /B ElnCommunicator.h /B "%appdata%\..\..\Documents\Arduino\libraries\ElectricalAge-Arduino-Library\" /B
@copy /B keywords.txt /B "%appdata%\..\..\Documents\Arduino\libraries\ElectricalAge-Arduino-Library\" /B
@xcopy examples "%appdata%\..\..\Documents\Arduino\libraries\ElectricalAge-Arduino-Library\examples\" /E