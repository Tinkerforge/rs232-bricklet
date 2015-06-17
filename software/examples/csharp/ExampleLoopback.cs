using Tinkerforge;
using System;

// In this program we connect RX to TX and to receive
// the messages that we are sending

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change to your UID

	static char[] StringToCharArray(string message)
	{
		char[] arr = message.ToCharArray();
		Array.Resize(ref arr, 60);
		return arr;
	}

	// Callback function for read callback (parameter has unit cm)
	static void ReadCB(BrickletRS232 sender, char[] message, byte length)
	{
		string str = new string(message);
		System.Console.WriteLine("message (length: " + length + "): \"" + str + "\"");
	}

	static void Main() 
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletRS232 rs232 = new BrickletRS232(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register read callback to function ReadCB
		rs232.ReadCallback += ReadCB;
		rs232.EnableReadCallback();

		rs232.Write(StringToCharArray("test\n"), 5);

		System.Console.WriteLine("Press enter to exit");
		System.Console.ReadLine();
		ipcon.Disconnect();
	}
}
