#!/bin/sh
# connects to localhost:4223 by default, use --host and --port to change it

# change to your UID
uid=XYZ

tinkerforge call rs232-bricklet $uid enable-callback

# We use 4 characters and fill the rest up to reach a total 
# of 60 chars, as required by the API.
tinkerforge call rs232-bricklet $uid write t,e,s,t,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9 4

# handle incoming read callback
tinkerforge dispatch rs232-bricklet $uid read-callback

