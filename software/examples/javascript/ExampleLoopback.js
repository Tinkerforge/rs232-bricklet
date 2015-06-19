var Tinkerforge = require('tinkerforge');

// For this example connect the RX1 and TX pin to receive the send message

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change to your UID

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var rs232 = new Tinkerforge.BrickletRS232(UID, ipcon); // Create device object

// Convert string to array of length 60 as needed by write
function stringToCharArray(message) {
    var array = [];
    for(var i = 0; i < message.length; i++) {
        array.push(message[i]);
    }
    for(var i = message.length; i < 60; i++) {
        array.push('\0');
    }
    return array
}

// Assume that the message consists of ASCII characters and
// convert it from an array of chars to a string
function charArrayToString(message, length) {
    return message.slice(0, length).join('');
}

ipcon.connect(HOST, PORT,
    function(error) {
        console.log('Error: '+error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function(connectReason) {
        rs232.enableReadCallback();

        var message = 'test';
        rs232.write(stringToCharArray(message), message.length);
    }
);

// Register callback
rs232.on(Tinkerforge.BrickletRS232.CALLBACK_READ_CALLBACK,
    // Callback function for read callback
    function(message, length) {
        var str = charArrayToString(message, length);
        console.log('Message (length: ' + length + '): "' + str + '"');
    }
);

console.log("Press any key to exit ...");
process.stdin.on('data',
    function(data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
