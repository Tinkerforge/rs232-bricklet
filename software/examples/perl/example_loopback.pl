#!/usr/bin/perl

# For this example connect the RX1 and TX pin to receive the send message

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletRS232;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change to your UID

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $rs232 = Tinkerforge::BrickletRS232->new(&UID, $ipcon); # Create device object

# Convert string to char array with length 60, as needed by write
sub string_to_char_array
{
    my ($message) = @_;

    my @chars = split("", $message);
    my @array = ("\0") x 60;
    for my $i (0..scalar(@chars)-1) {
        $array[$i] = $chars[$i];
    }

    return \@array;
}

# Assume that the message consists of ASCII characters and
# convert it from an array of chars to a string
sub char_array_to_string
{
    my ($message, $length) = @_;

    return substr(join("", $message), 0, $length);
}

# Callback function for read callback
sub cb_read
{
    my ($message, $length) = @_;

    my $str = char_array_to_string($message, $length);
    print "Message (length: " . $length . "): \"" . $str . "\"\n";
}

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register read callback to function cb_read
$rs232->register_callback($rs232->CALLBACK_READ_CALLBACK, 'cb_read');
$rs232->enable_read_callback();

my @message = string_to_char_array("test");
$rs232->write(@message, 4);

print "Press any key to exit...\n";
<STDIN>;
$ipcon->disconnect();
