; this file contains all ASM lib functions



%include "const.inc"


[SECTION .text]

; export functions

; string opetation
global	disp_str
global	disp_color_str

; interface operation
global	out_byte
global	in_byte

; control 8259A
global  enable_irq
global  disable_irq

; control int in CPU
global	disable_int
global	enable_int
global	halt


; string operation
global	memcpy
global	memset
global  strcpy



; void halt();
halt:
	hlt
ret


; void out_byte(u16 port, u8 value);
out_byte:
	mov	edx, [esp + 4]		
	mov	al, [esp + 4 + 4]	
	out	dx, al
	nop	
	nop
	ret


; u8 in_byte(u16 port);
in_byte:
	mov	edx, [esp + 4]		
	xor	eax, eax
	in	al, dx
	nop	
	nop
	ret


; void disable_irq(int irq); irq0 is for timer, irq1 is for keyboard, irq2 is for...
disable_irq:
        mov     ecx, [esp + 4]        
        pushf
        cli
        mov     ah, 1
        rol     ah, cl                
        cmp     cl, 8
        jae     disable_8             
disable_0:
        in      al, INT_M_CTLMASK
        test    al, ah
        jnz     dis_already          
        or      al, ah
        out     INT_M_CTLMASK, al   
        popf
        mov     eax, 1                
        ret
disable_8:
        in      al, INT_S_CTLMASK
        test    al, ah
        jnz     dis_already             
        or      al, ah
        out     INT_S_CTLMASK, al      
        popf
        mov     eax, 1                 
        ret
dis_already:
        popf
        xor     eax, eax             
        ret


; void enable_irq(int irq);
enable_irq:
        mov     ecx, [esp + 4]       
        pushf
        cli
        mov     ah, ~1
        rol     ah, cl                 
        cmp     cl, 8
        jae     enable_8                
enable_0:
        in      al, INT_M_CTLMASK
        and     al, ah
        out     INT_M_CTLMASK, al      
        popf
        ret
enable_8:
        in      al, INT_S_CTLMASK
        and     al, ah
        out     INT_S_CTLMASK, al       ; clear bit at slave 8259
        popf
        ret
disable_int:
	cli
ret

enable_int:
	sti
ret



; void* memcpy(void* es:p_dst, void* ds:p_src, int size);
memcpy:
	push	ebp
	mov	ebp, esp

	push	esi
	push	edi
	push	ecx

	mov	edi, [ebp + 8]	
	mov	esi, [ebp + 12]	
	mov	ecx, [ebp + 16]	
.1:
	cmp	ecx, 0		
	jz	.2		

	mov	al, [ds:esi]		
	inc	esi			
					
	mov	byte [es:edi], al	
	inc	edi			

	dec	ecx		
	jmp	.1		
.2:
	mov	eax, [ebp + 8]

	pop	ecx
	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp

	ret			


; void memset(void* p_dst, char ch, int size);
memset:
	push	ebp
	mov	ebp, esp

	push	esi
	push	edi
	push	ecx

	mov	edi, [ebp + 8]	
	mov	edx, [ebp + 12]	
	mov	ecx, [ebp + 16]	
.1:
	cmp	ecx, 0		
	jz	.2		

	mov	byte [edi], dl		
	inc	edi			

	dec	ecx		
	jmp	.1		
.2:

	pop	ecx
	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp

	ret		


; char* strcpy(char* p_dst, char* p_src);
strcpy:
	push    ebp
	mov     ebp, esp

	mov     esi, [ebp + 12] 
	mov     edi, [ebp + 8] 

.1:
	mov     al, [esi]              
	inc     esi                  
					
	mov     byte [edi], al   
	inc     edi                  

	cmp     al, 0         
	jnz     .1           

	mov     eax, [ebp + 8]

	pop     ebp
	ret                    


