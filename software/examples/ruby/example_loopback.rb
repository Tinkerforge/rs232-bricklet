#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

# For this example connect the RX1 and TX pin to receive the send message

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_rs232'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change to your UID

# Convert string to char array with length 60, as needed by write
def string_to_char_array(message)
  chars = message.split(//)
  chars += ['\0']*(60 - message.length)
  return chars
end

# Assume that the message consists of ASCII characters and
# convert it from an array of chars to a string
def char_array_to_string(message, length)
  return message.slice(0, length).join('')
end

ipcon = IPConnection.new # Create IP connection
rs232 = BrickletRS232.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Register read callback
rs232.register_callback(BrickletRS232::CALLBACK_READ_CALLBACK) do |message, length|
  puts "Message (Length: #{length}): #{char_array_to_string message, length}"
end

# Enable read callback
rs232.enable_read_callback

# Write "test" string
message = "test"
rs232.write string_to_char_array(message), message.length

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
