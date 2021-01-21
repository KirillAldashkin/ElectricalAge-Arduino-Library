using System;
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
// Pin parameters
int[] DIGITAL_READ = {2, 4, 7, 8, 12, 13};
int[] ANALOG_READ = {14, 15, 16, 17, 18, 19};
int[] PWM_WRITE = {3, 5, 6, 9, 10, 11};


// Getting the port name
Console.WriteLine("Enter the COM port number:");
Console.WriteLine("Note - available COM ports:");
foreach(string c_port in SerialPort.GetPortNames())
    Console.WriteLine($"  {c_port}");
Console.Write(">");
string port_name = Console.ReadLine();

// Opening the COM port
Console.WriteLine($"The {port_name} port is selected.");
SerialPort port = new SerialPort();
Console.Write("Opening the port... ");
try {
    port = new SerialPort((string)port_name, SPEED);
    port.NewLine = "\n";
    port.Open();
} catch(Exception e) {
    Console.WriteLine();
    #if DEBUG
        Exit(e.ToString(), 1);
    #else
        Exit("Error when opening the COM port.", 1);
    #endif
}
Console.WriteLine($"Done.");

// Establishing a connection
Console.Write("Establishing a connection... Request ");
int r = 1;
int cl = Console.CursorLeft;
while(port.BytesToRead == 0 && r < MAX_CONNECT_REQUESTS+1) {
    Console.CursorLeft = cl;
    Console.Write(r++);
    port.WriteLine("C");
    Thread.Sleep(CONNECT_REQUEST_DELAY);
}
if(r == MAX_CONNECT_REQUESTS+1)
    Exit($"The response was not received after {MAX_CONNECT_REQUESTS} requests.", 2);
string board_type = port.ReadLine().Substring(1).TrimEnd('_');
Console.WriteLine();
Console.WriteLine($"The connection is established. Board Type: {board_type}");

// Stress Test
Console.CursorTop++;
string packet;
Stopwatch stp = new Stopwatch();
while(true) {
    packet = GeneratePacket();
    stp.Reset();
    stp.Start();
    port.WriteLine(packet);
    string res = port.ReadLine(); 
    stp.Stop();
    Console.CursorTop--;
    long freq = 1000 / stp.ElapsedMilliseconds;
    Console.WriteLine($"Request timeout: {stp.ElapsedMilliseconds} ms. Estimated frequency: {freq} packet{((freq!=1)?"s":"")} per second. ");
    Console.WriteLine(new string(' ', Console.WindowWidth - Console.CursorLeft));
    Console.CursorTop--;
}
// Exit function
void Exit(string e = "", int code = 0) {
    port.Close();
    if(e != "") {
        ConsoleColor c = Console.ForegroundColor;
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
    string packet = "N32";
    Random rnd = new Random();
    for(int i = 0; i < 32; i++) {
        packet += i.ToString().PadLeft(2, '0');
        if(i%2==0) {
            packet += 0;
        } else {
            packet += 1;
            int val = rnd.Next(-32768, 32768);
            if(val < 0) {
                val = -val;
                packet += '-';
            }
            packet += val.ToString().PadLeft(5, '0');    
        }
    }
    packet += "R" + (DIGITAL_READ.Length+ANALOG_READ.Length).ToString().PadLeft(2, '0');
    foreach(int pin in ANALOG_READ) {
        packet += pin.ToString().PadLeft(2, '0');
        packet += '1';
    }
    foreach(int pin in DIGITAL_READ) {
        packet += pin.ToString().PadLeft(2, '0');
        packet += '0';
    }
    packet += "W" + PWM_WRITE.Length.ToString().PadLeft(2, '0');
    foreach(int pin in PWM_WRITE) {
        packet += pin.ToString().PadLeft(2, '0');
        packet += '1';
        packet += rnd.Next(256).ToString().PadLeft(3, '0');
    }
    return packet;
}