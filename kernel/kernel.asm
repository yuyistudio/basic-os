bits 32

%include "const.inc"

; C functions
extern	CInit			; do init work
extern	CMain			; system starts to work
extern	ExceptionHandler	; system handler
extern	KeyboardHandler
extern	ClockHandler
extern	SystemCallHandler


; C variables
extern	gdt_ptr		; for reloading gdt
extern	idt_ptr		; for loading idt
extern	p_proc_ready	; we need to get back to user process
extern	tss		; we need to refresh tss when get back to process
extern	k_reenter	; judge if reenter


[SECTION .bss]
StackSpace		resb	2 * 1024
StackTop:

[section .text]	; 代码在此

global _start	; 导出 _start

; start process
global restart

; int handlers
global  sys_call
global	exception_handler
global	clock_handler
global	keyboard_handler

global print_a_string

_start:

	; load system stack
	mov	esp, StackTop

	; init system
	sgdt	[gdt_ptr]	; save the addr of GDT into var gdt_ptr
	call	CInit		; in 'start.c'
	lgdt	[gdt_ptr]
	lidt	[idt_ptr]
	jmp	SELECTOR_KERNEL_CS:csinit
csinit:	
	xor	eax, eax
	mov	ax, SELECTOR_TSS
	ltr	ax
	
	; system start
	jmp	CMain



; called after int happened
save:
        pushad        
        push    ds    
        push    es   
        push    fs   
        push    gs     
        mov     dx, ss
        mov     ds, dx
        mov     es, dx

        mov     esi, esp                  
	; 如果发生了中断重入(已经在系统栈)
	; 将返回地址restart_reenter压入系统栈
	; 否则
	; 切换到系统栈,并将返回地址restart压入系统栈
	; 最终都跳回调用者继续执行
	; 可以看到,这里的RET_ADDR发挥作用了
        inc     dword [k_reenter]          
        cmp     dword [k_reenter], 0       
        jne     .1	;若有中断重入，即现在已经在系统堆栈了，则跳转。
        mov     esp, StackTop       ;没有中断重入 。从用户进程切到这里。      
        push    restart      ;push时esp指向的是系统堆栈，所以将中断返回地址压入了系统堆栈。             
        jmp     [esi + RETADR - P_STACKBASE];esi指向PCB最低地址处。jmp到了retaddr处。retaddr的值是在call save的时候push进去的。相当于用jmp代替了ret了。
.1:;中断重入了，从系统进程到现在的位置。
        push    restart_reenter          
        jmp     [esi + RETADR - P_STACKBASE]
        

; clock      
clock_handler:	
	;时钟中断发生
	;sub esp,4	;跳过retaddr字段	
	;上面这句不需要写，因为call的时候做了一次Push eip，所以很奇妙地跳过了retaddr字段	
	call	save
	;告诉8259，中断处理结束了，现在可以进行下一次中断了
	in	al, INT_M_CTLMASK	
	or	al, 0x1	
	out	INT_M_CTLMASK, al
	mov	al, EOI			
	out	INT_M_CTL, al		
	;打开中断，允许下一次中断
	sti
	;允许本次中断的处理程序。这个程序可能被中断嵌套
	call	ClockHandler		
	;关中断，进行中断程序返回。
	cli
	in	al, INT_M_CTLMASK
	and	al, ~ 0x1	
	out	INT_M_CTLMASK, al
	ret

; keyboard
keyboard_handler:		
	call	save
	in	al, INT_M_CTLMASK	
	or	al, 0x2	
	out	INT_M_CTLMASK, al
	mov	al, EOI			
	out	INT_M_CTL, al		
	sti	
	call	KeyboardHandler
	cli
	in	al, INT_M_CTLMASK
	and	al, ~ 0x2
	out	INT_M_CTLMASK, al
	ret



; exceptions
exception_handler:
	call	ExceptionHandler
	add	esp, 4*2
	hlt


; system call
sys_call:
	; 若没有中断重入,则需要特权级别转换
	; 中断时从TSS取得的ESP指向被中断进程的PCB的STACK_FRAME顶端
	; 再自动保存当前进程的SS ESP CS EIP EFLAG
	; 这个时候,返回地址ESP保存到了PCB的RET_ADDR这个字段中
        call    save

        sti
	
	; 系统调用的实现
	; 调用功能号存储在eax中
	; 调用系统调用处理C函数
        call    SystemCallHandler
        ; 将函数的返回值存储到TSS中,以便作为返回值返回给CALLER
        mov     [esi + EAXREG - P_STACKBASE], eax
	
        cli

        ret



; start process
restart:
	; 载入当前进程的LDT
	; 将当前进程的PCB首地址保存到ESP中
	; load the ldt of current proc
	mov	esp, [p_proc_ready]
	lldt	[esp + P_LDT_SEL]	; P_LDT_SEL is the offset of ldt selector in the PCB of current proc
	; 将当前进程的PCB中的用户进程STACK_FRAME的顶端(高地址端)的地址
	; 返回时从TSS取得该值,于是将参数保存到用户进程堆栈中
	lea	eax, [esp + P_STACKTOP]; P_STACKTOP IS JUST P_LDT_SEL IN PCB
	; the value in eax is the address of STACKTOP in PCB
	mov	dword [tss + TSS3_S_SP0], eax	;  TSS3_S_SP0 is 4. ( tss + TSS3_S_SP0) points to the ESP0 in TSS
restart_reenter:
	dec	dword [k_reenter]
	; 为进程运行初始化各个寄存器值
	; 这些值在创建进程时就已经保存到了进程的PCB中,现在用上了这些值
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	; 跳过RET_ADDR
	add	esp, 4
	; 从PCB中取得用户程序的 CS EIP EFLAG, 并开始执行用户程序的指令
	; 特权级别由RING0到RING1或者RING3
	iretd

