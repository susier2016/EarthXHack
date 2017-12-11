.ORIG x3000

; I/O program #1
; based off of r/dailyprogrammer's challenge #1
; LC-3 Assembly
; Matthew Yu
; 12/11/17

; This program will ask the user's name, age, and reddit username.
; It will return in the format:
; your name is (blank), you are (blank) years old, and your username is (blank).

; clear registers
AND R0, R0, #0
AND R1, R1, #0
AND R2, R2, #0
AND R3, R3, #0
AND R4, R4, #0
AND R5, R5, #0
AND R6, R6, #0
AND R7, R7, #0

JSR GETNAME
JSR GETAGE
JSR GETUSERN
JSR CONCATENATE
HALT

; GETNAME subroutine
; gets the name input from the user and stores it into memory.
GETNAME
  ST R7, SAVE_R7_GN
  LEA R1, NAME_MEM
  LD R4, ENTER
  LEA R0, NAME_PROMPT
  PUTS
LOOP_N
  GETC
  OUT
  ; check if key is enter
  ADD R3, R0, R4
  BRNP STORE_N
  BR END_N
; store char and inc storage pointer if key != enter
STORE_N
  STR R0, R1, #0
  ADD R1, R1, #1
  BR  LOOP_N
; end method if key = enter
END_N
  LD R7, SAVE_R7_GN
  RET

SAVE_R7_GN  .BLKW #1

; GETAGE subroutine
; gets the age input from the user and stores it into memory.
GETAGE
  ST R7, SAVE_R7_GA
  LEA R1, AGE_MEM
  LD R4, ENTER
  LEA R0, AGE_PROMPT
  PUTS
  LOOP_A
    GETC
    OUT
    ; check if key is enter
    ADD R3, R0, R4
    BRNP STORE_A
    BR END_A
  ; store char and inc storage pointer if key != enter
  STORE_A
    STR R0, R1, #0
    ADD R1, R1, #1
    BR  LOOP_A
  ; end method if key = enter
  END_A
  LD R7, SAVE_R7_GA
  RET

SAVE_R7_GA  .BLKW #1

; GETAGE subroutine
; gets the username input from the user and stores it into memory.
GETUSERN
  ST R7, SAVE_R7_GUN
  LEA R1, USERN_MEM
  LD R4, ENTER
  LEA R0, USERNAME_PROMPT
  PUTS
  LOOP_UN
    GETC
    OUT
    ; check if key is enter
    ADD R3, R0, R4
    BRNP STORE_UN
    BR END_UN
  ; store char and inc storage pointer if key != enter
  STORE_UN
    STR R0, R1, #0
    ADD R1, R1, #1
    BR  LOOP_UN
  ; end method if key = enter
  END_UN
  LD R7, SAVE_R7_GUN
  RET

SAVE_R7_GUN  .BLKW #1

; CONCATENATE subroutine
; takes all previous inputs, formats it, and outputs to console.
CONCATENATE
  ST R7, SAVE_R7_C
  LEA R0, C_OUT_1
  PUTS
  LEA R0, NAME_MEM
  PUTS
  LEA R0, C_OUT_2
  PUTS
  LEA R0, AGE_MEM
  PUTS
  LEA R0, C_OUT_3
  PUTS
  LEA R0, USERN_MEM
  PUTS
  LD R0, PERIOD
  OUT

  LD R7, SAVE_R7_C
  RET

SAVE_R7_C .BLKW #1

ENTER .FILL x-A
AGE_MEM   .BLKW #4
NAME_MEM  .BLKW #25
USERN_MEM .BLKW #25
NAME_PROMPT  .STRINGZ "What is your name? "
USERNAME_PROMPT  .STRINGZ "What is your username? "
AGE_PROMPT  .STRINGZ "What is your age? "
C_OUT_1 .STRINGZ  "Your name is "
C_OUT_2 .STRINGZ  ", you are "
C_OUT_3 .STRINGZ  " years old, and your username is "
PERIOD  .FILL x2E
; your name is (blank), you are (blank) years old, and your username is (blank).
.END
