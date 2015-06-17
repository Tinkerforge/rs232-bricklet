#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_rs232.h"

#define HOST "localhost"
#define PORT 4223
#define UID "XYZ" // Change to your UID

// In this program we connect RX to TX and to receive
// the messages that we are sending

// Callback function for read callback
void cb_read(char *message, uint8_t length, void *user_data) {
	(void)user_data; // avoid unused parameter warning

	printf("message (length: %d): \"%s\"\n", length, message);
}

int main() {
	// Create IP connection
	IPConnection ipcon;
	ipcon_create(&ipcon);

	// Create device object
	RS232 rs232;
	rs232_create(&rs232, UID, &ipcon); 

	// Connect to brickd
	if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
		fprintf(stderr, "Could not connect\n");
		exit(1);
	}
	// Don't use device before ipcon is connected

	// Register read callback to function cb_read
	rs232_register_callback(&rs232,
	                        RS232_CALLBACK_READ_CALLBACK,
	                        (void *)cb_read,
	                        NULL);
	rs232_enable_read_callback(&rs232);
	rs232_write(&rs232, "test\n", 5);

	printf("Press key to exit\n");
	getchar();
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
}
