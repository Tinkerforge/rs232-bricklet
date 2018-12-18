package main

import (
    "fmt"
    "github.com/tinkerforge/go-api-bindings/ipconnection"
    "github.com/tinkerforge/go-api-bindings/rs232_bricklet"
)

// For this example connect the RX1 and TX pin to receive the send message

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your RS232 Bricklet.

func main() {
	ipcon := ipconnection.New()
    defer ipcon.Close()
	rs232, _ := rs232_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
    defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	rs232.RegisterReadCallback(func(message [60]rune, length uint8) {
		fmt.Println("Message (Length %d) %s", length, string(message[:length]))
	})

	// Enable read callback
	rs232.EnableReadCallback()

    var chars [60]rune
    testChars := []rune{'t','e','s','t'}
    copy(chars[:], testChars)
    
    rs232.Write(chars, 4)
    
	fmt.Print("Press enter to exit.")
	fmt.Scanln()

	ipcon.Disconnect()
}
