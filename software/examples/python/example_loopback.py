#!/usr/bin/env python
# -*- coding: utf-8 -*-  

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change to your UID

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_rs232 import RS232

# In this program we connect RX to TX and to receive
# the messages that we are sending

# Converts a string into a list of chars of length 60
# as it is needed by the RS232 write method
def string_to_charlist(s):
    clist = list(s)
    clist.extend([' ']*(60-len(s)))
    return clist, len(s)

def cb_read(message, length):
    print('receive: "' + ''.join(message[:length]) + '"')

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    rs232 = RS232(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    rs232.register_callback(rs232.CALLBACK_READ_CALLBACK, cb_read)
    rs232.enable_callback()

    rs232.write(*string_to_charlist('test\n'))

    raw_input('Press key to exit\n') # Use input() in Python 3
    ipcon.disconnect()
