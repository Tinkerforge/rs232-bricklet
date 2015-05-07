program ExampleCallback;

{$ifdef MSWINDOWS}{$apptype CONSOLE}{$endif}
{$ifdef FPC}{$mode OBJFPC}{$H+}{$endif}

{ In this program we connect RX to TX and to receive }
{ the messages that we are sending }

uses
  SysUtils, IPConnection, BrickletRS232;

type
  TExample = class
  private
    ipcon: TIPConnection;
    rs232: TBrickletRS232;
  public
    procedure ReadCB(sender: TBrickletRS232; const message: TArray0To59OfChar; const len: byte);
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'XYZ'; { Change to your UID }

var
  e: TExample;

{ Callback function for read callback }
procedure TExample.ReadCB(sender: TBrickletRS232; const message: TArray0To59OfChar; const len: byte);
begin
  WriteLn(Format('message (length %d): "%s"', [len, message]));
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
  rs232.EnableCallback();

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