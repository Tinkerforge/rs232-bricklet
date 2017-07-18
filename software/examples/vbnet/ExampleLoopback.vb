Imports System
Imports Tinkerforge

' For this example connect the RX1 and TX pin to receive the send message

Module ExampleLoopback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your RS232 Bricklet

    ' Convert message to array of length 60 as needed by write
    Function StringToCharArray(ByVal message As String) As Char()
        Dim chars As Char() = message.ToCharArray()
        ReDim Preserve chars(60)
        Return chars
    End Function

    ' Assume that the message consists of ASCII characters and
    ' convert it from an array of chars to a string
    Function CharArrayToString(ByVal message As Char(), ByVal length As Byte) As String
        Return new String(message, 0, length)
    End Function

    ' Callback subroutine for read callback
    Sub ReadCB(ByVal sender As BrickletRS232, ByVal message As Char(), ByVal length As Byte)
        Dim ascii as String = CharArrayToString(message, length)
        Console.WriteLine("Message (Length: {0}): ""{1}""", length, ascii)
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim rs232 As New BrickletRS232(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register read callback to subroutine ReadCB
        AddHandler rs232.ReadCallback, AddressOf ReadCB

        ' Enable read callback
        rs232.EnableReadCallback()

        ' Write "test" string
        rs232.Write(StringToCharArray("test"), 4)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
