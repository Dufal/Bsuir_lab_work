name "lab3"

PUTC    MACRO   char
        PUSH    AX
        MOV     AL, char
        MOV     AH, 0Eh
        INT     10h     
        POP     AX
ENDM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;





org 100h

jmp start


; define variables:

msg0 db "operations with integer values.",0Dh,0Ah, '$'
msg1 db 0Dh,0Ah, 0Dh,0Ah, 'enter first number: $'
msg2 db "enter the operator:    +  -  *  / or q     : $"
msg3 db "enter second number: $"
msg4 db  0dh,0ah , 'the result of this operation is : $' 
msg5 db  0dh,0ah ,'press any key... ', 0Dh,0Ah, '$'
err1 db  "try again", 0Dh,0Ah , '$'
smth db  " and something.... $"


opr db '?'

; first and second number:
num1 dw ?
num2 dw ?


start:
mov dx, offset msg0
mov ah, 9
int 21h


lea dx, msg1
mov ah, 09h    ; output string at ds:dx
int 21h  




call scan_num

; store first number:
mov num1, cx 



; new line:
putc 0Dh
putc 0Ah




lea dx, msg2
mov ah, 09h     ; output string at ds:dx
int 21h  


; get operator:
mov ah, 1   ; single char input to AL.
int 21h
mov opr, al



; new line:
putc 0Dh
putc 0Ah


cmp opr, 'q'      ; q - exit 
je exit

cmp opr, '*'
jb wrong_opr
cmp opr, '/'
ja wrong_opr






; output of a string at ds:dx
lea dx, msg3
mov ah, 09h
int 21h  




call scan_num


; store second number:
mov num2, cx 




lea dx, msg4
mov ah, 09h      ; output string at ds:dx
int 21h  










cmp opr, '+'
je do_plus

cmp opr, '-'
je do_minus

cmp opr, '*'
je do_mult

cmp opr, '/'
je do_div



wrong_opr:
lea dx, err1
mov ah, 09h     ; output string at ds:dx
int 21h  


exit:
; output of a string at ds:dx
lea dx, msg5
mov ah, 09h
int 21h  


; wait for any key...
mov ah, 0
int 16h


ret  ; return back to os.











do_plus:


mov ax, num1
add ax, num2
call print_num    ; print ax value.

jmp exit



do_minus:

mov ax, num1
sub ax, num2
call print_num    ; print ax value.

jmp exit




do_mult:

mov ax, num1
imul num2 ; (dx ax) = ax * num2. 
call print_num    ; print ax value.


jmp exit




do_div:

mov dx, 0
mov ax, num1
idiv num2  ; ax = (dx ax) / num2.
cmp dx, 0
jnz approx
call print_num    ; print ax value.
jmp exit
approx:
call print_num    ; print ax value.
lea dx, smth
mov ah, 09h    ; output string at ds:dx
int 21h  
jmp exit













; gets the multi-digit SIGNED number from the keyboard,
; and stores the result in CX register:
SCAN_NUM        PROC    NEAR
        PUSH    DX
        PUSH    AX
        PUSH    SI
        
        MOV     CX, 0

        ; reset flag:
        MOV     CS:make_minus, 0

next_digit:

        ; get char from keyboard
        ; into AL:
        MOV     AH, 00h
        INT     16h
        ; and print it:
        MOV     AH, 0Eh
        INT     10h

        ; check for MINUS:
        CMP     AL, '-'
        JE      set_minus

        ; check for ENTER key:
        CMP     AL, 0Dh  ; carriage return?
        JNE     not_cr
        JMP     stop_input
not_cr:


        CMP     AL, 8                   
        JNE     backspace_checked
        MOV     DX, 0                   ; remove last digit by
        MOV     AX, CX                  ; division:
        DIV     CS:ten                .
        MOV     CX, AX
        PUTC    ' '                     ; clear position.
        PUTC    8                      
        JMP     next_digit
backspace_checked:


        ; allow only digits:
        CMP     AL, '0'
        JAE     ok_AE_0
        JMP     remove_not_digit
ok_AE_0:        
        CMP     AL, '9'
        JBE     ok_digit
remove_not_digit:       
        PUTC    8       ; backspace.
        PUTC    ' '     ; clear last entered not digit.
        PUTC    8       ; backspace again.        
        JMP     next_digit ; wait for next input.       
ok_digit:


        ; multiply CX by 10 (first time the result is zero)
        PUSH    AX
        MOV     AX, CX
        MUL     CS:ten                  ; DX:AX = AX*10
        MOV     CX, AX
        POP     AX

        ; check if the number is too big
        ; (result should be 16 bits)
        CMP     DX, 0
        JNE     too_big

        ; convert from ASCII code:
        SUB     AL, 30h

        ; add AL to CX:
        MOV     AH, 0
        MOV     DX, CX      ; backup, in case the result will be too big.
        ADD     CX, AX
        JC      too_big2    ; jump if the number is too big.

        JMP     next_digit

set_minus:
        MOV     CS:make_minus, 1
        JMP     next_digit

too_big2:
        MOV     CX, DX      ; restore the backuped value before add.
        MOV     DX, 0       ; DX was zero before backup!
too_big:
        MOV     AX, CX
        DIV     CS:ten  ; reverse last DX:AX = AX*10, make AX = DX:AX / 10
        MOV     CX, AX
        PUTC    8       ; backspace.
        PUTC    ' '     ; clear last entered digit.
        PUTC    8       ; backspace again.        
        JMP     next_digit ; wait for Enter/Backspace.
        
        
stop_input:
        ; check flag:
        CMP     CS:make_minus, 0
        JE      not_minus
        NEG     CX
not_minus:

        POP     SI
        POP     AX
        POP     DX
        RET
make_minus      DB      ?       ; used as a flag.
SCAN_NUM        ENDP





; this procedure prints number in AX,
; used with PRINT_NUM_UNS to print signed numbers:
PRINT_NUM       PROC    NEAR
        PUSH    DX
        PUSH    AX

        CMP     AX, 0
        JNZ     not_zero

        PUTC    '0'
        JMP     printed

not_zero:
        ; the check SIGN of AX,
        ; make absolute if it's negative:
        CMP     AX, 0
        JNS     positive
        NEG     AX

        PUTC    '-'

positive:
        CALL    PRINT_NUM_UNS
printed:
        POP     AX
        POP     DX
        RET
PRINT_NUM       ENDP



; this procedure prints out an unsigned
; number in AX (not just a single digit)
; allowed values are from 0 to 65535 (FFFF)
PRINT_NUM_UNS   PROC    NEAR
        PUSH    AX
        PUSH    BX
        PUSH    CX
        PUSH    DX

       
        MOV     CX, 1

       .
        MOV     BX, 10000      

        
        CMP     AX, 0
        JZ      print_zero

begin_print:

       :
        CMP     BX,0
        JZ      end_print

        :
        CMP     CX, 0
        JE      calc
        ; if AX<BX then result of DIV will be zero:
        CMP     AX, BX
        JB      skip
calc:
        MOV     CX, 0   ; set flag.

        MOV     DX, 0
        DIV     BX      ; AX = DX:AX / BX   (DX=remainder).

        ; print last digit
        ; AH is always ZERO, so it's ignored
        ADD     AL, 30h    ; convert to ASCII code.
        PUTC    AL


        MOV     AX, DX  ; get remainder from last div.

skip:
        ; calculate BX=BX/10
        PUSH    AX
        MOV     DX, 0
        MOV     AX, BX
        DIV     CS:ten  ; AX = DX:AX / 10   (DX=remainder).
        MOV     BX, AX
        POP     AX

        JMP     begin_print
        
print_zero:
        PUTC    '0'
        
end_print:

        POP     DX
        POP     CX
        POP     BX
        POP     AX
        RET
PRINT_NUM_UNS   ENDP



ten             DW      10      







GET_STRING      PROC    NEAR
PUSH    AX
PUSH    CX
PUSH    DI
PUSH    DX

MOV     CX, 0                   ; char counter.

CMP     DX, 1                   ; buffer too small?
JBE     empty_buffer            ;

DEC     DX                      ; reserve space for last zero.




wait_for_key:

MOV     AH, 0                   ; get pressed key.
INT     16h

CMP     AL, 0Dh                  ; 'RETURN' pressed?
JZ      exit_GET_STRING


CMP     AL, 8                   ; 'BACKSPACE' pressed?
JNE     add_to_buffer
JCXZ    wait_for_key            ; nothing to remove!
DEC     CX
DEC     DI
PUTC    8                       ; backspace.
PUTC    ' '                     ; clear position.
PUTC    8                       ; backspace again.
JMP     wait_for_key

add_to_buffer:

        CMP     CX, DX          ; buffer is full?
        JAE     wait_for_key    ; if so wait for 'BACKSPACE' or 'RETURN'...

        MOV     [DI], AL
        INC     DI
        INC     CX
        
        ; print the key:
        MOV     AH, 0Eh
        INT     10h

JMP     wait_for_key
;============================

exit_GET_STRING:


MOV     [DI], 0

empty_buffer:

POP     DX
POP     DI
POP     CX
POP     AX
RET
GET_STRING      ENDP