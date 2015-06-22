#!/bin/sh
# connects to localhost:4223 by default, use --host and --port to change it

# change to your UID
uid=XYZ

tinkerforge call rs232-bricklet $uid enable-read-callback

# write 4 characters
tinkerforge call rs232-bricklet $uid write t,e,s,t,.. 4

# handle incoming read callback
tinkerforge dispatch rs232-bricklet $uid read-callback
