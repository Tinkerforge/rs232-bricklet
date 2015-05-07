import com.tinkerforge.BrickletRS232;
import com.tinkerforge.IPConnection;

// In this program we connect RX to TX and to receive
// the messages that we are sending

public class ExampleLoopback {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;
	private static final String UID = "XYZ"; // Change to your UID

	public static char[] stringToCharArray(String message) {
		char[] arr = new char[60];
		for(int i = 0; i < message.length() ; i++) {
			arr[i] = message.charAt(i);
		}

		return arr;
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
				String str = new String(message);
				System.out.println("message (length: " + length + "): \"" + str + "\"");
			}
		});

		rs232.enableCallback();
		rs232.write(stringToCharArray("test\n"), (short)5);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
