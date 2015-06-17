#!/usr/bin/perl  

# In this program we connect RX to TX and to receive
# the messages that we are sending

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletRS232;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change to your UID

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $rs232 = Tinkerforge::BrickletRS232->new(&UID, $ipcon); # Create device object

# Callback function for read callback
sub cb_read
{
    my ($message, $length) = @_;

    print "message (length: " . $length . "): \"" . join("", $message) . "\"\n";
}

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register read callback to function cb_read
$rs232->register_callback($rs232->CALLBACK_READ_CALLBACK, 'cb_read');
$rs232->enable_read_callback();

# Convert string to char array with length 60, as needed by API
my @message = split("", "test\n");
my @arr = ('\0') x 60;
for my $i (0..@message-1) {
	$arr[$i] = $message[$i];
}

$rs232->write(\@arr, 5);

print "Press any key to exit...\n";
<STDIN>;
$ipcon->disconnect();
