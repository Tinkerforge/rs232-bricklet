<?php

// For this example connect the RX1 and TX pin to receive the send message

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletRS232.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletRS232;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your RS232 Bricklet

// Convert string to char array with length 60, as needed by write
function stringToCharArray($message)
{
    $chars = str_split($message);

    while (sizeof($chars) < 60)
    {
        array_push($chars, '\0');
    }

    return $chars;
}

// Assume that the message consists of ASCII characters and
// convert it from an array of chars to a string
function charArrayToString($message, $length)
{
    return implode(array_slice($message, 0, $length));
}

// Callback function for read callback
function cb_read($message, $length)
{
    $str = charArrayToString($message, $length);
    echo "Message (Length: " . $length . "): \"" . $str . "\"\n";
}

$ipcon = new IPConnection(); // Create IP connection
$rs232 = new BrickletRS232(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register read callback to function cb_read
$rs232->registerCallback(BrickletRS232::CALLBACK_READ_CALLBACK, 'cb_read');

// Enable read callback
$rs232->enableReadCallback();

// Write "test" string
$message = "test";
$rs232->write(stringToCharArray($message), strlen($message));

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
