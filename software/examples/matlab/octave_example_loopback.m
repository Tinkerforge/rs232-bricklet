function octave_example_loopback()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change to your UID

    ipcon = java_new("com.tinkerforge.IPConnection"); % Create IP connection
    rs232 = java_new("com.tinkerforge.BrickletRS232", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register read callback to function cb_read
    rs232.addReadCallbackCallback(@cb_read);
    rs232.enableReadCallback();

    rs232.write(string2chars("test"), 4);

    input("Press any key to exit...\n", "s");
    ipcon.disconnect();
end

% Convert string to array of length 60 as needed by write
function chars = string2chars(string)
    chars = javaArray("java.lang.String", 60);

    for i = 1:length(string)
      chars(i) = substr(string, i, 1);
    end

    for i = length(string)+1:60
      chars(i) = "x";
    end
end

% Assume that the message consists of ASCII characters and
% convert it from an array of chars to a string
function string = chars2string(chars, len)
    string = "";

    for i = 1:len
      string = strcat(string, chars(i));
    end
end

% Callback function for illuminance callback (parameter has unit Lux/10)
function cb_read(e)
    message = java_invoke("java.util.Arrays", "copyOf", e.message, e.length);
    len = byte2int(e.length)

    fprintf("Message (length: %d): \"%s\"\n", len, chars2string(e.message, len));
end

function int = byte2int(byte)
    if compare_versions(version(), "3.8", "<=")
        int = byte.intValue();
    else
        int = byte;
    end
end
