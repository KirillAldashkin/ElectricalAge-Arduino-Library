using System;
using System.Text;
using System.IO.Ports;
using System.Diagnostics;
using System.Threading;

/*
  Stress Test
  Sends the next packet as soon as the Arduino processes
  the previous one. Displays information about the speed.
*/

// COM Port speed
const int SPEED = 500000;
// Options for establishing the connection
const int CONNECT_REQUEST_DELAY = 25;
const int MAX_CONNECT_REQUESTS = 16;
// Number of numeric channels
const int NUMERIC_CHANNELS_COUNT = 32;
// Pin parameters
int[] DIGITAL_READ = {2, 4, 7, 8, 12, 13};
int[] ANALOG_READ = {14, 15, 16, 17, 18, 19};
int[] PWM_WRITE = {3, 5, 6, 9, 10, 11};


// Getting the port name
Console.WriteLine("Available COM ports:");
foreach(string c_port in SerialPort.GetPortNames())
    Console.WriteLine($"  {c_port}");
Console.Write("Enter the COM port number: ");
var port_name = Console.ReadLine();

// Opening the COM port
Console.WriteLine($"The \'{port_name}\' port is selected.");
SerialPort port = new SerialPort();
Console.Write("Opening the port... ");
try {
    port = new SerialPort((string)port_name, SPEED);
    port.NewLine = "\n";
    port.Open();
} catch(Exception e) {
    Console.WriteLine();
    Exit(e.ToString(), 1);
}
Console.WriteLine($"Done.");

// Establishing a connection
Console.Write("Establishing a connection... Request ");
var rq = 1;
var cl = Console.CursorLeft;
while(port.BytesToRead == 0 && rq < MAX_CONNECT_REQUESTS+1) {
    Console.CursorLeft = cl;
    Console.Write(rq++);
    Console.Write(" of ");
    Console.Write(MAX_CONNECT_REQUESTS);
    port.WriteLine("C");
    Thread.Sleep(CONNECT_REQUEST_DELAY);
}
if(rq == MAX_CONNECT_REQUESTS+1)
    Exit($"The response was not received after {MAX_CONNECT_REQUESTS} requests.", 2);
var board_type = port.ReadLine().Substring(1).TrimEnd('_');
Console.WriteLine();
Console.WriteLine($"The connection is established. Board Type: {board_type}");

// Stress Test
Console.CursorTop++;
Stopwatch stp = new();
DateTime lastPrintTime = new(0);
while(true) {
    var request = GeneratePacket();
    stp.Reset();
    stp.Start();
    port.WriteLine(request);
    var response = port.ReadLine(); 
    stp.Stop();
    if ((DateTime.Now - lastPrintTime).TotalMilliseconds > 250)
    {
        lastPrintTime = DateTime.Now;
        var freq = 1000 / stp.ElapsedMilliseconds;
        var datarate = freq * (request.Length + response.Length);
        string r(long size) => size != 1 ? "s" : ""; // Defines the end of a word
        string[] info = new string[] {
            $"|Request size: {request.Length} byte{r(request.Length)}",
            $"|Response size: {response.Length} byte{r(response.Length)}",
            $"|Request delay: {stp.ElapsedMilliseconds} milliseconds.",
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
void Exit(string e = "", int code = 0) {
    port.Close();
    // Print an error if there is one
    if(e != "") {
        var c = Console.ForegroundColor;
        Console.ForegroundColor = ConsoleColor.Red;
        Console.WriteLine();
        Console.WriteLine("Error:");
        Console.WriteLine(e);
        Console.ForegroundColor = c;
    }
    Console.Write("Press any key to exit...");
    Console.ReadKey(true);
    Environment.Exit(code);
}

// Generates a packet that randomly changes the numeric
// channels and operates with the specified pins.
string GeneratePacket() {
    // Random numeric channels operations
    Random rnd = new Random();
    StringBuilder sb = new("N");
    sb.Append(NUMERIC_CHANNELS_COUNT);
    for(var i = 0; i < NUMERIC_CHANNELS_COUNT; i++) {
        sb.Append(i.ToString().PadLeft(2, '0'));
        if(i%2==0) {
            sb.Append(0);
        } else {
            sb.Append(1);
            var val = rnd.Next(-32768, 32768);
            if(val < 0) {
                val = -val;
                sb.Append('-');
            }
            sb.Append(val.ToString().PadLeft(5, '0'));    
        }
    }
    // Random read operations
    sb.Append("R");
    sb.Append((DIGITAL_READ.Length+ANALOG_READ.Length).ToString().PadLeft(2, '0'));
    foreach(var pin in ANALOG_READ) {
        sb.Append(pin.ToString().PadLeft(2, '0'));
        sb.Append('1');
    }
    foreach(var pin in DIGITAL_READ) {
        sb.Append(pin.ToString().PadLeft(2, '0'));
        sb.Append('0');
    }
    // PWM Write operations
    sb.Append("W");
    sb.Append(PWM_WRITE.Length.ToString().PadLeft(2, '0'));
    foreach(var pin in PWM_WRITE) {
        sb.Append(pin.ToString().PadLeft(2, '0'));
        sb.Append('1');
        sb.Append(rnd.Next(256).ToString().PadLeft(3, '0'));
    }
    return sb.ToString();
}