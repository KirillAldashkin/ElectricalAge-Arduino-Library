using System;
using System.Diagnostics;
using System.IO.Ports;
using System.Text;

/*
  Stress Test
  Sends the next packet as soon as the Arduino processes
  the previous one. Displays information about the speed.
*/

// COM Port speed
const int COMBaudRate = 500000;
// Options for establishing the connection
const int ConnectRequestDelay = 25;
const int MaximumConnectRequests = 16;
// Number of numeric channels
const int NumericChannelsCount = 32;
// Pin parameters
int[] DigitalReadPins = { 2, 4, 7, 8, 12, 13 };
int[] AnalogReadPins = { 14, 15, 16, 17, 18, 19 };
int[] PWMWritePins = { 3, 5, 6, 9, 10, 11 };
// Port for future use
SerialPort currentPort = new();
// Some events
Console.CancelKeyPress += (s, e) => Exit("User cancelled", 0, false);
AppDomain.CurrentDomain.UnhandledException += (s, e) => Exit($"Unhandled exception:\r\n{e.ExceptionObject}", 4, false);


// Getting the port name
Console.WriteLine("Available COM ports:");
foreach (string comPort in SerialPort.GetPortNames())
    Console.WriteLine($"  {comPort}");
Console.Write("Enter the COM port number: ");
var currentPortName = Console.ReadLine();

// Opening the COM port
Console.WriteLine($"The \'{currentPortName}\' port is selected.");
Console.Write("Opening the port... ");
try
{
    currentPort = new(currentPortName, COMBaudRate);
    currentPort.NewLine = "\n";
    currentPort.Open();
}
catch (Exception e)
{
    Console.WriteLine();
    Exit($"Error while opening COM port:\r\n{e}", 1);
}
Console.WriteLine($"Done.");

// Establishing a connection
Console.Write("Establishing a connection... Request ");
var requestNumber = 1;
var requestCursorLeft = Console.CursorLeft;
while (currentPort.BytesToRead == 0 && requestNumber <= MaximumConnectRequests)
{
    Console.CursorLeft = requestCursorLeft;
    Console.Write(requestNumber++);
    Console.Write(" of ");
    Console.Write(MaximumConnectRequests);
    currentPort.WriteLine("C");
    System.Threading.Thread.Sleep(ConnectRequestDelay);
}
if (requestNumber > MaximumConnectRequests)
    Exit($"The response was not received after {MaximumConnectRequests} requests.", 2);
var boardType = currentPort.ReadLine();
if(boardType.Length == 0 || boardType[0] != 'I')
    Exit($"The answer (\"{boardType}\") is not the type of board (maybe the board was already enabled earlier).", 3, true);
Console.WriteLine();
Console.WriteLine($"The connection is established. Board Type: {boardType.Substring(1).TrimEnd('_')}");

// Stress Test
Stopwatch delayCounter = new();
DateTime lastPrintTime = new(0);
while (currentPort.IsOpen)
{
    var request = GeneratePacket();
    delayCounter.Reset();
    delayCounter.Start();
    currentPort.WriteLine(request);
    var response = currentPort.ReadLine();
    delayCounter.Stop();
    // Displaying information after an interval
    if ((DateTime.Now - lastPrintTime).TotalMilliseconds > 250)
    {
        lastPrintTime = DateTime.Now;
        var freq = 1000 / delayCounter.ElapsedMilliseconds;
        var datarate = freq * (request.Length + response.Length);
        string r(long size) => size != 1 ? "s" : ""; // Defines the end of a word
        string[] info = new string[] {
            $"|Request size: {request.Length} byte{r(request.Length)}",
            $"|Response size: {response.Length} byte{r(response.Length)}",
            $"|Request delay: {delayCounter.ElapsedMilliseconds} milliseconds",
            $"|Estimated frequency: {freq} packet{r(freq)} per second",
            $"|Estimated data rate: {datarate} byte{r(datarate)} per second"
        };
        var oldCoursorTop = Console.CursorTop;
        foreach (var s in info)
            Console.WriteLine(s.PadRight(Console.WindowWidth));
        Console.CursorTop = oldCoursorTop;
    }
}

// Exit function
void Exit(string info = "", int code = 0, bool waitExit = true)
{
    currentPort.Close();
    // Print a info if any
    if (info != "")
    {
        var c = Console.ForegroundColor;
        // If this is an error
        if (code != 0)
        {
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine();
            Console.WriteLine("Error:");
        }
        else
            Console.ForegroundColor = ConsoleColor.Yellow;
        Console.WriteLine(info);
        Console.ForegroundColor = c;
    }
    if (waitExit)
    {
        Console.Write("Press any key to exit...");
        Console.ReadKey(true);
    }
    Environment.Exit(code);
}

// Generates a packet that randomly changes the numeric
// channels and operates with the specified pins.
string GeneratePacket()
{
    // Random numeric channels operations
    Random random = new();
    StringBuilder packet = new("N");
    packet.Append(NumericChannelsCount);
    for (var i = 0; i < NumericChannelsCount; i++)
    {
        packet.Append(i.ToString().PadLeft(2, '0'));
        if (i % 2 == 0)
            packet.Append(0);
        else
        {
            packet.Append(1);
            var val = random.Next(-32768, 32768);
            if (val < 0)
            {
                val = -val;
                packet.Append('-');
            }
            packet.Append(val.ToString().PadLeft(5, '0'));
        }
    }
    // Random read operations
    packet.Append('R');
    packet.Append((DigitalReadPins.Length + AnalogReadPins.Length).ToString().PadLeft(2, '0'));
    foreach (var pin in AnalogReadPins)
    {
        packet.Append(pin.ToString().PadLeft(2, '0'));
        packet.Append('1');
    }
    foreach (var pin in DigitalReadPins)
    {
        packet.Append(pin.ToString().PadLeft(2, '0'));
        packet.Append('0');
    }
    // PWM Write operations
    packet.Append('W');
    packet.Append(PWMWritePins.Length.ToString().PadLeft(2, '0'));
    foreach (var pin in PWMWritePins)
    {
        packet.Append(pin.ToString().PadLeft(2, '0'));
        packet.Append('1');
        packet.Append(random.Next(256).ToString().PadLeft(3, '0'));
    }
    return packet.ToString();
}