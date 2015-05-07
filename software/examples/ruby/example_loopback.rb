#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

# In this program we connect RX to TX and to receive
# the messages that we are sending

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_rs232'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change to your UID

def string_to_char_array(str)
  # Use char array of size 60, as required by API
  arr = str.split(//);
  arr += ['\0']*(60 - str.length)
  return arr
end

ipcon = IPConnection.new # Create IP connection
rs232 = BrickletRS232.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Register read callback
rs232.register_callback(BrickletRS232::CALLBACK_READ_CALLBACK) do |message, length|
  puts "Message (length #{length}): #{message.join('')}"
end

message = "test"

rs232.write string_to_char_array(message), message.length

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
