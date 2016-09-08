extern Print
extern PrintAt
extern Cls
extern MoveCursor
extern SetCursorAt
extern SetCursorNextLine
extern DelChar
extern PutChar
extern PutCharAt

;manul
;0: void Print( char * str); ebx
;1: void PrintAt( int r, int c, char * str); edi ecx ebx
;2: void Cls();
;3: void MoveCursor( int _offset_to_current_position); ebx
;4: void SetCursorAt( int _row, int _col);	ebx,ecx
;5: void SetCursorNextLine();
;6: void DelChar();
;7: void PutChar( char _ch);	ebx
;8: void PutCharAt( int _row, int _col, char _char); ebx,ecx,edi

%macro ij 1
	cmp eax,%1
	jz sc%1
%endmacro

global SystemCallHandler
;void SystemCallHandler( int _function_no);


SystemCallHandler:
	ij 0
	ij 1
	ij 2
	ij 3
	ij 4
	ij 5
	ij 6
	ij 7
	ij 8
	ij 9
	jmp break
	
sc0:
	push ebx
	call Print
	pop ebx
	jmp break
sc1:
	push ebx	;str
	push ecx	;col
	push edi	;row
	call PrintAt
	pop edi
	pop ecx
	pop ebx
	jmp break
sc2:
	call Cls
	jmp break
sc3:
	push ebx
	call MoveCursor
	pop ebx
	jmp break
sc4:
	push ecx
	push ebx
	call SetCursorAt
	pop ebx
	pop ecx
	jmp break
sc5:
	call SetCursorNextLine
	jmp break
sc6:
	call DelChar
	jmp break
sc7:
	push ebx
	call PutChar
	pop ebx
	jmp break
sc8:
	push ebx
	push ecx
	push edi
	call PutCharAt
	pop ebx
	pop ecx
	pop edi
	jmp break
sc9:
	; I really don't know how to shutdown the pc
	jmp break
break:
	ret


