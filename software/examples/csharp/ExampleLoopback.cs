using System;
using Tinkerforge;

// For this example connect the RX1 and TX pin to receive the send message

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change to your UID

	// Convert message to array of length 60 as needed by write
	static char[] StringToCharArray(string message)
	{
		char[] array = message.ToCharArray();

		Array.Resize(ref array, 60);

		return array;
	}

	// Assume that the message consists of ASCII characters and
	// convert it from an array of chars to a string
	static string CharArrayToString(char[] message, byte length)
	{
		return new string(message, 0, length);
	}

	// Callback function for read callback
	static void ReadCB(BrickletRS232 sender, char[] message, byte length)
	{
		string str = CharArrayToString(message, length);
		Console.WriteLine("Message (length: " + length + "): \"" + str + "\"");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletRS232 rs232 = new BrickletRS232(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register read callback to function ReadCB
		rs232.ReadCallback += ReadCB;

		// Enable read callback
		rs232.EnableReadCallback();

		// Write "test" string
		rs232.Write(StringToCharArray("test"), 4);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
