import java.util.Arrays;
import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletRS232;

// For this example connect the RX1 and TX pin to receive the send message

public class ExampleLoopback {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;
	private static final String UID = "XYZ"; // Change to your UID

	// Convert string to array of length 60 as needed by write
	public static char[] stringToCharArray(String message) {
		return Arrays.copyOf(message.toCharArray(), 60);
	}

	// Assume that the message consists of ASCII characters and
	// convert it from an array of chars to a string
	public static String charArrayToString(char[] message, short length) {
		return new String(message, 0, length);
	}

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions you
	//       might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletRS232 rs232 = new BrickletRS232(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Add and implement read listener
		rs232.addReadCallbackListener(new BrickletRS232.ReadCallbackListener() {
			public void readCallback(char[] message, short length) {
				String str = charArrayToString(message, length);
				System.out.println("Message (length: " + length + "): \"" + str + "\"");
			}
		});

		rs232.enableReadCallback();
		rs232.write(stringToCharArray("test"), (short)4);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
