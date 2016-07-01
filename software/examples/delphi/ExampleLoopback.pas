program ExampleLoopback;

{$ifdef MSWINDOWS}{$apptype CONSOLE}{$endif}
{$ifdef FPC}{$mode OBJFPC}{$H+}{$endif}

{ For this example connect the RX1 and TX pin to receive the send message }

uses
  SysUtils, IPConnection, BrickletRS232;

type
  TExample = class
  private
    ipcon: TIPConnection;
    rs232: TBrickletRS232;
  public
    procedure ReadCB(sender: TBrickletRS232;
                     const msg: TArray0To59OfChar; const len: byte);
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'XYZ'; { Change XYZ to the UID of your RS232 Bricklet }

var
  e: TExample;

{ Callback function for read callback }
procedure TExample.ReadCB(sender: TBrickletRS232;
                          const msg: TArray0To59OfChar; const len: byte);
begin
  WriteLn(Format('Message (Length: %d): "%s"', [len, Copy(msg, 0, len)]));
end;

procedure TExample.Execute;
begin
  { Create IP connection }
  ipcon := TIPConnection.Create;

  { Create device object }
  rs232 := TBrickletRS232.Create(UID, ipcon);

  { Connect to brickd }
  ipcon.Connect(HOST, PORT);
  { Don't use device before ipcon is connected }

  { Register read callback to procedure ReadCB }
  rs232.OnReadCallback := {$ifdef FPC}@{$endif}ReadCB;

  { Enable read callback }
  rs232.EnableReadCallback;

  { Write "test" string }
  rs232.Write('test', 4);

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy; { Calls ipcon.Disconnect internally }
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
