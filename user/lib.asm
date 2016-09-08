%include "const.inc"

info db "damn",0

global userproc
;void userproc();
userproc:
	lea ebx,[info]
	mov edi,10	;row
	mov ecx,10	;col
	mov eax,1
	int INT_VECTOR_SYS_CALL
	jmp $
