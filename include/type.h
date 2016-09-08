#include "const.h"

#ifndef	_ORANGES_TYPE_H_
#define	_ORANGES_TYPE_H_

// externed type
typedef int			BOOL;
typedef	unsigned int		u32;
typedef	unsigned short		u16;
typedef	unsigned char		u8;
typedef char *			p_stack;
// functions pointer
typedef	void	(*int_handler)	();
typedef void	(*p_code)	();
typedef	void	(*irq_handler)	( int _irq);

// protected mode struct
typedef struct s_descriptor		/* 共 8 个字节 */
{
	u16	limit_low;		/* Limit */
	u16	base_low;		/* Base */
	u8	base_mid;		/* Base */
	u8	attr1;			/* P(1) DPL(2) DT(1) TYPE(4) */
	u8	limit_high_attr2;	/* G(1) D(1) 0(1) AVL(1) LimitHigh(4) */
	u8	base_high;		/* Base */
}DESCRIPTOR;
typedef struct s_gate
{
	u16	offset_low;	/* Offset Low */
	u16	selector;	/* Selector */
	u8	dcount;		/* 该字段只在调用门描述符中有效。
				如果在利用调用门调用子程序时引起特权级的转换和堆栈的改变，需要将外层堆栈中的参数复制到内层堆栈。
				该双字计数字段就是用于说明这种情况发生时，要复制的双字参数的数量。 */
	u8	attr;		/* P(1) DPL(2) DT(1) TYPE(4) */
	u16	offset_high;	/* Offset High */
}GATE;
typedef struct s_tss {
	u32	backlink;
	u32	esp0;		/* stack pointer to use during interrupt */
	u32	ss0;		/*   "   segment  "  "    "        "     */
	u32	esp1;
	u32	ss1;
	u32	esp2;
	u32	ss2;
	u32	cr3;
	u32	eip;
	u32	flags;
	u32	eax;
	u32	ecx;
	u32	edx;
	u32	ebx;
	u32	esp;
	u32	ebp;
	u32	esi;
	u32	edi;
	u32	es;
	u32	cs;
	u32	ss;
	u32	ds;
	u32	fs;
	u32	gs;
	u32	ldt;
	u16	trap;
	u16	iobase;	
}TSS;

// process struct
typedef struct s_stackframe {
	u32	gs;
	u32	fs;
	u32	es;
	u32	ds;	
	u32	edi;	
	u32	esi;	
	u32	ebp;	
	u32	kernel_esp;	
	u32	ebx;		
	u32	edx;		
	u32	ecx;		
	u32	eax;		
	u32	retaddr;	
	u32	eip;	
	u32	cs;	
	u32	eflags;	
	u32	esp;		
	u32	ss;	
}STACK_FRAME;
typedef struct s_proc {
	STACK_FRAME	regs;
	u16		ldt_sel;	
	DESCRIPTOR	ldts[LDT_SIZE];
	u32		pid;	
	char		p_name[16];
	BOOL		empty;	// point out whether this pcb is empty or not
	int		privilege;
	int		counter;
}PROCESS;

#endif /* _ORANGES_TYPE_H_ */
