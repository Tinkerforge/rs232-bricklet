Imports Tinkerforge
Imports System

' In this program we connect RX to TX and to receive
' the messages that we are sending

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change to your UID

    Function StringToCharArray(ByVal message As string) As Char()
        Dim arr As Char() = message.ToCharArray()
        ReDim Preserve arr(60)

        Return arr
    End Function

    ' Callback function for read callback
    Sub ReadCB(ByVal sender As BrickletRS232, ByVal message As Char(), ByVal length As Byte)
        Dim str as String = new String(message)
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
