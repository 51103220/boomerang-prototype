         ORG   0x00       ; this statement is not really necessary as it
                          ;   is the default behavior of the assembler
         LJMP  Start      ; jump over the prog mem locations reserved for ISVs
                          ;   (interrupt service vectors) and jump over
                          ;   the non-executable DB statements brought in
                          ;   by the inclusion of the FontData.txt file.

         ORG   0x03       ; the ISV for the EXT 0 interrupt is 0x0003
EXT0INT: LJMP  Int0Isr
        
         ORG   0x0B       ; the ISV for the TMR 0 interrupt is 0x000B
TMR0INT: LJMP  Tmr0Isr

         ORG   0x13       ; the ISV for the EXT 1 interrupt is 0x0013
EXT1INT: LJMP  Int1Isr

         ORG   0x1B       ; the ISV for the TMR 1 interrupt is 0x001B
TMR1INT: LJMP  Tmr1Isr

         ORG   0x23       ; the ISV for the Serial Port interrupt is 0x0023
SERINT:  LJMP  SerIsr

INCLUDE  "FontData.txt"

         ORG   0x100      ; the interrupt service vectors consume program memory
                          ;   locations 0x03 thru 0x25 = 37 and the DB statements
                          ;   in FontData.txt consume another 27*8 = 216 bytes
                          ;   (remember there is a space character after Z).
                          ;   Hence this program memory address of 0x100 = 256 is
                          ;   safely past these items.
Start:   MOV   DPTR,#StartOfImageData
                          ; We initialize the DPTR ("data pointer"), a 16 bit
                          ;   SFR ("special function register"), to point to
                          ;   the first column of font data (the first column
                          ;   of the letter 'A') in program memory.  The DPTR
                          ;   register continues to hold this same value
                          ;   throughout this program.
         MOV   SCON,#0x10 ; assert REN so the serial port is enabled to receive
         MOV   IE,#0x90   ; 10010000B, MSBit is the global intr enable,
                          ;   the other bit we raise is ES, the serial
                          ;   port interrupt enable

         MOV   A, #0x01   ; we walk this 1 bit across the signboard while
                          ;   waiting for an interrupt

Loop:    RL    A          ; loop while waiting for an interrupt
         MOV   P0,A
         SJMP  Loop


; If we somehow land in any of the interrupt service routines that this
; program is not prepared to handle we just initiate an infinite loop
; so we can detect the mistake.

Int0Isr:                  ; interrupt service routine for EXT 0 interrupt
         SJMP  Int0Isr
        
Int1Isr:                  ; interrupt service routine for EXT 1 interrupt
         SJMP  Int1Isr

Tmr0Isr:                  ; interrupt service routine for TMR 0 interrupt
         SJMP  Tmr0Isr

Tmr1Isr:                  ; interrupt service routine for TMR 1 interrupt
         SJMP  Tmr1Isr

SerIsr:                   ; interrupt service routine for the serial port
                          ;  interrupt
         PUSH  ACC        ; protect the prior value in the ACC since the
                          ;  interrupt service routine needs to use it

         MOV   A,SBUF     ; read alphabet index from serial buffer SFR
         LCALL Draw1Char

         POP   ACC        ; restore the prior value of the ACC
         MOV   SCON,#0x10 ; clear the RI bit in SCON (leave REN asserted)
         RETI

Draw1Char:
; This is a subroutine which is responsible for outputting exactly 8
; columns (hence 1 character) of font data to the electric sign board.
; Before you call this subroutine initialize the ACC with the index
; of the desired character.  That is, if you want the letter 'C'
; output to the sign board then call this subroutine with ACC = 2.

; We first need to convert the letter index (ranging from 0 to 26)
; to a column index (ranging from 0 to 208).  This column index
; will describe the byte offset to that letter's column data.

                          ; the ACC presently holds an alphabet index
           MOV   B,#8     ; each letter has 8 columns
           MUL   AB       ; This computes the byte offset beyond
                          ;   StartOfImageData where the letter starts. This
                          ;   product is guaranteed to be < 256 since the
                          ;   entire alphabet only has 216 (27*8) columns
                          ;   (remember there is a space char after Z),
                          ;   hence we only need to keep (deal with) the
                          ;   LSByte of the product.  The LSByte is found
                          ;   in the ACC following the "MUL AB" instr.

; Within this subroutine we use R0 as a temporary storage location
; for the accumulator's value (we need this because the "MOVC A,@A+DPTR"
; instruction keeps perturbing the accumulator).  And we use the
; R1 register to control the loop (that is, decide how many times
; we iterate).  Note that R0 is synonymous with data memory location 0
; and R1 is synonymous with data memory location 1 (at least while the
; 8051 is in its default configuration).

           MOV   R0,A     ; preserve this starting column # since the
                          ;   upcoming "MOVC A,@A+DPTR" instruction
                          ;   replaces the value in the ACC
           MOV   R1,#0    ; we use R1 to control the looping

CharLoop:  MOV  A,R0      ; prepare for upcoming "MOVC A,@A+DPTR" instruction
           MOVC A,@A+DPTR ; the ACC now holds one column of font data
           MOV  P0,A      ; write that column to the sign board
           INC  R0        ; advance to the next column
           INC  R1        ; increment the loop counter
           CJNE R1,#8,CharLoop
                          ; we loop 8 times in order to output 8 columns
           RET