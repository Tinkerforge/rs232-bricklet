use std::{error::Error, io, thread};
use tinkerforge::{ipconnection::IpConnection, rs232_bricklet::*};

// For this example connect the RX1 and TX pin to receive the send message

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your RS232 Bricklet

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let rs232_bricklet = RS232Bricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    //Create listener for read events.
    let read_listener = rs232_bricklet.get_read_receiver();
    // Spawn thread to handle received events. This thread ends when the rs232_bricklet
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for event in read_listener {
            let message: String = event.message.iter().collect();
            println!("Message (Length: {}) {}", event.length, message);
        }
    });

    // Enable read callback
    rs232_bricklet.enable_read_callback();

    let mut chars = ['\0'; 60];
    let test_chars = ['t', 'e', 's', 't'];
    chars[0..4].copy_from_slice(&test_chars);

    // Write "test" string
    rs232_bricklet.write(chars, 4);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
