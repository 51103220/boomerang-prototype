; Turbo51 version 0.1.3.10, Copyright 2000 - 2011 Igor Funa

$REGISTERBANK (0)

_CODE         SEGMENT  CODE
_CONST        SEGMENT  CODE
_DATA         SEGMENT  DATA

              EXTRN    DATA  (CurrentIO)
              EXTRN    IDATA (StackStart)

              EXTRN    CODE  (sysWriteStr_CODE)
              EXTRN    CODE  (sysWriteLine)

; {
;     This file is part of the Turbo51 examples.
;     Copyright (C) 2008 - 2011 by Igor Funa
; 
;     http://turbo51.com/
; 
;     This file is distributed in the hope that it will be useful,
;     but WITHOUT ANY WARRANTY; without even the implied warranty of
;     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
; }
; 
; Program HelloWorld;
; 
; Const

RSEG _CONST

;  Osc      = 22118400;
;  BaudRate = 19200;
; 
;  BaudRateTimerValue = Byte (- Osc div 12 div 32 div BaudRate);
; 
; Var SerialPort: Text;

RSEG _DATA

SerialPort:                     DS       4

; 
; Procedure WriteToSerialPort; Assembler;


RSEG _CODE

USING 0

WriteToSerialPort:
; Asm
;   CLR   TI
              CLR       SCON.1

;   MOV   SBUF, A
              MOV       SBUF, A

; @WaitLoop:
L_001D:
;   JNB   TI, @WaitLoop
              JNB       SCON.1, L_001D

; end;
              RET

; 
; Procedure Init;

Init:
; begin
;   TL1  := BaudRateTimerValue;
              MOV       TL1, #$FD

;   TH1  := BaudRateTimerValue;
              MOV       TH1, #$FD

;   TMOD := %00100001;    { Timer1: no GATE, 8 bit timer, autoreload }
              MOV       TMOD, #$21

;   SCON := %01010000;    { Serial Mode 1, Enable Reception }
              MOV       SCON, #$50

;   TI   := True;         { Indicate TX ready }
              SETB      SCON.1

;   TR1  := True;         { Enable timer 1 }
              SETB      TCON.6

; 
;   Assign (SerialPort, WriteToSerialPort);
              MOV       SerialPort, #LOW  (WriteToSerialPort)
              MOV       SerialPort+1, #HIGH (WriteToSerialPort)

; end;
              RET

RSEG _CONST

C_0071:                         DB        12, 'Hello world!'


CSEG AT $0000

HelloWorld:
; 
; begin
              MOV       SP, #StackStart-1

;   Init;
              LCALL     Init

;   Writeln (SerialPort, 'Hello world!');
              MOV       CurrentIO, SerialPort
              MOV       CurrentIO+1, SerialPort+1
              MOV       DPTR, #C_0071
              MOV       R6, #0
              LCALL     sysWriteStr_CODE
              LCALL     sysWriteLine

; end.
L_0017:
              SJMP      L_0017

              END
