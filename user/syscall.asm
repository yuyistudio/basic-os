;将系统调用封装成为c函数
;0: void Print( char * str); ebx
;1: void PrintAt( int r, int c, char * str); edi ecx ebx
;2: void Cls();
;3: void MoveCursor( int _offset_to_current_position); ebx
;4: void SetCursorAt( int _row, int _col);	ebx,ecx
;5: void SetCursorNextLine();
;6: void DelChar();
;7: void PutChar( char _ch);	ebx
;8: void PutCharAt( int _row, int _col, char _char); ebx,ecx,edi
;9: void Shutdown();
%include "const.inc"

global iPrint
global iPrintAt
global iCls
global iMoveCursor
global iSetCursorAt
global iSetCursorNextLine
global iDelChar
global iPutChar
global iPutCharAt
global Shutdown

iPrint:
	push ebp
	mov ebp,esp
	pushad
	mov ebx,[ebp+8]
	mov eax,0
	int INT_VECTOR_SYS_CALL
	popad
	pop ebp
	ret

iPrintAt:
	push ebp
	mov ebp,esp
	pushad
	mov edi,[ebp+8]
	mov ecx,[ebp+12]
	mov ebx,[ebp+16]
	mov eax,1
	int INT_VECTOR_SYS_CALL
	popad
	pop ebp
	ret
iCls:
	push ebp
	mov ebp,esp
	pushad
	mov eax,2
	int INT_VECTOR_SYS_CALL
	popad
	pop ebp
	ret
iMoveCursor:
	push ebp
	mov ebp,esp
	pushad
	mov ebx,[ebp+8]
	mov eax,3
	int INT_VECTOR_SYS_CALL
	popad
	pop ebp
	ret
iSetCursorAt:
	push ebp
	mov ebp,esp
	pushad
	mov ebx,[ebp+8]
	mov ecx,[ebp+12]
	mov eax,4
	int INT_VECTOR_SYS_CALL
	popad
	pop ebp
	ret	
iSetCursorNextLine:
	push ebp
	mov ebp,esp
	pushad
	mov eax,5
	int INT_VECTOR_SYS_CALL
	popad
	pop ebp
	ret
iDelChar:
	push ebp
	mov ebp,esp
	pushad
	mov eax,6
	int INT_VECTOR_SYS_CALL
	popad
	pop ebp
	ret
iPutChar:
	push ebp
	mov ebp,esp
	pushad
	mov ebx,[ebp+8]
	mov eax,7
	int INT_VECTOR_SYS_CALL
	popad
	pop ebp
	ret
iPutCharAt:
	push ebp
	mov ebp,esp
	pushad
	mov edi,[ebp+8]
	mov ecx,[ebp+12]
	mov ebx,[ebp+16]
	mov eax,8
	int INT_VECTOR_SYS_CALL
	popad
	pop ebp
	ret
Shutdown:
	pushad
	mov eax,9
	int INT_VECTOR_SYS_CALL
	popad
	ret
