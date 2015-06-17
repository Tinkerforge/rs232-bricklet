var Tinkerforge = require('tinkerforge');

// In this program we connect RX to TX and to receive
// the messages that we are sending

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change to your UID

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var rs232 = new Tinkerforge.BrickletRS232(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function(error) {
        console.log('Error: '+error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function(connectReason) {
        rs232.enableReadCallback();

        message = 'test\n';

        // Convert message to array of length 60 as needed by write
        var arr = [];
        for(var i = 0; i < message.length; i++) {
            arr.push(message[i]);
        }

        for(var i = message.length; i < 60; i++) {
            arr.push('\0');
        }

        rs232.write(arr, message.length);
    }
);

// Register callback
rs232.on(Tinkerforge.BrickletRS232.CALLBACK_READ_CALLBACK,
    // Callback function for read callback
    function(message, length) {
        str = message.join('');
        console.log('message (length: ' + length + '): "' + str + '"');
    }

);

console.log("Press any key to exit ...");
process.stdin.on('data',
    function(data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
