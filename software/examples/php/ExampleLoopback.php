<?php

// In this program we connect RX to TX and to receive
// the messages that we are sending

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletRS232.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletRS232;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change to your UID

// Callback function for read callback
function cb_read($message, $length)
{
    echo "message (length: " . $length . "): \"" . implode($message) . "\"\n";
}

$ipcon = new IPConnection(); // Create IP connection
$rs232 = new BrickletRS232(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register read callback to function cb_read
$rs232->registerCallback(BrickletRS232::CALLBACK_READ_CALLBACK, 'cb_read');
$rs232->enableCallback();

$message = "test\n";
$rs232->write(str_split($message), strlen($message));

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
