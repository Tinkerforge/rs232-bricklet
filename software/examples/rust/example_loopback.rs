use std::{error::Error, io, thread};
use tinkerforge::{ip_connection::IpConnection, rs232_bricklet::*};

// For this example connect the RX1 and TX pin to receive the send message

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your RS232 Bricklet

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let rs232 = Rs232Bricklet::new(UID, &ipcon); // Create device object

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd
                                          // Don't use device before ipcon is connected

    //Create receiver for read events.
    let read_receiver = rs232.get_read_receiver();
    // Spawn thread to handle received events. This thread ends when the rs232
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for read in read_receiver {
            let message: String = read.message.iter().collect();
            println!("Message (Length: {}) {}", read.length, message);
        }
    });

    // Enable read callback
    rs232.enable_read_callback();

    let mut chars = ['\0'; 60];
    let test_chars = ['t', 'e', 's', 't'];
    chars[0..4].copy_from_slice(&test_chars);

    // Write "test" string
    rs232.write(chars, 4);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
