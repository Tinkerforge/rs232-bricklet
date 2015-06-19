Imports Tinkerforge

' For this example connect the RX1 and TX pin to receive the send message

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change to your UID

    ' Convert message to array of length 60 as needed by write
    Function StringToCharArray(ByVal message As String) As Char()
        Dim chars As Char() = message.ToCharArray()
        ReDim Preserve chars(60)
        Return chars
    End Function

    ' Assume that the message consists of ASCII characters and
    ' convert it from an array of chars to a string
    Function CharArrayToString(ByVal message As Char(), ByVal length As Byte) As String
        Dim str as String = new String(message, 0, length)
        Return str
    End Function

    ' Callback function for read callback
    Sub ReadCB(ByVal sender As BrickletRS232, ByVal message As Char(), ByVal length As Byte)
        Dim str as String = CharArrayToString(message, length)
        System.Console.WriteLine("message (length: {0}): '{1}'", length, str)
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim rs232 As New BrickletRS232(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register read callback to function ReadCB
        AddHandler rs232.ReadCallback, AddressOf ReadCB
        rs232.EnableReadCallback()

        rs232.Write(StringToCharArray("test"), 4)

        System.Console.WriteLine("Press key to exit")
        System.Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
