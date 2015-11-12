#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change to your UID

# Handle incoming read callback
tinkerforge dispatch rs232-bricklet $uid read-callback &

# Enable read callback
tinkerforge call rs232-bricklet $uid enable-read-callback

# Write "test" string
tinkerforge call rs232-bricklet $uid write t,e,s,t,.. 4

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
