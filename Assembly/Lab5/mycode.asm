.model small
.stack 100h
.data        

    filename        db "filename.txt"
    buffer          db 128 dup(0)
    buf             db 0    
    handle          dw 0       
    counter         dw 0 
    c               dw 0
    flag            db 0
    space_counter   dw 0  
      
    closeString     db "Close the file$"
    errorExeString  db "Atata, ne zapuskay .exe!$"
    openFileError   db "Error of open!$"  
    openString      db "Open the file$" 
    newLine         db 13, 10, '$'
    errorString     db "Error!$"
    exitString      db "Exit$"
    lastSymbol      db 0
  
.code 
   
;����� ������     
outputString proc
    mov ah, 09h
    int 21h
ret 
outputString endp 
             
;����� \n             
printNewLine proc
    lea dx, newLine
    call outputString
ret
printNewLine endp   
     
;���������� ����� ����� �� ���.������
get_name proc
    push ax  ;��������� ���� ��������
    push cx
    push di
    push si
    xor cx, cx
    mov cl, es:[80h]  ;���������� �������� � ��������� ������
    cmp cl, 0
    je end_get_name
    mov di, 82h       ;�������� ��������� ������ � ����� PSP
    lea si, filename
cicle1:
    mov al, es:[di]   ;������� � al ����������� �������� ��������� ������
    cmp al, 0Dh       ;0Dh - ������� Enter, �� �� ������� �������                                 
    je end_get_name
    mov [si], al      ;������� ������ �� ���.������ � filename 
    inc di            ;�� ��������� ������
    inc si            
    jmp cicle1 
end_get_name:
    dec si
    cmp BYTE PTR [si], 'e'
    je exeError
    
    cmp BYTE PTR [si], 'm'
    je exeError          
    
    pop si            ;��������������� ��������
    pop di
    pop cx
    pop ax   
ret
get_name endp

;�������� ����� ��� ������ � ������
fopen  proc 
   mov ah, 3dh         ;3Dh - ������� ������������ ����
   mov al, 2           ;����� ������� (������ � ������)
   lea dx, filename    ;DS:DX - ���� � �����
   int 21h             ;��������� ���������� (��������)
   jc openError        ;������� ������� - �������, CF = 1
   mov handle, ax      ;��������� �������������     
ret
fopen endp

;�������� �����
fclose proc 
   mov ah, 3eh         ;3Eh - �������� �����
   mov bx, handle      ;�������������
   int 21h             ;��������� �������(����������) ����
   jc error            ;������� � ������    
ret
fclose endp     


checkTab:            
    cmp BYTE PTR [si], 9
    jne notWhiteSpace
    jmp next
    
;�������� ������ �����
proc space 
mov counter, 0
mov space_counter, 0
i:     
    mov cx, 128    ;� cx ���������� ���� ��� ������
    mov bx, handle
    lea dx, buffer  ;� dx ����� ������ ��� ����������
    mov ah, 3fh     
    int 21h
    jc error
    xor cx, cx
    mov cx, ax      ;� �� ���������� ��������� ��������
    jcxz close      ;����� ����� 
    
    push ax
    xor si, si
    mov c, 0        ;������� ����������� ��������
    mov flag, 0
    lea si, buffer  ;����� ������
    cmp BYTE PTR [si], 0
    je close     
            
        k:  
            inc c   ;���������� �������� � ������++
            cmp  BYTE PTR [si], 10  ;newline
            je endOfLine            ;���� ����� ������
            cmp  BYTE PTR [si], ' '
            jne checkTab
            next:  
            pop ax
            cmp ax, c
            je endOfLine
            push ax
            inc si
            jmp k
    jmp i

notWhiteSpace:
    cmp BYTE PTR [si], 13   ;cret
    je cret    
    pop ax
    cmp ax, c
    je endOfLine
    push ax
    mov flag, 1     ;���� = 1, ������ ������ �� ������
    inc si
    jmp k  

nonEmpty:
;���������� ���������
;bx - �������������, cx:dx - ����������, al = 0 - ������������ ������
    xor ax, ax
    mov bx, handle
    mov ah, 42h
    mov dx, counter
    xor cx, cx
    int 21h 

;����� � ����
;bx - �������������, ds:dx - ����� ������ � �������
;cx - ����� ������ ��� ������
    xor ax, ax
    mov bx, handle
    mov ah, 40h
    mov dx, offset buffer
    xor cx, cx
    mov cx, c  
    int 21h
 
;��������� � ������ ����� ����������� �������� ����� ��������, �����������
;�� ������� ������  
    mov ax, counter
    add ax, c
    mov counter, ax

    mov ax, counter
    add ax, space_counter
    mov counter, ax
      
;����� ������ ���������, ������ �� ���� ��� � ������ ��������� ������      
    xor ax, ax
    mov bx, handle
    mov ah, 42h
    mov dx, counter
    xor cx, cx
    int 21h

    mov ax, counter
    sub ax, space_counter
    mov counter, ax
    
    jmp i 

Empty:
;��������� �������� ��������� �������� � �������� � ������ ������ 
    mov ax, c
    add space_counter, ax
    mov ax, counter
    add ax, space_counter
    mov counter, ax

;���������� ��������� � ����� ������ ������   
    xor ax, ax
    mov bx, handle
    mov ah, 42h
    mov dx, counter
    xor cx, cx
    int 21h
   
    mov ax, counter
    sub ax, space_counter
    mov counter, ax

    jmp i

;�������� ����� ������ - ������ ���� �� ��� ������          
endOfLine:
    cmp flag, 1
    je  nonEmpty
    jne Empty
        
cret:
    pop ax
    cmp ax, c
    je endOfLine
    push ax
    inc si
    jmp k
endp     
      
;���� ��������  
error:
    lea dx, errorString
    call outputString
    call printNewLine
    jmp exit  

exeError:
    lea dx, errorExeString
    call outputString
    call printNewLine
    jmp exit
    
      
openError:
    lea dx, openFileError
    call outputString
    call printNewLine
    jmp exit  
               
begin:         
    mov ax, @data
    mov ds, ax
    
    call get_name  ;�������� �������� �����
    call fopen     ;� ��������� ���  
    
    lea dx, openString
    call outputString  
    call printNewLine
    
    call space 
    jmp close 
      
close:                                                             
    
    xor ax, ax
    mov bx, handle
    mov ah, 42h 
    dec counter
    mov dx, counter
    xor cx, cx
    int 21h    

    mov bx, handle
    mov ah, 40h
    int 21h
           
    call fclose 
    
    lea dx, closeString  
    call outputString
    call printNewLine

exit:                  
    lea dx, exitString
    call outputString 
    call printNewLine
    mov ah, 4ch
    int 21h            
end begin